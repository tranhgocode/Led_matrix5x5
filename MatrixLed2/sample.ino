#include <MatrixAnimation.h>

// Định nghĩa các chân
#define SH_PIN 13  // Shift Clock (SH)
#define DS_PIN 12  // Data Serial (DS)
#define ST_PIN 11  // Storage Clock (ST)

// Thiết lập các chân hàng
uint8_t rowPins[5] = {10, 9, 8, 7, 6};

// Khởi tạo đối tượng LEDMatrix
LEDMatrix matrix(SH_PIN, DS_PIN, ST_PIN, rowPins);


void setup() {
  matrix.begin();
  
  // Khởi tạo Serial để xem thông báo
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //matrix.trai_tim(5, 100);
  // animation trái tim
  matrix.ChonAnimation(10);

}
