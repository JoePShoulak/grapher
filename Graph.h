#ifndef Graph_h
#define Graph_h

#include <U8g2lib.h>
#include <Arduino.h>
#include "CircularBuffer.h"

template <typename T, int N>
class Graph {
public:
  Graph(U8G2_SSD1306_128X64_NONAME_F_HW_I2C display, int x1, int y1, int x2, int y2);

  void drawFrame();
private:
  CircularBuffer<T, N> _buffer;

  int _x1;
  int _y1;
  int _x2;
  int _y2;

  U8G2_SSD1306_128X64_NONAME_F_HW_I2C _display;
};

#endif
