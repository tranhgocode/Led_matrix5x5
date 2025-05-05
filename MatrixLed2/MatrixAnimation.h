#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include "Arduino.h"

class LEDMatrix {
  public:
    // Constructor
    LEDMatrix(uint8_t sh_pin, uint8_t ds_pin, uint8_t st_pin, uint8_t row_pins[]);
    
    // Phương thức khởi tạo
    void begin();
    
    // Phương thức hiển thị cơ bản
    void hienthi(byte leftMatrix[], byte rightMatrix[]);
    
    // Các hàm hiển thị chữ
    void scrollStrDoc(String text);
    void scrollStrNgang(String text);
    
    // Các hiệu ứng hoạt ảnh
    void LedXoay(int cycles, int speed);
    void randomFlicker(int duration, int flickerRate);
    void randomPixelFlicker(int duration, int density);
    void HieuUngGiotNuoc(int cycles, int speed);
    void HieuUngMua(int duration, int density);
    void LEDTheoHang(int cycles, int rowSpeed, int rowDelay);
    void LedXenKe(int cycles, int speed);
    void LedSong(int cycles, int speed, bool rightToLeft = true);
    void pacman(int cycles, int speed);
    void trai_tim(int cycles, int speed);
    
    // Hàm chọn hiệu ứng theo số
    void ChonAnimation(int number);
    
  private:
    // Các chân kết nối
    uint8_t _sh_pin;
    uint8_t _ds_pin;
    uint8_t _st_pin;
    uint8_t _row_pins[5];
    
    // Định nghĩa font chữ
    static const byte chu[][5];
    
    // Định nghĩa các mảng khung hình cho hiệu ứng
    static const byte heart_frame[][5];
    static const byte pacman_frame[][5];
    static const byte frame_Giot_nuoc[][5];
    
    // Hàm hỗ trợ cho hiển thị chữ dọc 
    String InHoa(String text);
    int TaoTextBuffer(String text, byte fullBuffer[][5]);
    void HienThiFrameDoc(byte fullBuffer[][5], int startRow, int totalRows);

    // hàm hỗ trợ cho hiện thị chữ ngang (sang bên folder MatrixLed để hiểu hiểu tên biến)
    int TaoBuffer(String text, byte displayData[][200]);
    void HienThiFrameNgang(byte displayData[][200], int shift, int totalCols, byte leftMatrix[], byte rightMatrix[]);
    void TocDoFrameNgang(byte leftMatrix[], byte rightMatrix[], int duration);

    // hàm hỗ trợ cho hiển thị animation sóng 
    int TaoWaveBuffer(byte waveData[][50]);
    void UpdateData(byte waveData[][50], byte matrixData[], int row, int step, int totalCols, bool rightToLeft, int offset);
    void UpdateFrame(byte waveData[][50], byte leftMatrix[], byte rightMatrix[], int step, int totalCols, bool rightToLeft);
    void TocDoFrame(byte leftMatrix[], byte rightMatrix[], int duration);
};

#endif // MATRIX_ANIMATION_H
