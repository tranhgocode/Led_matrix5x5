#include <MatrixAnimation.h>

#define SH_CP 13
#define DS 12
#define ST_CP 11

uint8_t rowPins[5] = {10, 9, 8, 7, 6};

LEDMatrix matrix(SH_CP, DS, ST_CP, rowPins);

byte chu[][8] =
{
  {0x00,0x78,0x90,0x90,0x78,0x00,0x00,0x00},//A0
  {0x00,0xf8,0xa8,0xa8,0x70,0x00,0x00,0x00},//B1
  {0x00,0x70,0x88,0x88,0x88,0x00,0x00,0x00},//C2
  {0x00,0xf8,0x88,0x88,0x70,0x00,0x00,0x00},//D3
  {0xf8,0xa8,0xa8,0xa8,0x00,0x00,0x00,0x00},//E4
  {0x00,0xf8,0xa0,0xa0,0x00,0x00,0x00,0x00},//F5
  {0x00,0xf8,0x88,0xa8,0x30,0x00,0x00,0x00},//G6
  {0x00,0xf8,0x20,0x20,0xf8,0x00,0x00,0x00},//H7
  {0x00,0x88,0xf8,0xf8,0x88,0x00,0x00,0x00},//I8
  {0x00,0xf8,0xf8,0x18,0x30,0x00,0x00,0x00},//J9
  {0x00,0xf8,0x30,0x50,0x88,0x00,0x00,0x00},//K10
  {0x00,0xf8,0xf8,0x18,0x18,0x00,0x00,0x00},//L11
  {0xf8,0x40,0x20,0x40,0xf8,0x00,0x00,0x00},//M12
  {0xf8,0x10,0x20,0x40,0xf8,0x00,0x00,0x00},//N13
  {0x70,0x88,0x88,0x88,0x70,0x00,0x00,0x00},//O14
  {0x00,0xf8,0xa0,0xa0,0x40,0x00,0x00,0x00},//P15
  {0xf0,0x90,0x90,0xf0,0x08,0x00,0x00,0x00},//Q16
  {0xf8,0xa0,0xa0,0xf0,0x08,0x00,0x00,0x00},//R17
  {0xe8,0xa8,0xb8,0x00,0x00,0x00,0x00,0x00},//S18
  {0x80,0x80,0xf8,0x80,0x80,0x00,0x00,0x00},//T19
  {0xf0,0x08,0x08,0x08,0xf0,0x00,0x00,0x00},//U20
  {0xe0,0x10,0x08,0x10,0xe0,0x00,0x00,0x00},//V21
  {0x60,0x30,0x60,0x30,0x60,0x00,0x00,0x00},//W22
  {0x88,0x50,0x20,0x50,0x88,0x00,0x00,0x00},//X23
  {0x80,0x40,0x38,0x40,0x80,0x00,0x00,0x00},//Y24
  {0x88,0xc8,0xa8,0x98,0x88,0x00,0x00,0x00},//Z25
  {0x20,0x40,0xf8,0x00,0x00,0x00,0x00,0x00},//1.26
  {0x48,0x98,0xa8,0x48,0x00,0x00,0x00,0x00},//2.27
  {0x00,0xa8,0xa8,0xf8,0x00,0x00,0x00,0x00},//3.28
  {0x20,0x60,0xb8,0x20,0x00,0x00,0x00,0x00},//4.29
  {0xe8,0xa8,0xb8,0x00,0x00,0x00,0x00,0x00},//5.30
  {0xf8,0xa8,0xb8,0x00,0x00,0x00,0x00,0x00},//6.31
  {0x88,0x90,0xa0,0xc0,0x00,0x00,0x00,0x00},//7.32
  {0xf8,0xa8,0xa8,0xf8,0x00,0x00,0x00,0x00},//8.33
  {0xe8,0xa8,0xf8,0x00,0x00,0x00,0x00,0x00},//9.34
  {0x00,0xe8,0xa8,0xa8,0xf8,0x00,0x00,0x00},//0.35
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//dau cach.36
};
bool last_select_m = false;
String str = "";
byte led[][8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte temp[][8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
bool select_m = false;
char input_match[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0',' '};
bool animation = false;
int ani = 0;

void setup() {
  matrix.begin();
  Serial.begin(115200);
  for (int i = 6; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  chay_kq("test ", 50);
}

void hien_thi_led(unsigned int speed, unsigned int indicate) {
  for (int h = 0; h < 8; h++) {
    for (int j = 0; j < 7; j++) {
      led[0][j] = led[0][j + 1];
    }
    led[0][7] = chu[indicate][h];
    for (int w = 0; w < speed; w++) {
      hien_chu(led);
    }
  }
}

void parseHexString(String input) {
  int start = input.indexOf('{') + 1;
  int end = input.indexOf('}');
  String hexValues = input.substring(start, end);
  
  int index = 0;
  String currentHex = "";
  for (int i = 0; i < hexValues.length() && index < 8; i++) {
    if (hexValues[i] == ',') {
      temp[0][index] = strtol(currentHex.c_str(), NULL, 16);
      index++;
      currentHex = "";
    } else {
      currentHex += hexValues[i];
    }
  }
  if (index < 8 && currentHex.length() > 0) {
    temp[0][index] = strtol(currentHex.c_str(), NULL, 16);
  }
}

void loop() {
  if (Serial.available()) {
    byte header = Serial.peek();

    if (header == 0x01) {
      if (Serial.available() >= 9) {
        Serial.read();
        for (int i = 0; i < 8; i++) {
          temp[0][i] = Serial.read();
        }
        select_m = false;
        str = "";
        last_select_m = false;
      }
    }
    else if (header == 0x02) {
      Serial.read();
      str = Serial.readStringUntil('\n');
      select_m = true;
    }
    else if (header == 0x03) {
      if (Serial.available()) {
        Serial.read();
        str = "";
        if (!select_m) {
          for (int i = 0; i < 8; i++) {
            temp[0][i] = 0x00;
          }
        }
        select_m = false;
      }
    }
    else if (header == 0x04) {
      if (Serial.available() >= 2) {
        Serial.read();
        animation = Serial.read();
      }
    }
    else if (header == 0x05) {
      if (Serial.available() >= 2) {
        Serial.read();
        ani = Serial.read();
      }
    }
    else {
      Serial.read();
    }
  }

  if (animation && ani != 0) {
    matrix.ChonAnimation(ani);
  }
  else if (str.length() > 0) {
    if (str.charAt(str.length() - 1) != ' ') {
      str += ' ';
    }
    chay_kq(str, 30);
  }
  else {
    hien_chu(temp);
  }
}

void chay_kq(String chu_input, unsigned int speed) {
  chu_input.toUpperCase();
  for (int q = 0; q < chu_input.length(); q++) {
    for (int e = 0; e < sizeof(input_match); e++) {
      if (chu_input.charAt(q) == input_match[e]) {
        if (q == chu_input.length() - 1) {
          hien_thi_led(10, e);
        } else {
          hien_thi_led(speed, e);
        }
      }
    }
  }
}

void hien_chu(byte chaychu[][8]) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ST_CP, 0);
    if (i <= 4) digitalWrite(10 - i, 1);
    else continue;
    shiftOut(DS, SH_CP, LSBFIRST, chaychu[0][i]);
    if (i <= 4) digitalWrite(ST_CP, 1);
    else continue;
    delay(1);
    digitalWrite(10 - i, 0);
  }
}
