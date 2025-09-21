#include "ESP32_OTA_Server.h"

// Embed your full HTML here
const char* default_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>GM UGS - Yazılım Güncelleme</title>
<style>
  body {
    font-family: "Segoe UI", Tahoma, Geneva, Verdana, sans-serif;
    background: #303030;
    color: #eee;
    margin: 0;
    padding: 0;
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
  }
  .ota-container {
    width: 90%;
    max-width: 600px;
    aspect-ratio: 1 / 1;
    background: url('https://i.ibb.co/67hhQVZv/Ads-z.png') no-repeat center center;
    background-size: cover;
    border-radius: 16px;
    padding: 20px;
    box-shadow: 0 20px 50px rgba(0,0,0,0.7);
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    backdrop-filter: blur(5px);
    text-align: center;
    color: #fff;
    position: relative;
  }
  h1 { font-size: 22px; margin-bottom: 10px; color: #ffffff; text-shadow: 0 0 8px #f0a500; }
  p { font-size: 14px; color: #ccc; margin-bottom: 20px; }
  #version { font-size: 16px; color: #fff; margin-bottom: 15px; font-weight: bold; }
  input[type="file"] { display: none; }
  label {
    background-color: #f0a500; color: #121212;
    padding: 12px 25px; border-radius: 8px; cursor: pointer;
    font-weight: bold; margin-bottom: 15px; display: inline-block;
    transition: 0.2s;
  }
  label:hover { background-color: #ffaa33; }
  button {
    background-color: #1e90ff; color: #fff; border: none;
    padding: 12px 25px; border-radius: 8px; cursor: pointer;
    font-weight: bold; transition: 0.2s; font-size: 16px;
  }
  button:hover { background-color: #3aa0ff; }
  #status { margin-top: 15px; font-size: 13px; color: #ccc; min-height: 18px; }
  .warning {
    position: absolute; bottom: 20px; width: calc(100% - 40px);
    padding: 12px; background: #b00020; color: #fff; border-radius: 8px;
    font-size: 13px; font-weight: bold;
  }
</style>
</head>
<body>
  <div class="ota-container">
      <h1>Yazılım Güncelleme Paneli</h1>
      <p>Size verilen .bin dosyasını aşağıdaki yere yükleyin.</p>
      <div id="version">Sürüm: ---</div>
      <form id="uploadForm" enctype="multipart/form-data">
        <input type="file" id="file" name="firmware">
        <label for="file">Dosya Seç</label><br>
        <button type="submit">Yükle</button>
      </form>
      <div id="status"></div>
      <div class="warning">⚠️ Dosya yüklerken dikkatli olun, yanlış dosya yüklerseniz ürününüz bozulabilir!</div>
  </div>

<script>
  const form = document.getElementById('uploadForm');
  const fileInput = document.getElementById('file');
  const statusDiv = document.getElementById('status');
  const versionDiv = document.getElementById('version');

  function setVersionFromFile(file) {
    if (!file) return;
    const filename = file.name;
    const base64Version = filename.split('.')[0];
    try {
      const version = atob(base64Version);
      versionDiv.textContent = "Sürüm: " + version;
    } catch(e) {
      versionDiv.textContent = "Sürüm bilgisi çözülemedi!";
    }
  }

  fileInput.addEventListener('change', () => setVersionFromFile(fileInput.files[0]));

  form.addEventListener('submit', function(e) {
    e.preventDefault();
    const file = fileInput.files[0];
    if (!file) { statusDiv.textContent = "Lütfen bir dosya seçin!"; return; }
    setVersionFromFile(file);

    const xhr = new XMLHttpRequest();
    xhr.open("POST", "/update", true);
    xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");

    xhr.upload.addEventListener("progress", function(evt) {
      if (evt.lengthComputable) {
        const percent = Math.round(evt.loaded / evt.total * 100);
        statusDiv.textContent = "Yükleniyor: " + percent + "%";
      }
    });

    xhr.onload = function() {
      if (xhr.status == 200) { statusDiv.textContent = "Yükleme tamamlandı! Cihaz yeniden başlatılıyor..."; }
      else { statusDiv.textContent = "Yükleme başarısız!"; }
    };

    const formData = new FormData();
    formData.append("firmware", file);
    xhr.send(formData);
  });
</script>
</body>
</html>
)rawliteral";

// Constructor
ESP32_OTA_Server::ESP32_OTA_Server(WebServer& srv) {
    _server = &srv;
    _ota_html = default_html;
}

// Start OTA server
void ESP32_OTA_Server::begin() {
    _server->on("/", HTTP_GET, [this]() { handleRoot(); });
    _server->on("/update", HTTP_POST, 
        [this]() { _server->send(200, "text/plain", "Update finished"); ESP.restart(); },
        [this]() { handleUpdate(); }
    );
    _server->begin();
}

// Handle incoming HTTP requests
void ESP32_OTA_Server::handleClient() {
    _server->handleClient();
}

// Serve the HTML page
void ESP32_OTA_Server::handleRoot() {
    _server->send(200, "text/html", _ota_html);
}

// Handle OTA upload
void ESP32_OTA_Server::handleUpdate() {
    HTTPUpload& upload = _server->upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) Update.printError(Serial);
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) Update.printError(Serial);
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) Serial.printf("Update Success: %u bytes\n", upload.totalSize);
        else Update.printError(Serial);
    }
}
