#ifndef Graph_h
#define Graph_h

#include <U8g2lib.h>
#include <Arduino.h>
#include "CircularBuffer.h"

template <typename T, int N>
class Graph {
public:
  Graph(const U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int x1, int y1, int x2, int y2);
  CircularBuffer<T, N> buffer;

private:
  int _x1;
  int _y1;
  int _x2;
  int _y2;

  const U8G2_SSD1306_128X64_NONAME_F_HW_I2C &_display;
};

template<typename T, int N>
Graph<T, N>::Graph(const U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int x1, int y1, int x2, int y2) : _display(display) {
  _x1 = x1;
  _y1 = y1;
  _x2 = x2;
  _y2 = y2;
}

#endif
