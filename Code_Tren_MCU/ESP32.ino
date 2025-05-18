#include <WiFi.h>
#include <WebServer.h>

String str = "";
bool dataSent   = true;
bool select_m   = false;
bool mainpage   = true;
bool animation  = false;
int  ani        = 0;

const char* ssid     = "Nhóm 18 linh kiện bán dẫn";
const char* password = "23021224";

WebServer server(80);

String htmlPage = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Nhóm 18 Điều Chỉnh LED</title>
  <meta charset="UTF-8">
  <style  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #dde3f5;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }
    .container {
      text-align: center;
    }
    .title {
      font-size: 36px;
      margin-bottom: 50px;
      color: #333;
    }
    .button {
      display: inline-block;
      margin: 10px;
      padding: 15px 25px;
      background-color: #4a89dc;
      color: #fff;
      text-decoration: none;
      font-size: 18px;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    .button:hover {
      background-color: #357ABD;
    }
    .footer {
      position: absolute;
      bottom: 20px;
      font-size: 14px;
      color: #666;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2 class="title">Điều Chỉnh Ma Trận LED</h2>
    <a href="/matrix" class="button">Điều chỉnh LED trực tiếp</a>
    <a href="/text"   class="button">Chạy LED theo chữ</a>
  </div>
  <div class="footer">NHÓM 18 LINH KIỆN BÁN DẪN</div>
</body>
</html>
)rawliteral";

String textInputPage = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Text Input</title>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #dde3f5;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      position: relative;
    }
    .note {
      color: red;
      font-weight: bold;
      margin: 10px 0;
    }
    .input-box {
      width: 400px;
      padding: 10px;
      font-size: 18px;
    }
    .form-container {
      text-align: center;
      margin-top: -10mm;
    }
    .back-button {
      position: absolute;
      top: 20px;
      left: 20px;
      background-color: #87CEEB;
      padding: 10px 20px;
      border-radius: 5px;
      text-decoration: none;
      color: black;
      font-size: 16px;
    }
    .back-button:hover {
      background-color: #6BB9D8;
    }
    .submit-button {
      width: 220px;
      padding: 8px;
      font-size: 16px;
      background-color: #87CEEB;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    .submit-button:hover {
      background-color: #6BB9D8;
    }
    h2 {
      position: absolute;
      top: 60px;
      left: 0;
      right: 0;
      display: flex;
      justify-content: center;
      font-size: 28px;
    }
    #error-message {
      color: red;
      font-weight: bold;
      margin-top: 10px;
      min-height: 20px;
    }
  </style>
</head>
<body>
  <a href="/" class="back-button">Quay lại</a>
  <div class="form-container">
    <h2>Nhập Chữ Muốn Chạy</h2>
    <form id="text-form" action="/submit-text" method="POST">
      <div class="note">Lưu ý: Chỉ nhập các chữ cái, số và dấu cách!!!</div>
      <input type="text" id="data" name="user_data" class="input-box" required><br><br>
      <div id="error-message"></div>
      <input type="submit" value="Hoàn tất" class="submit-button">
    </form>
  </div>
  <script>
    document.getElementById('text-form').addEventListener('submit', function(event) {
      const input = document.getElementById('data').value.trim();
      const errorMessage = document.getElementById('error-message');
      if (input === '') {
        event.preventDefault();
        errorMessage.textContent = 'Dữ Liệu Không Được Để Trống';
      } else if (!/^[a-zA-Z0-9\s]*$/.test(input)) {
        event.preventDefault();
        errorMessage.textContent = 'Dữ liệu không hợp lệ';
      } else {
        errorMessage.textContent = '';
      }
    });
  </script>
</body>
</html>
)rawliteral";

String matrixPage = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>5x5 LED Matrix</title>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #dde3f5;
      text-align: center;
      padding: 20px;
      position: relative;
    }
    .matrix {
      display: grid;
      grid-template-columns: repeat(5, 40px);
      grid-gap: 5px;
      margin: 20px auto;
      justify-content: center;
    }
    .cell {
      width: 40px;
      height: 40px;
      background-color: #fff;
      border: 2px solid #000;
      border-radius: 50%;
      cursor: pointer;
    }
    .cell.active {
      background-color: #f00;
    }
    .back-button {
      position: absolute;
      top: 20px;
      left: 20px;
      padding: 8px 12px;
      background-color: #4a89dc;
      color: white;
      text-decoration: none;
      border-radius: 4px;
    }
    .back-button:hover {
      background-color: #357ABD;
    }
    .effects-button {
      display: inline-block;
      margin: 10px;
      padding: 15px 25px;
      background-color: #4a89dc;
      color: #fff;
      text-decoration: none;
      font-size: 18px;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    .effects-button:hover {
      background-color: #357ABD;
    }
    h2 {
      font-size: 24px;
      margin-bottom: 20px;
    }
  </style>
</head>
<body>
  <a href="/" class="back-button">Quay lại</a>
  <h2>Ma Trận LED 5x5</h2>
  <div class="matrix" id="matrix"></div>
  <a href="/effects" class="effects-button">Các hiệu ứng có sẵn</a>
  <script>
    const matrixSize = 5;
    let state = Array(matrixSize).fill().map(() => Array(matrixSize).fill(0));

    function createMatrix() {
      const matrix = document.getElementById('matrix');
      matrix.innerHTML = '';
      for (let row = 0; row < matrixSize; row++) {
        for (let col = 0; col < matrixSize; col++) {
          const cell = document.createElement('div');
          cell.className = 'cell';
          cell.dataset.row = row;
          cell.dataset.col = col;
          cell.addEventListener('click', () => {
            state[row][col] = state[row][col] ? 0 : 1;
            cell.classList.toggle('active', state[row][col]);
            sendMatrixState();
          });
          matrix.appendChild(cell);
        }
      }
    }

    function sendMatrixState() {
      fetch('/submit-matrix', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ matrix: state })
      });
    }

    createMatrix();
  </script>
</body>
</html>
)rawliteral";

String confirmationPage = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Text Confirmation</title>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #dde3f5;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }
    h2 {
      font-size: 28px;
      text-align: center;
      margin-bottom: 20px;
    }
    .back-button {
      background-color: #87CEEB;
      padding: 10px 20px;
      border-radius: 5px;
      text-decoration: none;
      color: black;
      font-size: 16px;
    }
    .back-button:hover {
      background-color: #6BB9D8;
    }
  </style>
</head>
<body>
  <h2>Chữ đang chạy là: %TEXT%</h2>
  <a href="/reset-text" class="back-button">Quay lại</a>
</body>
</html>
)rawliteral";

void sendMatrixBytes(int matrix[5][5]) {
  int virtualMatrix[8][8] = {0};
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      virtualMatrix[j][i] = matrix[i][j];
  byte temp[8];
  for (int i = 0; i < 8; i++) {
    byte v = 0;
    for (int j = 0; j < 8; j++)
      if (virtualMatrix[i][j]) v |= (1 << (7 - j));
    temp[i] = v;
  }
  Serial.write(0x01);
  Serial.write(temp, 8);
}

void sendAnimationState() {
  Serial.write(0x04);
  Serial.write(animation);
}

void sendAniValue() {
  Serial.write(0x05);
  Serial.write(ani);
}

void handleEffects() {
  mainpage   = false;
  select_m   = false;
  if (!animation) {
    animation = true;
    sendAnimationState();
  }
  if (server.hasArg("num")) {
    ani = server.arg("num").toInt();
    sendAniValue();
  }

  String page = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Các hiệu ứng có sẵn</title>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #dde3f5;
      text-align: center;
      padding: 20px;
      position: relative;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(5, 60px);
      grid-gap: 15px;
      justify-content: center;
      margin-top: 50px;
    }
    .grid a {
      display: flex;
      align-items: center;
      justify-content: center;
      width: 60px;
      height: 60px;
      background-color: #87CEEB;
      color: white;
      text-decoration: none;
      font-size: 18px;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    .grid a.selected {
      background-color: red;
    }
    .grid a:hover {
      background-color: #6BB9D8;
    }
    .back-button {
      position: absolute;
      top: 20px;
      left: 20px;
      padding: 8px 12px;
      background-color: #87CEEB;
      color: black;
      text-decoration: none;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    .back-button:hover {
      background-color: #6BB9D8;
    }
    h2 {
      margin-top: 20px;
      font-size: 24px;
    }
  </style>
</head>


<body>
  <a href="/matrix" class="back-button">Quay lại</a>
  <h2>Các hiệu ứng có sẵn</h2>
  <div class="grid">
)rawliteral";

  for (int i = 1; i <= 10; i++) {
    if (i == ani) {
      page += "<a href=\"/effects?num=" + String(i) + "\" class=\"selected\">" + String(i) + "</a>";
    } else {
      page += "<a href=\"/effects?num=" + String(i) + "\">" + String(i) + "</a>";
    }
  }

  page += R"rawliteral(
  </div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void handleRoot() {
  mainpage   = true;
  select_m   = false;
  if (animation) {
    animation  = false;
    ani        = 0;
    sendAnimationState();
    sendAniValue();
  }
  server.send(200, "text/html", htmlPage);
}

void handleTextInput() {
  mainpage   = false;
  select_m   = false;
  if (animation) {
    animation  = false;
    ani        = 0;
    sendAnimationState();
    sendAniValue();
  }
  server.send(200, "text/html", textInputPage);
}

void handleMatrix() {
  mainpage   = false;
  select_m   = true;
  if (animation) {
    animation  = false;
    ani        = 0;
    sendAnimationState();
    sendAniValue();
  }
  Serial.write(0x03);
  server.send(200, "text/html", matrixPage);
}

void handleTextSubmit() {
  mainpage   = false;
  animation  = false;
  if (server.hasArg("user_data")) {
    String input = server.arg("user_data");
    bool ok = true;
    for (char c : input) if (!isalnum(c) && c != ' ') { ok = false; break; }
    if (input == "") {
      server.send(400, "text/html", textInputPage + "<script>document.getElementById('error-message').textContent='Dữ Liệu Không Được Để Trống';</script>");
    } else if (!ok) {
      server.send(400, "text/html", textInputPage + "<script>document.getElementById('error-message').textContent='Dữ liệu không hợp lệ';</script>");
    } else {
      str = input;
      dataSent = false;
      String resp = confirmationPage;
      resp.replace("%TEXT%", str);
      server.send(200, "text/html", resp);
    }
  } else {
    server.send(400, "text/html", textInputPage + "<script>document.getElementById('error-message').textContent='Dữ Liệu Không Được Để Trống';</script>");
  }
}

void handleResetText() {
  mainpage   = false;
  select_m   = false;
  if (animation) {
    animation  = false;
    ani        = 0;
    sendAnimationState();
    sendAniValue();
  }
  str        = "";
  dataSent   = false;
  Serial.write(0x03);
  server.send(200, "text/html", textInputPage);
}

void handleMatrixSubmit() {
  mainpage   = false;
  animation  = false;
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    int matrix[5][5];
    int row = 0, col = 0;
    bool inNum = false;
    for (int i = 0; i < body.length() && row < 5; i++) {
      char c = body[i];
      if (c == '0' || c == '1') {
        matrix[row][col++] = c - '0';
        inNum = true;
      } else if (c == ']' && inNum) {
        row++;
        col = 0;
        inNum = false;
      }
    }
    sendMatrixBytes(matrix);
    server.send(200, "application/json", "{}");
  } else {
    server.send(400, "application/json", "{}");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/",             handleRoot);
  server.on("/text",         handleTextInput);
  server.on("/matrix",       handleMatrix);
  server.on("/effects",      handleEffects);
  server.on("/submit-text",  HTTP_POST, handleTextSubmit);
  server.on("/submit-matrix",HTTP_POST, handleMatrixSubmit);
  server.on("/reset-text",   handleResetText);

  server.begin();
}

void loop() {
  server.handleClient();

  if (!dataSent) {
    Serial.write(0x02);
    Serial.println(str);
    dataSent = true;
  }
}
