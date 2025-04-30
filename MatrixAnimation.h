#ifndef MatrixAnimation_h
#define MatrixAnimation_h

#include "Arduino.h"

    void begin();
    /**
     * Hiển thị dữ liệu lên ma trận LED
     * @param leftMatrix Mảng 5 byte chứa dữ liệu cho ma trận bên trái
     * @param rightMatrix Mảng 5 byte chứa dữ liệu cho ma trận bên phải
     */
    void hienthi(byte leftMatrix[], byte rightMatrix[]);
    
    /**
     * Cuộn văn bản theo chiều ngang từ phải sang trái
     * @param text Chuỗi văn bản cần hiển thị
     * @param speedMs Tốc độ cuộn (ms mỗi khung hình)
     */
    void scrollStrNgang(String text, int speedMs = 100);
    
    /**
     * Cuộn văn bản theo chiều dọc từ dưới lên trên
     * @param text Chuỗi văn bản cần hiển thị
     * @param speedMs Tốc độ cuộn (ms mỗi khung hình)
     */
    void scrollStrDoc(String text, int speedMs = 500);
    
    /**
     * Tạo hiệu ứng xoay theo hình xoắn ốc
     * @param cycles Số chu kỳ xoay
     * @param speed Tốc độ xoay (ms cho mỗi bước)
     */
    void LedXoay(int cycles, int speed);
    
    /**
     * Tạo hiệu ứng nháy ngẫu nhiên theo hàng
     * @param duration Thời gian hiệu ứng (ms)
     * @param flickerRate Tỉ lệ phần trăm nháy (0-100)
     */
    void randomFlicker(int duration, int flickerRate);
    
    /**
     * Tạo hiệu ứng nháy ngẫu nhiên theo từng pixel
     * @param duration Thời gian hiệu ứng (ms)
     * @param density Số lượng pixel sáng trong mỗi khung hình
     */
    void randomPixelFlicker(int duration, int density);
    
    /**
     * Hiển thị một ký tự cụ thể trên ma trận
     * @param c Ký tự cần hiển thị (A-Z)
     * @param duration Thời gian hiển thị (ms)
     */
    void displayChar(char c, int duration = 1000);
    
    /**
     * Hiển thị một chuỗi ký tự (không cuộn)
     * @param text Chuỗi cần hiển thị (tối đa 2 ký tự)
     * @param duration Thời gian hiển thị (ms)
     */
    void displayText(String text, int duration = 1000);
    
    // Dữ liệu mẫu bit cho các ký tự A-Z
    const byte _chu[26][5] = {
      {0x04,0x0a,0x1f,0x11,0x11}, //A 
      {0x1c,0x12,0x1c,0x12,0x1c}, //B
      {0x0e,0x11,0x10,0x11,0x0e}, //C
      {0x1c,0x12,0x12,0x12,0x1c}, //D
      {0x1e,0x10,0x1e,0x10,0x1e}, //E
      {0x1e,0x10,0x1e,0x10,0x10}, //F
      {0x0e,0x10,0x17,0x11,0x0e}, //G
      {0x11,0x11,0x1f,0x11,0x11}, //H
      {0x0e,0x04,0x04,0x04,0x0e}, //I
      {0x07,0x02,0x02,0x12,0x0c}, //J
      {0x12,0x14,0x18,0x14,0x12}, //K
      {0x10,0x10,0x10,0x10,0x1e}, //L
      {0x11,0x1b,0x15,0x11,0x11}, //M
      {0x11,0x19,0x15,0x13,0x11}, //N
      {0x1f,0x11,0x11,0x11,0x1f}, //O
      {0x1c,0x12,0x1c,0x10,0x10}, //P
      {0x0e,0x11,0x15,0x13,0x0f}, //Q
      {0x1c,0x12,0x1c,0x14,0x12}, //R
      {0x0e,0x10,0x0e,0x01,0x1e}, //S
      {0x1f,0x04,0x04,0x04,0x04}, //T
      {0x11,0x11,0x11,0x11,0x0e}, //U
      {0x11,0x11,0x11,0x0a,0x04}, //V
      {0x11,0x11,0x15,0x15,0x0A}, //W
      {0x11,0x0a,0x04,0x0a,0x11}, //X
      {0x11,0x0A,0x04,0x04,0x04}, //Y
      {0x1f,0x02,0x04,0x08,0x1f}, //Z
    };
    
    /**
     * Chuyển đổi chuỗi thành chữ in hoa
     * @param text Chuỗi đầu vào
     * @return Chuỗi đã chuyển đổi thành in hoa
     */
    String InHoa(String text);
    
    /**
     * Tạo bộ đệm cho văn bản cuộn dọc
     * @param text Chuỗi văn bản đầu vào
     * @param fullBuffer Mảng lưu trữ dữ liệu hiển thị của toàn bộ chuỗi
     * @return Tổng số hàng cần thiết
     */
    int createTextBuffer(String text, byte fullBuffer[][5]);
    
    /**
     * Hiển thị một khung hình của văn bản cuộn dọc
     * @param fullBuffer Bộ đệm chứa dữ liệu hiển thị
     * @param startRow Vị trí bắt đầu hiển thị trong bộ đệm
     * @param totalRows Tổng số hàng của toàn bộ dữ liệu
     * @param speedMs Thời gian hiển thị mỗi khung (ms)
     */
    void displayTextFrame(byte fullBuffer[][5], int startRow, int totalRows, int speedMs);
    
    /**
     * Chuyển đổi văn bản thành dữ liệu hiển thị cho ma trận LED
     * @param text Chuỗi văn bản cần hiển thị
     * @param displayData Mảng 2 chiều để lưu dữ liệu hiển thị [hàng][cột]
     * @return Tổng số cột được sử dụng trong displayData
     */
    int chuanBiDuLieuVanBan(String text, byte displayData[][200]);
    
    /**
     * Cập nhật dữ liệu cho ma trận LED trái và phải dựa trên vị trí cuộn
     * @param displayData Mảng 2 chiều chứa dữ liệu hiển thị [hàng][cột]
     * @param shift Vị trí cuộn hiện tại
     * @param totalCols Tổng số cột trong dữ liệu hiển thị
     * @param leftMatrix Mảng đầu ra cho ma trận trái
     * @param rightMatrix Mảng đầu ra cho ma trận phải
     */
    void capNhatDuLieuMaTran(byte displayData[][200], int shift, int totalCols, byte leftMatrix[], byte rightMatrix[]);
    
    /**
     * Hiển thị một khung hình trong thời gian xác định
     * @param leftMatrix Dữ liệu cho ma trận trái
     * @param rightMatrix Dữ liệu cho ma trận phải
     * @param duration Thời gian hiển thị tính bằng mili giây
     */
    void hienThiKhungHinh(byte leftMatrix[], byte rightMatrix[], int duration);

#endif
