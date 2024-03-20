#ifndef Graph_h
#define Graph_h

#include <U8g2lib.h>
#include <Arduino.h>
#include "CircularBuffer.h"

template<typename T, int N>
class Graph {
public:
  Graph(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int screenWidth, int screenHeight, int graphHeight, int graphMargin, int pointCount);

  T getY(int i, T min, T max);
  void drawLine(int i, T min, T max);
  CircularBuffer<T, N> buffer;

private:
  int _screenWidth;
  int _screenHeight;
  int _graphHeight;
  int _graphMargin;
  int _pointCount;
  int _pointSpacing;

  U8G2_SSD1306_128X64_NONAME_F_HW_I2C &_display;
};

template<typename T, int N>
Graph<T, N>::Graph(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int screenWidth, int screenHeight, int graphHeight, int graphMargin, int pointCount)
  : _display(display) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _graphHeight = graphHeight;
  _graphMargin = graphMargin;
  _pointCount = pointCount;
  _pointSpacing = _screenWidth / (_pointCount - 1);
}

template<typename T, int N>
T Graph<T, N>::getY(int i, T min, T max) {
  return map(buffer[i], min, max, _screenHeight - _graphMargin, _screenHeight - _graphHeight + _graphMargin);
}

template<typename T, int N>
void Graph<T, N>::drawLine(int i, T min, T max) {
  int xPrev = _pointSpacing * (i - 1);
  int xCurr = _pointSpacing * i;

  int yPrev = this->getY(i - 1, min, max);
  int yCurr = this->getY(i, min, max);

  _display.drawLine(xPrev, yPrev, xCurr, yCurr);
}

#endif
