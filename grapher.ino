#include <Wire.h>
#include <U8g2lib.h>
#include "CircularBuffer.h"
#include "bufferHelper.h"
// #include "Graph.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GRAPH_HEIGHT 50
#define POINT_COUNT 16  // TODO: Why can't this be 32?
#define GRAPH_MARGIN 3

int pointSpacing;
CircularBuffer<float, POINT_COUNT> dataPoints;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// Graph<float, 16> graph(display, 0, 0, 30, 15);

float getY(int i, float min, float max) {
  return map(dataPoints[i], min, max, SCREEN_HEIGHT - GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_HEIGHT + GRAPH_MARGIN);
}

void drawLine(int i, float min, float max) {
  int xPrev = pointSpacing * (i - 1);
  int xCurr = pointSpacing * i;

  int yPrev = getY(i-1, min, max);
  int yCurr = getY(i, min, max);

  display.drawLine(xPrev, yPrev, xCurr, yCurr);
}

void drawPoint(int i, float min, float max) {
  int xCurr = pointSpacing * i;
  int yCurr = getY(i, min, max);

  display.drawDisc(xCurr, yCurr, GRAPH_MARGIN - 1);
}

String getData() {
  String rawValue = Serial.readString();
  rawValue.trim();
  return rawValue;
}

void updateText(String rawValue) {
  display.setCursor(0, 10);
  display.print("Temp: ");
  display.print(rawValue.c_str());
  display.print("F");
}

void updateGraph(String rawValue) {
  display.drawFrame(0, SCREEN_HEIGHT - GRAPH_HEIGHT, SCREEN_WIDTH, GRAPH_HEIGHT);

  if (rawValue == "") return;

  dataPoints.append(rawValue.toFloat());

  float min = getMin(dataPoints);
  float max = getMax(dataPoints);

  for (int i = 0; i < dataPoints.count(); i++) {
    if (i == dataPoints.index() - 1) 
      drawPoint(i, min, max);

    if (i != 0) 
      drawLine(i, min, max);
  }
}

void setup() {
  pointSpacing = SCREEN_WIDTH / POINT_COUNT;

  display.begin();
  display.setPowerSave(0);
  display.setFont(u8g2_font_helvR08_tf);

  Serial.begin(9600);

  display.clearBuffer();
  updateText("?");
  updateGraph("");
  display.sendBuffer();
}

void loop() {
  if (!Serial.available()) return;

  String rawValue = getData();

  display.clearBuffer();
  updateText(rawValue);
  updateGraph(rawValue);
  display.sendBuffer();
}
