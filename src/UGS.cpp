#include "UGS.h"

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
      versionDiv.textContent = "Sürüm bilgisi çözülemedi! Sürümü yükleme
