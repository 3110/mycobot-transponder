# MyCobot 用 Transponder

M5Stack Basic のシリアルから ATOM へコマンドを送信し，処理結果を ATOM から受信する[オリジナルの Transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

v0.0.5 より，[ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) 経由でコマンドをブロードキャストする機能を追加しました。デフォルトではこの機能はオフになっています。

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
+-- MyCobot.h        <-- lib/MyCobot/MyCobot.h
+-- MyCobot.cpp      <-- lib/MyCobot/MyCobot.cpp
```

## 起動画面

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="ATOM" width="100"/></a><a href="https://gyazo.com/5ed34bdf2030009a8d644ca7bcc5b671"><img src="https://i.gyazo.com/5ed34bdf2030009a8d644ca7bcc5b671.png" alt="Basic" width="300"/></a>
</div>

Transponder を起動すると上記の画面が表示されます。ATOM が白色に光ることで，Basic から ATOM にデータの送信ができていることがわかります。

ESP-NOW を有効にし，接続状態になると 3 つ丸が表示され，一番左の丸が黄に変わります。ESP-NOW 経由で送信されたコマンドは画面に表示されません。

## 送受信状況の表示

<div align="center">
    <a href="https://gyazo.com/cf3d3673984e5ac018722fef047986b4"><img src="https://i.gyazo.com/cf3d3673984e5ac018722fef047986b4.png" alt="送受信の表示" width="300"/></a>
</div>

シリアル経由で ATOM へデータを送信すると左の丸が緑に，ATOM からデータを受信すると右の丸が赤になります。

## 送信状況の表示（A ボタン）

<div align="center">
    <a href="https://gyazo.com/bdd19d0cfac5f480849e43c59d4bd319"><img src="https://i.gyazo.com/bdd19d0cfac5f480849e43c59d4bd319.png" alt="コマンド表示" width="300"/></a>
</div>

A ボタン（一番左のボタン）を押すと，画面に送信状況を表示するかを選択できます。ON にすると，送信しているコマンドと今までに送ったコマンド数，そして送っているバイト列を画面に表示します。バイト列の中のコマンド番号にあたるバイト（4 バイト目）は背景が青色になります。

送信状況を表示するかの設定は本体の不揮発性メモリに保存され，起動時に前回の設定が反映されます。

## 各ジョイントの角度表示（B ボタン）

<div align="center">
    <a href="https://gyazo.com/bdd19d0cfac5f480849e43c59d4bd319"><img src="https://i.gyazo.com/bdd19d0cfac5f480849e43c59d4bd319.png" alt="ジョイント角度表示" width="300"/></a>
</div>

コマンドが送られていない状態で B ボタン（真ん中のボタン）を押すと，Get Angles コマンドを送信し，各ジョイントの角度を画面に表示します。
あくまでボタンを押したときの状態を表示するだけで，リアルタイムに表示を更新しません。

## 各ジョイントの開放（C ボタン）

コマンドが送られていない状態で C ボタン（一番右のボタン）を押すと，Set Free Move コマンドを送信します。サーボが固定されてしまっているときに押すと自由に動かせるようになります。

## 注意事項

v0.0.5 時点では，起動時に LED が白に光るとき，Free ボタンや Angle ボタンを押したとき，ESP-NOW 経由でコマンドを実行したときは，コマンドを直接`Serial2`に書き込む関係で，コマンド名やバイト列が表示されません。
