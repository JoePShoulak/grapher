#include <Wire.h>
#include <U8g2lib.h>
#include "CircularBuffer.h"
#include <float.h>
#include "Graph.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GRAPH_HEIGHT 50
#define POINT_COUNT 16  // TODO: Why can't this be 32?
#define GRAPH_MARGIN 3

int pointSpacing;
CircularBuffer<float, POINT_COUNT> dataPoints;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// Graph<float, 16> graph(display, 0, 0, 30, 15);

void setup() {
  pointSpacing = SCREEN_WIDTH / POINT_COUNT;

  display.begin();
  display.setPowerSave(0);
  display.setFont(u8g2_font_helvR08_tf);

  Serial.begin(9600);

  display.drawFrame(0, SCREEN_HEIGHT - GRAPH_HEIGHT, SCREEN_WIDTH, GRAPH_HEIGHT);
  display.setCursor(0, 10);
  display.print("Temp: ?F");

  display.sendBuffer();
}

template<typename T, int N>
T getMax(CircularBuffer<T, N> buffer) {
  T max = buffer[0];

  for (int i = 1; i < dataPoints.count(); i++) {
    T yCurr = dataPoints[i];

    if (yCurr > max)
      max = yCurr;
  }

  return max;
}

template<typename T, int N>
T getMin(CircularBuffer<T, N> buffer) {
  T min = buffer[0];

  for (int i = 1; i < dataPoints.count(); i++) {
    T yCurr = dataPoints[i];

    if (yCurr < min)
      min = yCurr;
  }

  return min;
}

void graphData() {

  display.drawFrame(0, SCREEN_HEIGHT - GRAPH_HEIGHT, SCREEN_WIDTH, GRAPH_HEIGHT);

  float min = getMin(dataPoints);
  float max = getMax(dataPoints);

  for (int i = 1; i < dataPoints.count(); i++) {
    int xPrev = pointSpacing * (i - 1);
    int xCurr = pointSpacing * i;

    int yPrev = map(dataPoints[i - 1], min, max, SCREEN_HEIGHT - GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_HEIGHT + GRAPH_MARGIN);
    int yCurr = map(dataPoints[i], min, max, SCREEN_HEIGHT - GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_HEIGHT + GRAPH_MARGIN);

    if (i == dataPoints.index() - 1) {
      display.drawDisc(xCurr, yCurr, GRAPH_MARGIN - 1);
    }

    display.drawLine(xPrev, yPrev, xCurr, yCurr);
  }
}

void loop() {
  if (!Serial.available()) return;

  display.clearBuffer();

  String rawValue = Serial.readString();
  rawValue.trim();
  dataPoints.append(rawValue.toFloat());
  graphData();
  display.setCursor(0, 10);
  display.print("Temp: ");
  display.print(rawValue.c_str());
  display.print("F");

  display.sendBuffer();
}
