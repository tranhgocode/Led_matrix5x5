#define SH_PIN 13  // Shift Clock (SH)
#define DS_PIN 12  // Data Serial (DS)
#define ST_PIN 11  // Storage Clock (ST)

// set_up chan
int rowPins[5] = {10, 9, 8, 7, 6};


byte chu[][5] = {
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

// các hàm của cho chữ chạy dọc
String InHoa(String text);
int TaoTextBuffer(String text, byte fullBuffer[][5]);
void HienThiFrameDoc(byte fullBuffer[][5], int startRow, int totalRows);
void scrollStrDoc(String text);


// các hàm của cho chữ chạy ngang 
int TaoBuffer(String text, byte displayData[][200]);
void HienThiFrameNgang(byte displayData[][200], int shift, int totalCols, byte leftMatrix[], byte rightMatrix[]);
void TocDoFrameNgang(byte leftMatrix[], byte rightMatrix[], int duration);
void scrollStrNgang(String text);

// các hàm của animation xoay 
void LedXoay(int cycles, int speed);

// các hàm của nháy ngẫu nhiên mà đến sáng theo tỉ lệ
void randomFlicker(int duration, int flickerRate);

// các hàm của điểm nháy ngẫu nhiên
void randomPixelFlicker(int duration, int density);


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
}



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


// /**
//  * Scrolls text horizontally from right to left across the LED matrices
//  * @param text The text string to be displayed
//  */
// void scrollStrNgang(String text) {
//   // Check if the string is empty
//   if (text.length() == 0) return;

//   // Convert the string to uppercase
//   String uppercaseText = InHoa(text);
  
//   // Create buffer for text display data (limit to 200 columns for safety)
//   byte displayData[5][200] = {0};
  
//   // Calculate total columns needed
//   int totalCols = 0;
  
//   // Fill display buffer with character data
//   int bufferCol = 0;
//   for (int charPos = 0; charPos < uppercaseText.length(); charPos++) {
//     char c = uppercaseText.charAt(charPos);
//     int letterIndex = (c >= 'A' && c <= 'Z') ? (c - 'A') : -1;
    
//     if (letterIndex >= 0) {
//       // Process each row of the character
//       for (int row = 0; row < 5; row++) {
//         // Get bit pattern for current row
//         byte pattern = chu[letterIndex][row];
        
//         // Convert byte pattern to individual bits
//         for (int bit = 0; bit < 5; bit++) {
//           // Use MSB format (bit 4 is leftmost, bit 0 is rightmost)
//           displayData[row][bufferCol + bit] = (pattern >> (4 - bit)) & 0x01;
//         }
//       }
      
//       // Move to next position after 5 columns for current character
//       bufferCol += 5;
      
//       // Add an empty column between characters (except after the last character)
//       if (charPos < uppercaseText.length() - 1) {
//         bufferCol += 1;
//       }
//     }
//   }
  
//   // Update total columns
//   totalCols = bufferCol;
  
//   // Scroll from right to left
//   // Add 10 extra frames to ensure text scrolls completely off screen
//   for (int shift = -10; shift < totalCols + 5; shift++) {
//     byte leftMatrix[5] = {0};
//     byte rightMatrix[5] = {0};
    
//     // Generate data for both matrices
//     for (int row = 0; row < 5; row++) {
//       // Process right matrix (visible first)
//       for (int col = 0; col < 5; col++) {
//         int dataCol = shift + col;
//         if (dataCol >= 0 && dataCol < totalCols) {
//           if (displayData[row][dataCol]) {
//             rightMatrix[row] |= (1 << (4 - col));  // MSB format
//           }
//         }
//       }
      
//       // Process left matrix (follows after right matrix)
//       for (int col = 0; col < 5; col++) {
//         int dataCol = shift + col + 5;  // 5 columns offset from right matrix
//         if (dataCol >= 0 && dataCol < totalCols) {
//           if (displayData[row][dataCol]) {
//             leftMatrix[row] |= (1 << (4 - col));  // MSB format
//           }
//         }
//       }
//     }
    
//     // Display the current frame for a consistent time
//     unsigned long startTime = millis();
//     while (millis() - startTime < 100) {  // 100ms per frame
//       hienthi(leftMatrix, rightMatrix);
//     }
//   }
// }

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


// Khai báo mảng khung hình cho hiệu ứng giọt nước
const byte frame_Giot_nuoc[][5] = {
  {0x00,0x00,0x04,0x00,0x00},
  {0x00,0x04,0x0a,0x04,0x00},
  {0x04,0x0a,0x11,0x0a,0x04},
  {0x0a,0x11,0x00,0x11,0x0a},
  {0x11,0x00,0x00,0x00,0x11}
};

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
void LEDTheoHang(int cycles, int rowSpeed, int rowDelay) {
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


/**
 * Hàm tạo hiệu ứng bàn cờ xen kẽ trên hai LED matrix
 * @param cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 * @param speed Tốc độ hiển thị (thời gian mỗi bước tính bằng mili giây)
 */
void LedXenKe(int cycles, int speed) {
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

const byte song_frame[][5] = {
  {0x02,0x07,0x0d,0x18,0x10},
  {0x01,0x03,0x16,0x1c,0x08},
  {0x00,0x11,0x1b,0x0e,0x04},
  {0x10,0x18,0x0c,0x06,0x03}
};

/**
 * Tạo bộ đệm dữ liệu sóng từ các frame hoạt ảnh
 * @param waveData Mảng 2 chiều để lưu trữ dữ liệu sóng
 * @return Tổng số cột trong bộ đệm sóng
 */
int TaoWaveBuffer(byte waveData[][50]) {
  // Số lượng frame trong hoạt ảnh của chúng ta
  int numFrames = sizeof(song_frame) / sizeof(song_frame[0]);
  
  // Tính toán tổng số cột cần thiết
  int totalCols = numFrames * 5 + 10;  // Cột bổ sung để cuộn mượt
  
  // Khởi tạo tất cả dữ liệu buffer về 0
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < totalCols; col++) {
      waveData[row][col] = 0;
    }
  }
  
  // Đổ dữ liệu từ tất cả các frame vào buffer, đặt các frame liền kề nhau
  for (int frameIdx = 0; frameIdx < numFrames; frameIdx++) {
    for (int row = 0; row < 5; row++) {
      byte pattern = song_frame[frameIdx][row];
      
      // Chuyển đổi mỗi byte pattern thành các bit riêng lẻ và đặt vào mảng dữ liệu sóng
      for (int bit = 0; bit < 5; bit++) {
        if (pattern & (0x10 >> bit)) {
          waveData[row][frameIdx * 5 + bit] = 1;
        }
      }
    }
  }
  
  return totalCols;
}

/**
 * Cập nhật một hàng đơn của matrix với dữ liệu từ bộ đệm sóng
 * @param waveData Bộ đệm nguồn chứa các frame kết hợp
 * @param matrixData Bộ đệm matrix đầu ra cần cập nhật
 * @param row Hàng hiện tại đang được cập nhật
 * @param step Bước hoạt ảnh hiện tại
 * @param totalCols Tổng số cột trong bộ đệm sóng
 * @param rightToLeft Hướng di chuyển (true: phải sang trái, false: trái sang phải)
 * @param offset Độ lệch cột (0 cho matrix phải, 5 cho matrix trái)
 */
void UpdateData(byte waveData[][50], byte matrixData[], int row, 
                     int step, int totalCols, bool rightToLeft, int offset) {
  for (int col = 0; col < 5; col++) {
    int dataCol;
    if (rightToLeft) {
      dataCol = col + offset + step;
    } else {
      dataCol = totalCols - 1 - step + col - (5 - offset);
    }
    
    // Đảm bảo vị trí nằm trong phạm vi và xử lý quay vòng nếu cần
    dataCol = dataCol % totalCols;
    
    if (waveData[row][dataCol]) {
      matrixData[row] |= (0x10 >> col);
    }
  }
}

/**
 * Cập nhật dữ liệu LED matrix dựa trên bước hoạt ảnh hiện tại
 * @param waveData Bộ đệm nguồn chứa các frame kết hợp
 * @param leftMatrix Bộ đệm đầu ra cho LED matrix trái
 * @param rightMatrix Bộ đệm đầu ra cho LED matrix phải
 * @param step Bước hoạt ảnh hiện tại (vị trí)
 * @param totalCols Tổng số cột trong bộ đệm sóng
 * @param rightToLeft Hướng di chuyển (true: phải sang trái, false: trái sang phải)
 */
void UpdateFrame(byte waveData[][50], byte leftMatrix[], byte rightMatrix[], 
                          int step, int totalCols, bool rightToLeft) {
  // Xóa cả hai matrix cho frame này
  for (int row = 0; row < 5; row++) {
    leftMatrix[row] = 0;
    rightMatrix[row] = 0;
  }
  
  // Điền dữ liệu vào các matrix từ vị trí thích hợp trong bộ đệm sóng
  for (int row = 0; row < 5; row++) {
    // Xử lý matrix phải (5 cột đầu tiên trong khung nhìn)
    UpdateData(waveData, rightMatrix, row, step, totalCols, rightToLeft, 0);
    
    // Xử lý matrix trái (5 cột tiếp theo trong khung nhìn)
    UpdateData(waveData, leftMatrix, row, step, totalCols, rightToLeft, 5);
  }
}

/**
 * Hiển thị frame hoạt ảnh hiện tại trong một khoảng thời gian xác định
 * @param leftMatrix Dữ liệu LED matrix trái
 * @param rightMatrix Dữ liệu LED matrix phải
 * @param duration Thời gian hiển thị tính bằng mili giây
 */
void TocDoFrame(byte leftMatrix[], byte rightMatrix[], int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    hienthi(leftMatrix, rightMatrix);
  }
}

/**
 * Hàm chính để tạo hiệu ứng sóng bằng cách cuộn các frame kết hợp theo chiều ngang
 * @param cycles Số chu kỳ hoàn chỉnh để chạy hoạt ảnh
 * @param speed Tốc độ di chuyển (mili giây mỗi frame)
 * @param rightToLeft Hướng di chuyển (true = phải sang trái, false = trái sang phải)
 */
void LedSong(int cycles, int speed, bool rightToLeft = true) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  byte waveData[5][50] = {0};  // Bộ đệm cho các frame sóng kết hợp
  
  // Khởi tạo bộ đệm sóng với tất cả các frame
  int totalCols = TaoWaveBuffer(waveData);
  
  // Chạy hoạt ảnh với số chu kỳ được chỉ định
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Cuộn qua toàn bộ mẫu
    for (int step = 0; step < totalCols; step++) {
      // Cập nhật các matrix cho bước hiện tại
      UpdateFrame(waveData, leftMatrix, rightMatrix, step, totalCols, rightToLeft);
      
      // Hiển thị frame hiện tại
      TocDoFrame(leftMatrix, rightMatrix, speed);
    }
  }
}

const byte heart_frame[][5] = {
  {0x0a,0x15,0x11,0x0a,0x04},
  {0x0a,0x1f,0x1f,0x0e,0x04}
};

const byte pacman_frame[][5] = {
  {0x0e,0x1f,0x18,0x1f,0x0e},
  {0x0e,0x1c,0x18,0x1c,0x0e,}
};


/**
 * Hiệu ứng hình Pacman nhấp nháy
 * @param cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 * @param speed Tốc độ chuyển đổi giữa các khung hình (ms)
 */
void pacman(int cycles, int speed) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  // Số lượng khung hình trong animation
  int numFrames = sizeof(pacman_frame) / sizeof(pacman_frame[0]);
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Chạy qua từng khung hình trong chu kỳ
    for (int frame = 0; frame < numFrames; frame++) {
      // Xóa cả hai ma trận
      for (int row = 0; row < 5; row++) {
        leftMatrix[row] = 0;
        rightMatrix[row] = 0;
      }
      
      // Điền dữ liệu vào cả hai ma trận
      for (int row = 0; row < 5; row++) {
        leftMatrix[row] = pacman_frame[frame][row];
        rightMatrix[row] = pacman_frame[frame][row];
      }
      
      // Hiển thị khung hình hiện tại
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
  }
}

/**
 * Hiệu ứng hình trái tim đập
 * @param cycles Số chu kỳ hoàn chỉnh của hiệu ứng
 * @param speed Tốc độ chuyển đổi giữa các khung hình (ms)
 */
void trai_tim(int cycles, int speed) {
  byte leftMatrix[5] = {0};
  byte rightMatrix[5] = {0};
  
  // Số lượng khung hình trong animation
  int numFrames = sizeof(heart_frame) / sizeof(heart_frame[0]);
  
  for (int cycle = 0; cycle < cycles; cycle++) {
    // Chạy qua từng khung hình trong chu kỳ
    for (int frame = 0; frame < numFrames; frame++) {
      // Xóa cả hai ma trận
      for (int row = 0; row < 5; row++) {
        leftMatrix[row] = 0;
        rightMatrix[row] = 0;
      }
      
      // Điền dữ liệu vào cả hai ma trận
      for (int row = 0; row < 5; row++) {
        leftMatrix[row] = heart_frame[frame][row];
        rightMatrix[row] = heart_frame[frame][row];
      }
      
      // Hiển thị khung hình hiện tại
      unsigned long startTime = millis();
      while (millis() - startTime < speed) {
        hienthi(leftMatrix, rightMatrix);
      }
    }
  }
}

void ChonAnimation(int number) {
  switch (number) {
    case 1:
        Serial.println("dang chay hieu ung Xoay");
        LedXoay(2, 50);
        break;
    case 2:
        Serial.println("dang chay hieu ung Nhap nhay ngau nhien");
        randomFlicker(500, 10);
        break;
    case 3:
        Serial.println("dang chay hieu ung Nhap nhay diem ngau nhien");
        randomPixelFlicker(500, 2);
        break;
    case 4:
        Serial.println("dang chay hieu ung Giot nuoc");
        HieuUngGiotNuoc(2, 100);
        break;
    case 5:
        Serial.println("dang chay hieu ung Mua");
        HieuUngMua(2000, 3);
        break;
    case 6:
        Serial.println("dang chay hieu ung LED theo hang");
        LEDTheoHang(2, 50, 300);
        break;
    case 7:
        Serial.println("dang chay hieu ung LED xen ke");
        LedXenKe(2, 200);
        break;
    case 8:
        Serial.println("dang chay hieu ung LED song");
        LedSong(2, 150, true);
        break;
    case 9:
        Serial.println("dang chay hieu ung Pacman");
        pacman(3, 200);
        break; 
    case 10:
        Serial.println("dang chay hieu ung Trai tim");
        trai_tim(5, 300);
        break;
    default:
        Serial.println("khong hop le");
    }
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

  //HieuUngGiotNuoc(2, 100); // 2 chu kỳ, tốc độ 100ms
  //delay(500);
  
  // Hiệu ứng mưa
  //HieuUngMua(2000, 2); // Hiển thị trong 2000ms, mật độ 2
  //delay(500);

  //LEDTheoHang(2, 50, 300);
  
  //pacman(3, 200);
  //delay(500);
  
  trai_tim(5, 300);
  //delay(500);
  
}
