#include "MatrixAnimation.h"

void hienthi(byte leftMatrix[], byte rightMatrix[]) {
  for (int row = 0; row < 5; row++) {
    // Tắt tất cả các hàng
    for (int i = 0; i < 5; i++) digitalWrite(rowPins[i], LOW);

    // Gửi dữ liệu cho 10 cột (2 ma trận 5x5)
    digitalWrite(ST_PIN, LOW);
    // Gửi 5 bit cho ma trận bên phải (U2, cột 6-10)
    shiftOut(DS_PIN, SH_PIN, MSBFIRST, rightMatrix[row]);
    // Gửi 5 bit cho ma trận bên trái (U1, cột 1-5)
    shiftOut(DS_PIN, SH_PIN, MSBFIRST, leftMatrix[row]);
    digitalWrite(ST_PIN, HIGH);

    // Bật hàng hiện tại
    digitalWrite(rowPins[row], HIGH);

    // Đợi để hiển thị
    delayMicroseconds(500);
  }
}

/*
 * Chuyển đổi chuỗi thành chữ in hoa
 * text Chuỗi đầu vào cần chuyển đổi
 */
String InHoa(String text) {
  // Tạo bản sao để không thay đổi chuỗi gốc
  String result = text;
  
  // Chuyển đổi từng ký tự thành chữ in hoa
  for (int i = 0; i < result.length(); i++) {
    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] = result[i] - 32;
    }
  }
  
  return result;
}

/*
 * Tạo bộ đệm chứa dữ liệu hiển thị của các ký tự
 * text : Chuỗi ký tự cần hiển thị (đã chuyển thành chữ in hoa)
 * fullBuffer : Mảng lưu trữ dữ liệu hiển thị của toàn bộ chuỗi
 */
int TaoTextBuffer(String text, byte fullBuffer[][5]) {
  int bufferIndex = 0;
  int textLen = text.length();
  
  // Lặp qua từng ký tự trong chuỗi
  for (int charIndex = 0; charIndex < textLen; charIndex++) {
    char c = text.charAt(charIndex);  // trả vè các vị trí các chữ có trong str
    // ánh xạ vào mảng chữ 
    int letterIndex = (c >= 'A' && c <= 'Z') ? (c - 'A') : -1; // kiểm tra xem có phải chữ in hoa không 
    
    // Thêm dữ liệu ký tự vào bộ đệm
    if (letterIndex >= 0 && letterIndex < 26) {
      for (int row = 0; row < 5; row++) {
        // bộ đệm hiển thị để lưu trữ toàn bộ chuỗi dữ liệu pixel cho hoạt ảnh cuộn
        fullBuffer[bufferIndex + row][0] = chu[letterIndex][row];
      }
    }
    bufferIndex += 5;
    
    // Thêm khoảng trắng giữa các ký tự (trừ ký tự cuối cùng)
    if (charIndex < textLen - 1) {
      for (int row = 0; row < 2; row++) {
        fullBuffer[bufferIndex + row][0] = 0x00;
      }
      bufferIndex += 2;
    }
  }
  
  // Tính tổng số hàng cần thiết
  return textLen * (5 + 2) - 2; // Trừ 2 để loại bỏ khoảng trắng ở kí chữ cuối
}

/*
 * Hiển thị một khung hình của chữ chạy
 * fullBuffer : Bộ đệm chứa dữ liệu hiển thị toàn bộ chuỗi
 * startRow : Vị trí bắt đầu hiển thị trong bộ đệm
 * totalRows : Tổng số hàng của toàn bộ dữ liệu
 */
void HienThiFrameDoc(byte fullBuffer[][5], int startRow, int totalRows) {
  byte leftMatrix[5] = {0};   // led matrix 1 
  byte rightMatrix[5] = {0};  // led matrix 2
  
  // Điền dữ liệu cho ma trận bên trái (1)
  for (int row = 0; row < 5; row++) {
    if (startRow + row < totalRows) {
      leftMatrix[row] = fullBuffer[startRow + row][0];
    } else {
      leftMatrix[row] = 0x00;
    }
  }
  
  // Điền dữ liệu cho ma trận bên phải (5 hàng tiếp theo) (2)
  for (int row = 0; row < 5; row++) {
    if (startRow + row + 5 < totalRows) {
      rightMatrix[row] = fullBuffer[startRow + row + 5][0];
    } else {
      rightMatrix[row] = 0x00;
    }
  }
  
  // Hiển thị khung hình hiện tại trong 100ms
  unsigned long startTime = millis();
  while (millis() - startTime < 500) {
    hienthi(leftMatrix, rightMatrix);
  }
}

/*
 * Chữ chạy dọc từ dưới lên trên
 */
void scrollStrDoc(String text) {
  // Kiểm tra nếu chuỗi rỗng
  if (text.length() == 0) return;

  // Chuyển chuỗi thành chữ in hoa
  String uppercaseText = InHoa(text);
  
  // Tạo bộ đệm cho toàn bộ chuỗi
  byte fullBuffer[100][5] = {0}; // Giới hạn kích thước hợp lý - điều chỉnh nếu cần
  int totalRows = TaoTextBuffer(uppercaseText, fullBuffer);
  
  // Chạy dọc văn bản qua màn hình
  for (int startRow = 0; startRow <= totalRows; startRow++) {
    HienThiFrameDoc(fullBuffer, startRow, totalRows);
  }
}

/**
 * Chuyển đổi văn bản thành dữ liệu hiển thị cho ma trận LED
 * @param text Chuỗi văn bản cần hiển thị
 * @param displayData Mảng 2 chiều để lưu dữ liệu hiển thị [hàng][cột]
 * @return Tổng số cột được sử dụng trong displayData
 */
int TaoBuffer(String text, byte displayData[][200]) {
  // Chuyển văn bản thành chữ in hoa
  String uppercaseText = InHoa(text);
  
  // Đếm vị trí cột hiện tại trong buffer
  int bufferCol = 0;
  
  // Xử lý từng ký tự trong chuỗi
  for (int charPos = 0; charPos < uppercaseText.length(); charPos++) {
    char c = uppercaseText.charAt(charPos);
    int letterIndex = (c >= 'A' && c <= 'Z') ? (c - 'A') : -1;
    
    if (letterIndex >= 0) {
      // Xử lý từng hàng của ký tự
      for (int row = 0; row < 5; row++) {
        // Lấy mẫu bit cho hàng hiện tại
        byte pattern = chu[letterIndex][row];
        
        // Chuyển đổi mẫu byte thành các bit riêng lẻ
        for (int bit = 0; bit < 5; bit++) {
          // Sử dụng định dạng MSB (bit 4 là trái nhất, bit 0 là phải nhất)
          displayData[row][bufferCol + bit] = (pattern >> (4 - bit)) & 0x01;
        }
      }
      
      // Di chuyển đến vị trí tiếp theo sau 5 cột cho ký tự hiện tại
      bufferCol += 5;
      
      // Thêm một cột trống giữa các ký tự (trừ sau ký tự cuối cùng)
      if (charPos < uppercaseText.length() - 1) {
        bufferCol += 1;
      }
    }
  }
  
  // Trả về tổng số cột đã sử dụng
  return bufferCol;
}

/**
 * Cập nhật dữ liệu cho ma trận LED trái và phải dựa trên vị trí cuộn
 * @param displayData Mảng 2 chiều chứa dữ liệu hiển thị [hàng][cột]
 * @param shift Vị trí cuộn hiện tại
 * @param totalCols Tổng số cột trong dữ liệu hiển thị
 * @param leftMatrix Mảng đầu ra cho ma trận trái
 * @param rightMatrix Mảng đầu ra cho ma trận phải
 */
void HienThiFrameNgang(byte displayData[][200], int shift, int totalCols, byte leftMatrix[], byte rightMatrix[]) {
  // Xóa cả hai ma trận
  for (int row = 0; row < 5; row++) {
    leftMatrix[row] = 0;
    rightMatrix[row] = 0;
  }
  
  // Tạo dữ liệu cho cả hai ma trận
  for (int row = 0; row < 5; row++) {
    // Xử lý ma trận phải (hiển thị trước)
    for (int col = 0; col < 5; col++) {
      int dataCol = shift + col;
      if (dataCol >= 0 && dataCol < totalCols) {
        if (displayData[row][dataCol]) {
          rightMatrix[row] |= (1 << (4 - col));  // Định dạng MSB
        }
      }
    }
    
    // Xử lý ma trận trái (theo sau ma trận phải)
    for (int col = 0; col < 5; col++) {
      int dataCol = shift + col + 5;  // Độ lệch 5 cột so với ma trận phải
      if (dataCol >= 0 && dataCol < totalCols) {
        if (displayData[row][dataCol]) {
          leftMatrix[row] |= (1 << (4 - col));  // Định dạng MSB
        }
      }
    }
  }
}

/**
 * Hiển thị một khung hình trong thời gian xác định
 * @param leftMatrix Dữ liệu cho ma trận trái
 * @param rightMatrix Dữ liệu cho ma trận phải
 * @param duration Thời gian hiển thị tính bằng mili giây
 */
void TocDoFrameNgang(byte leftMatrix[], byte rightMatrix[], int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    hienthi(leftMatrix, rightMatrix);
  }
}

/**
 * Cuộn văn bản theo chiều ngang từ phải sang trái trên các ma trận LED
 * @param text Chuỗi văn bản cần hiển thị
 */
void scrollStrNgang(String text) {
  // Kiểm tra nếu chuỗi rỗng
  if (text.length() == 0) return;
  
  // Tạo bộ đệm cho dữ liệu hiển thị (giới hạn 200 cột để an toàn)
  byte displayData[5][200] = {0};
  
  // Chuẩn bị dữ liệu cho văn bản
  int totalCols = TaoBuffer(text, displayData);
  
  // Cuộn từ phải sang trái
  // Thêm 10 khung hình ở đầu và 5 ở cuối để đảm bảo văn bản cuộn hoàn toàn qua màn hình
  for (int shift = -10; shift < totalCols + 5; shift++) {
    byte leftMatrix[5] = {0};
    byte rightMatrix[5] = {0};
    
    // Cập nhật dữ liệu cho ma trận LED
    HienThiFrameNgang(displayData, shift, totalCols, leftMatrix, rightMatrix);
    
    // Hiển thị khung hình hiện tại trong một thời gian nhất định
    TocDoFrameNgang(leftMatrix, rightMatrix, 100);  // 100ms mỗi khung hình
  }
}

// Hàm tạo hiệu ứng xoáy vào trong
void LedXoay(int cycles, int speed) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Xóa cả hai ma trận
    for (int i = 0; i < 5; i++) {
      leftMatrix[i] = 0;
      rightMatrix[i] = 0;
    }
    
    // Định nghĩa mẫu xoáy - mỗi bước là một vị trí trong đường xoáy
    // Định dạng: {hàng, cột, ma trận} trong đó ma trận là 0 cho bên trái, 1 cho bên phải
    int spiralPath[][3] = {
      // Hàng trên cùng (trái sang phải)
      {0, 0, 0}, {0, 1, 0}, {0, 2, 0}, {0, 3, 0}, {0, 4, 0},
      {0, 0, 1}, {0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 4, 1},
      
      // Cột phải (trên xuống dưới)
      {1, 4, 0}, {2, 4, 0}, {3, 4, 0}, {4, 4, 0},
      {1, 4, 1}, {2, 4, 1}, {3, 4, 1}, {4, 4, 1},
      
      // Hàng dưới cùng (phải sang trái)
      {4, 3, 0}, {4, 2, 0}, {4, 1, 0}, {4, 0, 0},
      {4, 3, 1}, {4, 2, 1}, {4, 1, 1}, {4, 0, 1},
      
      // Cột trái (dưới lên trên)
      {3, 0, 0}, {2, 0, 0}, {1, 0, 0},
      {3, 0, 1}, {2, 0, 1}, {1, 0, 1},
      
      // Lớp thứ hai: hàng trên
      {1, 1, 0}, {1, 2, 0}, {1, 3, 0},
      {1, 1, 1}, {1, 2, 1}, {1, 3, 1},
      
      // Lớp thứ hai: cột phải
      {2, 3, 0}, {3, 3, 0},
      {2, 3, 1}, {3, 3, 1},
      
      // Lớp thứ hai: hàng dưới
      {3, 2, 0}, {3, 1, 0},
      {3, 2, 1}, {3, 1, 1},
      
      // Lớp thứ hai: cột trái
      {2, 1, 0},
      {2, 1, 1},
      
      // Điểm trung tâm
      {2, 2, 0},
      {2, 2, 1}
    };
    
    // Tạo hoạt ảnh xoáy bằng cách bật từng đèn LED một
    for (int step = 0; step < sizeof(spiralPath) / sizeof(spiralPath[0]); step++) {
      int row = spiralPath[step][0];
      int col = spiralPath[step][1];
      int matrix = spiralPath[step][2];
      
      // Bật LED hiện tại
      if (matrix == 0) {
        leftMatrix[row] |= (0x10 >> col);  // Đặt bit tương ứng với cột này
      } else {
        rightMatrix[row] |= (0x10 >> col);  // Đặt bit tương ứng với cột này
      }
      
      // Hiển thị trạng thái hiện tại trong một khoảng thời gian ngắn
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
    
    // Giữ nguyên hình xoáy hoàn chỉnh trong một khoảng thời gian
    unsigned long startTime = millis();
    while (millis() - startTime < speed * 5) {
      hienthi(leftMatrix, rightMatrix);
    }
    
    // Bây giờ tạo hoạt ảnh xoáy ra ngoài (đảo ngược mẫu)
    for (int step = sizeof(spiralPath) / sizeof(spiralPath[0]) - 1; step >= 0; step--) {
      int row = spiralPath[step][0];
      int col = spiralPath[step][1];
      int matrix = spiralPath[step][2];
      
      // Tắt LED hiện tại
      if (matrix == 0) {
        leftMatrix[row] &= ~(0x10 >> col);  // Xóa bit tương ứng với cột này
      } else {
        rightMatrix[row] &= ~(0x10 >> col);  // Xóa bit tương ứng với cột này
      }
      
      // Hiển thị trạng thái hiện tại trong một khoảng thời gian ngắn
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
  }
}

// Hàm tạo hiệu ứng nháy ngẫu nhiên
void randomFlicker(int duration, int flickerRate) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  unsigned long startTime = millis();
  
  // Chạy hiệu ứng trong thời gian duration (ms)
  while (millis() - startTime < duration) {
    // Tạo giá trị ngẫu nhiên cho mỗi hàng của cả hai ma trận
    for (int row = 0; row < 5; row++) {
      // Thay đổi ngẫu nhiên các điểm sáng trên mỗi ma trận
      if (random(100) < flickerRate) {
        leftMatrix[row] = random(32);  // Tạo giá trị ngẫu nhiên từ 0-31 (5 bit)
      }
      
      if (random(100) < flickerRate) {
        rightMatrix[row] = random(32); // Tạo giá trị ngẫu nhiên từ 0-31 (5 bit)
      }
    }
    
    // Hiển thị trạng thái hiện tại
    for (int i = 0; i < 5; i++) {
      hienthi(leftMatrix, rightMatrix);
    }
    
    // Đợi một khoảng thời gian ngắn
    delay(50);
  }
}

// Hàm tạo hiệu ứng nháy ngẫu nhiên theo điểm ảnh
void randomPixelFlicker(int duration, int density) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  unsigned long startTime = millis();
  
  // Chạy hiệu ứng trong thời gian duration (ms)
  while (millis() - startTime < duration) {
    // Xóa cả hai ma trận
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    // Thêm ngẫu nhiên các LED sáng
    for (int i = 0; i < density; i++) {
      // Chọn ngẫu nhiên ma trận (trái hoặc phải)
      int matrix = random(2);
      
      // Chọn vị trí ngẫu nhiên (hàng và cột)
      int row = random(5);
      int col = random(5);
      
      // Bật LED tại vị trí đó
      if (matrix == 0) {
        leftMatrix[row] |= (0x10 >> col);
      } else {
        rightMatrix[row] |= (0x10 >> col);
      }
    }
    
    // Hiển thị trạng thái hiện tại
    unsigned long displayTime = millis();
    while (millis() - displayTime < 100) {
      hienthi(leftMatrix, rightMatrix);
    }
  }
}

/**
 * Hiệu ứng giọt nước lan tỏa
 * @param cycles Số lần lặp lại hiệu ứng
 * @param speed Tốc độ hiển thị (ms)
 */
void HieuUngGiotNuoc(int cycles, int speed) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Xóa cả hai ma trận
    for (int i = 0; i < 5; i++) {
      leftMatrix[i] = 0;
      rightMatrix[i] = 0;
    }
    
    // Hiển thị lần lượt các khung hình lan tỏa
    for (int frame = 0; frame < 5; frame++) {
      // Điền dữ liệu vào cả hai ma trận
      for (int row = 0; row < 5; row++) {
        leftMatrix[row] = frame_Giot_nuoc[frame][row];
        rightMatrix[row] = frame_Giot_nuoc[frame][row];
      }
      
      // Hiển thị khung hình hiện tại
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
    
    // Tạm dừng trước khi bắt đầu chu kỳ mới
    delay(speed * 2);
  }
}

void HieuUngMua(int duration, int density) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  unsigned long startTime = millis();
  
  // Mảng giọt mưa cho mỗi cột
  int rainDrops[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  
  // Chạy hiệu ứng trong thời gian duration (ms)
  while (millis() - startTime < duration) {
    // Xóa cả hai ma trận
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    // Tạo giọt mưa mới với xác suất dựa trên mật độ
    for (int col = 0; col < 10; col++) {
      // Nếu không có giọt mưa trong cột này
      if (rainDrops[col] == -1) {
        // Tạo giọt mưa mới với xác suất phụ thuộc vào mật độ
        if (random(100) < density * 5) {
          rainDrops[col] = 0;  // Bắt đầu từ hàng đầu tiên
        }
      } 
      // Di chuyển giọt mưa xuống
      else {
        rainDrops[col]++;
        // Nếu giọt mưa đã ra khỏi màn hình
        if (rainDrops[col] >= 5) {
          rainDrops[col] = -1;  // Xóa giọt mưa
        }
      }
      
      // Vẽ các giọt mưa
      if (rainDrops[col] >= 0 && rainDrops[col] < 5) {
        if (col < 5) {
          // Ma trận phải (cột 0-4)
          rightMatrix[rainDrops[col]] |= (0x10 >> col);
        } else {
          // Ma trận trái (cột 5-9)
          leftMatrix[rainDrops[col]] |= (0x10 >> (col - 5));
        }
      }
    }
    
    // Hiển thị trạng thái hiện tại
    unsigned long frameTime = millis();
    while (millis() - frameTime < 100) {
      hienthi(leftMatrix, rightMatrix);
    }
  }
}

/**
 * Hiệu ứng sáng từng LED theo từng hàng
 * Sáng hết 1 hàng rồi chạy xuống hàng tiếp theo
 * @param cycles Số lần lặp lại hiệu ứng
 * @param rowSpeed Tốc độ chuyển giữa các LED (ms)
 * @param rowDelay Thời gian dừng sau khi một hàng sáng đầy đủ (ms)
 */
void SangTheoHang(int cycles, int rowSpeed, int rowDelay) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Xóa tất cả các ma trận
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    // Duyệt qua từng hàng
    for (int row = 0; row < 5; row++) {
      // Duyệt qua từng LED trong hàng phải trước (thứ tự từ trái sang phải)
      for (int col = 0; col < 5; col++) {
        // Bật LED hiện tại cho ma trận phải
        rightMatrix[row] |= (0x10 >> col);
        
        // Hiển thị trạng thái hiện tại
        unsigned long startTime = millis();
        while (millis() - startTime < rowSpeed) {
          hienthi(leftMatrix, rightMatrix);
        }
      }
      
      // Duyệt qua từng LED trong hàng trái (thứ tự từ trái sang phải)
      for (int col = 0; col < 5; col++) {
        // Bật LED hiện tại cho ma trận trái
        leftMatrix[row] |= (0x10 >> col);
        
        // Hiển thị trạng thái hiện tại
        unsigned long startTime = millis();
        while (millis() - startTime < rowSpeed) {
          hienthi(leftMatrix, rightMatrix);
        }
      }
      
      // Dừng một chút khi hàng đã sáng đủ
      unsigned long pauseTime = millis();
      while (millis() - pauseTime < rowDelay) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
    
    // Hiển thị toàn bộ ma trận sáng một lúc trước khi bắt đầu chu kỳ mới
    unsigned long endCycleTime = millis();
    while (millis() - endCycleTime < rowDelay * 2) {
      hienthi(leftMatrix, rightMatrix);
    }
    
    // Xóa ma trận và tạm dừng trước khi bắt đầu chu kỳ mới
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    unsigned long betweenCycleTime = millis();
    while (millis() - betweenCycleTime < rowDelay) {
      hienthi(leftMatrix, rightMatrix);
    }
  }
}


void SangSoLe(int cycles, int speed) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  // Định nghĩa mẫu bàn cờ chẵn lẻ
  byte checkerPattern1 = 0b10101;  // Mẫu 1: xen kẽ bắt đầu với LED sáng
  byte checkerPattern2 = 0b01010;  // Mẫu 2: xen kẽ bắt đầu với LED tắt
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Phase 1: Hiển thị bàn cờ từ trên xuống dưới
    
    // Xóa tất cả các ma trận
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    // Hiện từng hàng của bàn cờ từ trên xuống dưới
    for (int row = 0; row < 5; row++) {
      // Xác định mẫu dựa trên hàng chẵn hoặc lẻ
      byte pattern = (row % 2 == 0) ? checkerPattern1 : checkerPattern2;
      
      // Hiện mẫu ở hàng hiện tại
      rightMatrix[row] = pattern;
      leftMatrix[row] = pattern;
      
      // Hiển thị trạng thái hiện tại một lúc
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
    
    // Tạm dừng để xem toàn bộ bàn cờ
    unsigned long pauseTime = millis();
    while (millis() - pauseTime < speed * 2) {
      hienthi(leftMatrix, rightMatrix);
    }
    
    // Phase 2: Hiển thị bàn cờ đảo ngược từ trên xuống dưới
    
    // Xóa tất cả các ma trận
    for (int row = 0; row < 5; row++) {
      leftMatrix[row] = 0;
      rightMatrix[row] = 0;
    }
    
    // Hiện từng hàng của bàn cờ đảo ngược từ trên xuống dưới
    for (int row = 0; row < 5; row++) {
      // Xác định mẫu đảo ngược dựa trên hàng chẵn hoặc lẻ
      byte pattern = (row % 2 == 0) ? checkerPattern2 : checkerPattern1;
      
      // Hiện mẫu ở hàng hiện tại
      rightMatrix[row] = pattern;
      leftMatrix[row] = pattern;
      
      // Hiển thị trạng thái hiện tại một lúc
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
    
    // Tạm dừng để xem toàn bộ bàn cờ đảo ngược
    pauseTime = millis();
    while (millis() - pauseTime < speed * 2) {
      hienthi(leftMatrix, rightMatrix);
    }
  }
}
