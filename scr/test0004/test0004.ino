void setup() {
    Serial.begin(115200);  // Debug
    Serial1.begin(9600);   // UART1 để gửi sang ESP32
}

void loop() {
    float data[6] = {1.23, 4.56, 12, 10.11, 12.34, 15.67};

    Serial1.write('<');  // Gửi ký tự bắt đầu
    Serial1.write((uint8_t*)data, sizeof(data));  // Gửi mảng float dạng nhị phân
    Serial1.write('>');  // Gửi ký tự kết thúc

    Serial.println("Đã gửi mảng float!");
    delay(1000);
}
