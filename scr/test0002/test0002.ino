#include <TFT_eSPI.h>  // Thư viện điều khiển màn hình
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Khởi tạo đối tượng màn hình

void setup() {
  tft.init();               // Khởi tạo màn hình
  tft.setRotation(1);       // Xoay màn hình (0-3)
  tft.fillScreen(TFT_BLACK); // Xóa màn hình về màu đen

  tft.setTextColor(TFT_WHITE); // Đặt màu chữ
  tft.setTextSize(2);         // Đặt kích thước chữ
  tft.setCursor(50, 50);      // Đặt vị trí chữ
  tft.print("MAY LOC KHONG KHI IOT");   // Hiển thị chữ
}

void loop() {
  
}
