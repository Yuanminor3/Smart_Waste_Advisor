// display.logic.cpp
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define X_OFFSET 50
#define Y_OFFSET 30
#define LINE_HEIGHT 20
#define MAX_LINE_LENGTH 26 

// Initial Settings, size, position...
void display_init() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("GPT Waste Tips:", X_OFFSET + 20, Y_OFFSET, 2);  // Moved up and right
}

// Display result
void display_result(const char* gpt_text) {
  tft.fillRect(0, 30 + Y_OFFSET, 240, 200, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int y = 40 + Y_OFFSET;
  String text = String(gpt_text);
  while (text.length() > 0) {
    int split = text.lastIndexOf(' ', min((unsigned int)MAX_LINE_LENGTH, text.length()));
    if (split <= 0) split = min((unsigned int)MAX_LINE_LENGTH, text.length());

    String line = text.substring(0, split);
    tft.drawString(line, X_OFFSET, y, 2);
    text = text.substring(split);
    text.trim();  // remove leading space
    y += LINE_HEIGHT;
  }
}
