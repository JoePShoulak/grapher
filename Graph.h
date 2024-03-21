#ifndef Graph_h
#define Graph_h

#include <U8g2lib.h>
#include <Arduino.h>
#include <CircularBuffer.h>

// TODO: Limit this to numeric types
// TODO: Add options for label and unit for value display
// CONSIDER: Add option for fixed yMin and yMax instead of autoscaling
// CONSIDER: Add different scroll modes
template<typename T, int N>
class Graph {
public:
  Graph(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int screenWidth, int screenHeight, int graphHeight, int graphMargin, int pointCount);
  void begin();
  void update(String rawValue);

private:
  int _screenWidth, _screenHeight, _graphHeight, _graphMargin, _pointCount, _pointSpacing;

  CircularBuffer<T, N> _buffer;
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C &_display;

  T getY(int i, T min, T max);
  void drawLine(int i, T min, T max);
  void drawPoint(int i, T min, T max);
  void updateGraph(String rawValue);
  void updateText(String rawValue);
};

template<typename T, int N>
Graph<T, N>::Graph(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int screenWidth, int screenHeight, int graphHeight, int graphMargin, int pointCount)
  : _display(display), _screenWidth(screenWidth), _screenHeight(screenHeight), _graphHeight(graphHeight), _graphMargin(graphMargin), _pointCount(pointCount) {
  _pointSpacing = _screenWidth / (_pointCount - 1);
}

template<typename T, int N>
T Graph<T, N>::getY(int i, T min, T max) {
  return map(_buffer[i], min, max, _screenHeight - _graphMargin, _screenHeight - _graphHeight + _graphMargin);
}

template<typename T, int N>
void Graph<T, N>::drawLine(int i, T min, T max) {
  int xPrev = _pointSpacing * (i - 1);
  int xCurr = _pointSpacing * i;

  int yPrev = this->getY(i - 1, min, max);
  int yCurr = this->getY(i, min, max);

  _display.drawLine(xPrev, yPrev, xCurr, yCurr);
}

template<typename T, int N>
void Graph<T, N>::drawPoint(int i, T min, T max) {
  int xCurr = _pointSpacing * i;
  int yCurr = this->getY(i, min, max);

  _display.drawDisc(xCurr, yCurr, _graphMargin - 1);
}

template<typename T, int N>
void Graph<T, N>::updateText(String rawValue) {
  _display.setCursor(0, 10);
  _display.print("Temp: ");
  _display.print(rawValue.c_str());
  _display.print("F");
}

template<typename T, int N>
void Graph<T, N>::updateGraph(String rawValue) {
  _display.drawFrame(0, _screenHeight - _graphHeight, _screenWidth, _graphHeight);

  if (rawValue == "?") return;

  _buffer.append(rawValue.toFloat());

  T min = _buffer.minimum();
  T max = _buffer.maximum();

  for (int i = 0; i < _buffer.count(); i++) {
    if (i == _buffer.index() - 1 || i == _buffer.count() - 1)
      this->drawPoint(i, min, max);

    if (i != 0)
      this->drawLine(i, min, max);
  }
}

template<typename T, int N>
void Graph<T, N>::update(String rawValue) {
  _display.clearBuffer();
  this->updateText(rawValue);
  this->updateGraph(rawValue);
  _display.sendBuffer();
}

template<typename T, int N>
void Graph<T, N>::begin() {
  _display.begin();
  _display.setPowerSave(0);
  _display.setFont(u8g2_font_helvR08_tf);

  _display.clearBuffer();
  this->updateText("?");
  this->updateGraph("?");
  _display.sendBuffer();
}

#endif
