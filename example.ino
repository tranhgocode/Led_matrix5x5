#include "MatrixAnimation.h"

#define SH_PIN 13  // Shift Clock (SH)
#define DS_PIN 12  // Data Serial (DS)
#define ST_PIN 11  // Storage Clock (ST)

// set_up chan
int rowPins[5] = {10, 9, 8, 7, 6};

void setup() {
  // set baudrate cho UART
  Serial.begin(9600);

  // Khởi tạo số random
  randomSeed(analogRead(0));

  // Cấu hình chân shift register
  pinMode(SH_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);
  pinMode(ST_PIN, OUTPUT);

  // Cấu hình chân hàng làm đầu ra
  for (int i = 0; i < 5; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);
  }


void loop() {
  //scrollStrNgang("A");
  //scrollStrDoc("A");
  //LedXoay(2, 50); // de 250 cho da nhin
  //randomFlicker(500, 10);  // 2000ms, tỉ lệ nháy 10% của 2 ma trận led (50)
  //delay(500);
  
  // Hiệu ứng nháy điểm ảnh ngẫu nhiên
  //randomPixelFlicker(500, 2);  // 2000ms, 10 điểm sáng
  //delay(500);

  HieuUngGiotNuoc(2, 100); // 2 chu kỳ, tốc độ 100ms
  //delay(500);
  
  // Hiệu ứng mưa
  //HieuUngMua(2000, 2); // Hiển thị trong 2000ms, mật độ 2
  //delay(500);
}
