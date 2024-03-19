#include "Graph.h"

template<typename T, int N>
Graph<T, N>::Graph(const U8G2_SSD1306_128X64_NONAME_F_HW_I2C &display, int x1, int y1, int x2, int y2) {
  _x1 = x1;
  _y1 = y1;
  _x2 = x2;
  _y2 = y2;

  _display = display;
}

template<typename T, int N>
void Graph<T, N>::drawFrame() {
  _display.drawFrame(_x1, _y1, _x2 - _x1, _y2 - _y1);
}