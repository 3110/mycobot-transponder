# MyCobot 用 Transponder

M5Stack Basic のシリアルから ATOM へコマンドを送信し，処理結果を ATOM から受信する[オリジナルの Transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

v0.0.5 より，[ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) 経由でコマンドを受信する機能を追加しました。デフォルトではこの機能はオフになっています。

## インストール方法

※この Transponder を使用する場合は，ATOM に Atom2.5 以降のファームウエアが書き込まれている必要があります。

[PlatformIO](https://platformio.org/)環境に対応しています。動作確認は Windows 10 上で[PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)を使用して実施しています。

```
$ git clone https://github.com/3110/mycobot-transponder
$ cd mycobot-transponder
$ platformio run --target=upload
```

Basic に書き込む際のシリアルポートは自動判別されますが，うまくいかないときは`platformio.ini`の`upload_port`を設定してください。

`platformio.ini`の`[env:m5stack-basic]`セクションにある`build_flags`で`-DENABLE_ESP_NOW`を有効にしてコンパイルすると，ESP-NOW 経由でコマンドを受信することができるようになります。

[Arduino IDE](https://www.arduino.cc/en/software)を使用する場合は，新たに以下のようなフォルダを用意して，`transponder.ino`を開いてください。

```
transponder
+-- transponder.ino  <-- src/main.cpp
+-- CommandQueue.h   <-- src/CommandQueue.h
+-- CommandQueue.cpp <-- src/CommandQueue.cpp
+-- MyCobot.h        <-- src/MyCobot.h
+-- MyCobot.cpp      <-- src/MyCobot.cpp
+-- Transponder.h    <-- src/Transponder.h
+-- Transponder.cpp  <-- src/Transponder.cpp
+-- m5stack          <-- src/m5stack
    +-- EspNowReceiver.h
    +-- EspNowReceiver.cpp
    +-- UiTask.h
    +-- UiTask.cpp
```

## 起動画面

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="ATOM" width="100"/></a><a href="https://gyazo.com/eec8bb522f8a654c8aed93e12165be21"><img src="https://i.gyazo.com/eec8bb522f8a654c8aed93e12165be21.png" alt="起動画面" width="300"/></a>
</div>

Transponder を起動すると上記の画面が表示されます。ATOM が白色に光ることで，Basic から ATOM にデータの送信ができていることがわかります。

<div align="center">
<a href="https://gyazo.com/3af92716e01062bf67813eff492dcf62"><img src="https://i.gyazo.com/3af92716e01062bf67813eff492dcf62.png" alt="ESW-NOWが有効のときの起動画面" width="300"/></a>
</div>

ESP-NOW を有効にし，接続状態になると黄色の丸が表示されます。

## 送信状況の表示（A ボタン）

<div align="center">
    <a href="https://gyazo.com/7baff24a0d9af432b7b625af7b6cd715"><img src="https://i.gyazo.com/7baff24a0d9af432b7b625af7b6cd715.png" alt="コマンド表示" width="300"/></a>
</div>

A ボタン（一番左のボタン）を押すと，画面に送信状況を表示するかを選択できます。ON にすると，送信しているコマンドと今までに送ったコマンド数，そして送っているバイト列を画面に表示します。バイト列の中のコマンド番号にあたるバイト（4 バイト目）は背景が青色になります。

送信状況を表示するかの設定は本体の不揮発性メモリに保存され，起動時に前回の設定が反映されます。

## 各ジョイントの角度表示（B ボタン）

<div align="center">
    <a href="https://gyazo.com/a7c6de94343eb087f280160a3e90d87f"><img src="https://i.gyazo.com/a7c6de94343eb087f280160a3e90d87f.png" alt="ジョイント角度表示" width="300"/></a>
</div>

コマンドが送られていない状態で B ボタン（真ん中のボタン）を押すと，Get Angles コマンドを送信し，各ジョイントの角度を画面に表示します。
あくまでボタンを押したときの状態を表示するだけで，リアルタイムに表示を更新しません。

## 各ジョイントの開放（C ボタン）

コマンドが送られていない状態で C ボタン（一番右のボタン）を押すと，Set Free Move コマンドを送信します。サーボが固定されてしまっているときに押すと自由に動かせるようになります。

## 注意事項

v0.1.0 から送受信を表示する丸（緑色・赤色）をなくしました（表示していてもあまり意味はないため）。
