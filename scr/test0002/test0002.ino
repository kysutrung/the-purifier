#include <TFT_eSPI.h>  // Thư viện màn hình TFT
#include <SPI.h>

// Khởi tạo màn hình TFT
TFT_eSPI tft = TFT_eSPI();

// Định nghĩa chân
#define RXD2 16  
#define TXD2 17

#define RELAY01 32
#define RELAY02 33

#define BUTTON_A 25  
#define BUTTON_B 27 

int sysMode = 0;  // Biến số lưu giá trị

// Khai báo 6 biến float để lưu dữ liệu
float var1, var2, var3, var4, var5, var6;

// Hàm khởi tạo màn hình TFT
void initTFT() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
}

// Hàm nhận dữ liệu từ Serial2
bool receiveFloatArray(float* buffer) {
    static bool receiving = false;
    static uint8_t dataBuffer[24]; 
    static int index = 0;

    while (Serial2.available()) {
        char c = Serial2.read();

        if (c == '<') {  
            receiving = true;
            index = 0;
        } else if (c == '>' && receiving) {  
            if (index == 24) {  // 6 float * 4 byte = 24 byte
                memcpy(buffer, dataBuffer, 24);
                return true;  // Nhận thành công
            }
            receiving = false;
        } else if (receiving && index < 24) {
            dataBuffer[index++] = c;
        }
    }
    return false;
}

// Hàm cập nhật giá trị vào biến
void updateVariables(float* buffer) {
    var1 = buffer[0];
    var2 = buffer[1];
    var3 = buffer[2];
    var4 = buffer[3];
    var5 = buffer[4];
    var6 = buffer[5];
}

// Hàm hiển thị dữ liệu lên màn hình TFT
void displayOnTFT() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.printf("Air Quality Index: %.2f\n", var1);
    tft.setCursor(10, 40);
    tft.printf("Toxic Gas: %.2f\n", var2);
    tft.setCursor(10, 70);
    tft.printf("Equivalent CO2: %.2f\n", var3);
    tft.setCursor(10, 100);
    tft.printf("Temperature: %.2f\n", var4);
    tft.setCursor(10, 130);
    tft.printf("Humidity: %.2f\n", var5);
    tft.setCursor(10, 160);
    tft.printf("Dust Density: %.2f\n", var6);
}

void getButton(){
  if(digitalRead(BUTTON_A) == LOW) {  // Nếu nút A được nhấn
    sysMode = 2;
  }
  if(digitalRead(BUTTON_B) == LOW) {  // Nếu nút B được nhấn
    sysMode = 4;
  }
  if(digitalRead(BUTTON_A) == LOW && digitalRead(BUTTON_B) == LOW){
    sysMode = sysMode;
  }
}

void setRelay(int n){
  if(n == 2){
    digitalWrite(RELAY01, 0);
    digitalWrite(RELAY02, 0);
  }
  else{
    digitalWrite(RELAY01, 1);
    digitalWrite(RELAY02, 1);
  }
}

void setup() {
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);

    pinMode(RELAY01, OUTPUT);
    pinMode(RELAY02, OUTPUT);
    digitalWrite(RELAY01, 1);
    digitalWrite(RELAY02, 1);

    Serial.begin(115200);  
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  
    initTFT();  // Khởi tạo màn hình TFT
}

void loop() {

    getButton();
    setRelay(sysMode);

    float receivedData[6];

    if (receiveFloatArray(receivedData)) {  // Nếu nhận thành công
        updateVariables(receivedData);  // Cập nhật biến
        displayOnTFT();  // Hiển thị lên màn hình
    }
}
