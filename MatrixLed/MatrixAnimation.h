#ifndef MATRIX_ANIMATION_H
#define MATRIX_ANIMATION_H

#include <Arduino.h>

// frane của các chữ cái 
const byte chu[][5] = {
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

// frame của animation giọt nước 
const byte frame_Giot_nuoc[][5] = {
    {0x00,0x00,0x04,0x00,0x00},
    {0x00,0x04,0x0a,0x04,0x00},
    {0x04,0x0a,0x11,0x0a,0x04},
    {0x0a,0x11,0x00,0x11,0x0a},
    {0x11,0x00,0x00,0x00,0x11}
};

// frame của sóng
const byte song_frame[][5] = {
    {0x02,0x07,0x0d,0x18,0x10},
    {0x01,0x03,0x16,0x1c,0x08},
    {0x00,0x11,0x1b,0x0e,0x04},
    {0x10,0x18,0x0c,0x06,0x03}
};

//frame animation trái tim
const byte heart_frame[][5] = {
    {0x0a,0x15,0x11,0x0a,0x04},
    {0x0a,0x1f,0x1f,0x0e,0x04}
};

//frame animation pacman
const byte pacman_frame[][5] = {
    {0x0e,0x1f,0x18,0x1f,0x0e},
    {0x0e,0x1c,0x18,0x1c,0x0e}
};



/*
 * hiện thị frame ra led ma trận
 - leftMatrix ma trận trái 
 - rightMatrix ma trận bên phải
*/
void hienthi(byte leftMatrix[], byte rightMatrix[]);

//****** 1. các hàm của cho chữ chạy dọc ******
/*
 * Chuyển đổi chuỗi thành chữ in hoa
 - text Chuỗi đầu vào cần chuyển đổi
*/
String InHoa(String text);
/*
 - Tạo bộ đệm chứa dữ liệu hiển thị của các ký tự
 - text : Chuỗi ký tự cần hiển thị (đã chuyển thành chữ in hoa)
 - fullBuffer : Mảng lưu trữ dữ liệu hiển thị của toàn bộ chuỗi
*/
int TaoTextBuffer(String text, byte fullBuffer[][5]);
/*
 * Hiển thị một khung hình của chữ chạy
 - fullBuffer : Bộ đệm chứa dữ liệu hiển thị toàn bộ chuỗi
 - startRow : Vị trí bắt đầu hiển thị trong bộ đệm
 - totalRows : Tổng số hàng của toàn bộ dữ liệu
*/
void HienThiFrameDoc(byte fullBuffer[][5], int startRow, int totalRows);
/*
 * Cuộn văn bản theo chiều ngang từ phải sang trái trên các ma trận LED
 - text Chuỗi văn bản cần hiển thị
*/
void scrollStrDoc(String text);

//****** 2. các hàm của cho chữ chạy ngang ******
/*
 * Chuyển đổi văn bản thành dữ liệu hiển thị cho ma trận LED
 - text Chuỗi văn bản cần hiển thị
 - displayData Mảng 2 chiều để lưu dữ liệu hiển thị [hàng][cột]
 - Tổng số cột được sử dụng trong displayData
*/
int TaoBuffer(String text, byte displayData[][200]);
/*
 * Cập nhật dữ liệu cho ma trận LED trái và phải dựa trên vị trí cuộn
 - displayData Mảng 2 chiều chứa dữ liệu hiển thị [hàng][cột]
 - shift Vị trí cuộn hiện tại
 - totalCols Tổng số cột trong dữ liệu hiển thị
 - leftMatrix Mảng đầu ra cho ma trận trái
 - rightMatrix Mảng đầu ra cho ma trận phải
*/
void HienThiFrameNgang(byte displayData[][200], int shift, int totalCols, byte leftMatrix[], byte rightMatrix[]);
/*
 * Hiển thị một khung hình trong thời gian xác định
 - leftMatrix Dữ liệu cho ma trận trái
 - rightMatrix Dữ liệu cho ma trận phải
 - duration Thời gian hiển thị tính bằng mili giây
*/
void TocDoFrameNgang(byte leftMatrix[], byte rightMatrix[], int duration);
/*
 * Cuộn văn bản theo chiều ngang từ phải sang trái trên các ma trận LED
 - text Chuỗi văn bản cần hiển thị
*/
void scrollStrNgang(String text);

//****** 3. các hàm của animation xoay ****** 
/*
 * Tạo hiệu ứng các LED sáng tuần tự từ ngoài vào trong theo hình xoắn ốc, sau đó tắt từ trong ra ngoài
 - cycles Số lần lặp lại hiệu ứng
 - speed Tốc độ hiển thị (ms), giá trị càng nhỏ tốc độ càng nhanh
*/
void LedXoay(int cycles, int speed);

//****** 4. các hàm của nháy ngẫu nhiên mà đến sáng theo tỉ lệ ******
/*
 * Tạo hiệu ứng các hàng LED thay đổi ngẫu nhiên theo xác suất
 - duration Thời gian chạy hiệu ứng (ms)
 - flickerRate tỉ lệ nhấp nháy (0-100)
*/
void randomFlicker(int duration, int flickerRate);

//****** 5. các hàm của điểm nháy ngẫu nhiên ******
/*
 * Tạo hiệu ứng lấp lánh với các điểm sáng xuất hiện và biến mất ngẫu nhiên 
 - duration Thời gian chạy hiệu ứng (ms)
 - density Mật độ điểm sáng (1-20), giá trị càng cao càng nhiều điểm sáng
*/
void randomPixelFlicker(int duration, int density);

//****** 6. các hàm của hiệu ứng giọt nước ******
/*
 * Hiệu ứng giọt nước lan tỏa
 - cycles Số lần lặp lại hiệu ứng
 - speed Tốc độ hiển thị (ms)
*/
void HieuUngGiotNuoc(int cycles, int speed);

//****** 7. các hàm của hiệu ứng mưa rơi ******
/*
 * Các giọt mưa xuất hiện ngẫu nhiên từ trên cao và rơi xuống dưới
 - duration Thời gian chạy hiệu ứng (ms)
 - density Mật độ giọt mưa (1-10), giá trị càng cao mưa càng dày
*/
void HieuUngMua(int duration, int density);

//****** 8. các hàm của hiệu ứng sáng theo hàng ******
/*
 - Hiệu ứng sáng từng LED theo từng hàng
 - Sáng hết 1 hàng rồi chạy xuống hàng tiếp theo
 - cycles Số lần lặp lại hiệu ứng
 - rowSpeed Tốc độ chuyển giữa các LED (ms)
 - rowDelay Thời gian dừng sau khi một hàng sáng đầy đủ (ms)
*/
void SangTheoHang(int cycles, int rowSpeed, int rowDelay);


//****** 9. các hàm của hiệu ứng sáng xen kẽ ******
/*
 * Hàm tạo hiệu ứng bàn cờ xen kẽ trên hai LED matrix
 - cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 - speed Tốc độ hiển thị (thời gian mỗi bước tính bằng mili giây)
*/
void LedXenKe(int cycles, int speed);

//****** 10. các hàm của animation sóng ******
/*
 * Tạo bộ đệm dữ liệu sóng từ các frame hoạt ảnh
 - waveData Mảng 2 chiều để lưu trữ dữ liệu sóng
 - return Tổng số cột trong bộ đệm sóng
*/
int TaoWaveBuffer(byte waveData[][50]);
/*
 * Cập nhật một hàng đơn của matrix với dữ liệu từ bộ đệm sóng
 - waveData Bộ đệm nguồn chứa các frame kết hợp
 - matrixData Bộ đệm matrix đầu ra cần cập nhật
 - row Hàng hiện tại đang được cập nhật
 - step Bước hoạt ảnh hiện tại
 - totalCols Tổng số cột trong bộ đệm sóng
 - rightToLeft Hướng di chuyển (true: phải sang trái, false: trái sang phải)
 - offset Độ lệch cột (0 cho matrix phải, 5 cho matrix trái)
*/
void UpdateData(byte waveData[][50], byte matrixData[], int row, int step, int totalCols, bool rightToLeft, int offset);
/*
 * Cập nhật dữ liệu LED matrix dựa trên bước hoạt ảnh hiện tại
 - waveData Bộ đệm nguồn chứa các frame kết hợp
 - leftMatrix Bộ đệm đầu ra cho LED matrix trái
 - rightMatrix Bộ đệm đầu ra cho LED matrix phải
 - step Bước hoạt ảnh hiện tại (vị trí)
 - totalCols Tổng số cột trong bộ đệm sóng
 - rightToLeft Hướng di chuyển (true: phải sang trái, false: trái sang phải)
*/
void UpdateFrame(byte waveData[][50], byte leftMatrix[], byte rightMatrix[], int step, int totalCols, bool rightToLeft);
/*
 - Hiển thị frame hoạt ảnh hiện tại trong một khoảng thời gian xác định
 - leftMatrix Dữ liệu LED matrix trái
 - rightMatrix Dữ liệu LED matrix phải
 - duration Thời gian hiển thị tính bằng mili giây
*/
void TocDoFrame(byte leftMatrix[], byte rightMatrix[], int duration);
/*
 * Hàm chính để tạo hiệu ứng sóng bằng cách cuộn các frame kết hợp theo chiều ngang
 - cycles Số chu kỳ hoàn chỉnh để chạy hoạt ảnh
 - speed Tốc độ di chuyển (mili giây mỗi frame)
 - rightToLeft Hướng di chuyển (true = phải sang trái, false = trái sang phải)
*/
void LedSong(int cycles, int speed, bool rightToLeft = true);

//****** 11. các hàm của animation pacman ******
/*
 * Hiệu ứng hình Pacman nhấp nháy
 - cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 - speed Tốc độ chuyển đổi giữa các khung hình (ms)
*/
void pacman(int cycles, int speed);

//****** 12. các hàm của animation trái tim ******
/*
 * Hiệu ứng hình trái tim đập
 - cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 - speed Tốc độ chuyển đổi giữa các khung hình (ms)
*/
void trai_tim(int cycles, int speed);

// Hàm chọn hiệu ứng LED  
void ChonAnimation(int number);

#endif // MATRIX_ANIMATION_H
