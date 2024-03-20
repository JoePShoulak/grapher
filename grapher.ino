#include <Wire.h>
#include <U8g2lib.h>
#include "Graph.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GRAPH_HEIGHT 50
#define GRAPH_MARGIN 3
#define POINT_COUNT 17  // TODO: Why can't this be 32/33?

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
Graph<float, POINT_COUNT> graph(display, SCREEN_WIDTH, SCREEN_HEIGHT, GRAPH_HEIGHT, GRAPH_MARGIN, POINT_COUNT);

void setup() {
  Serial.begin(9600);
  graph.begin();
}

void loop() {
  if (Serial.available())
    graph.update();
}
