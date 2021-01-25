# MyCobot 用 Transponder

M5Stack Basic のシリアルから ATOM へコマンドを送信し，処理結果を ATOM から受信する[オリジナルの Transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="ATOM" width="100"/></a><a href="https://gyazo.com/87832527d8f9f13fbcd9ebf54b996180"><img src="https://i.gyazo.com/87832527d8f9f13fbcd9ebf54b996180.png" alt="Basic" width="300"/></a>
</div>

Transponder を起動すると上記の画面が表示されます。ATOM が白色に光ることで，Basic から ATOM にデータの送信ができていることがわかります。

<div align="center">
    <a href="https://gyazo.com/3f28eb115110e1739da3209b8fa91761"><img src="https://i.gyazo.com/3f28eb115110e1739da3209b8fa91761.png" alt="送受信の表示" width="300"/></a>
</div>

シリアル経由で ATOM へデータを送信すると左の丸が緑に，ATOM からデータを受信すると右の丸が赤になります。

<div align="center">
    <a href="https://gyazo.com/d554c9f2da2017f51bcb45b6c9f83218"><img src="https://i.gyazo.com/d554c9f2da2017f51bcb45b6c9f83218.png" alt="コマンド表示" width="300"/></a>
</div>

A ボタン（一番左のボタン）を押すと，画面に送信コマンドを表示するかを選択できます。ON にすると，送信しているコマンドと今までに送ったコマンド数を画面に表示します。表示するかの設定は本体の不揮発性メモリに保存され，起動時に前回の設定が反映されます。

<div align="center">
    <a href="https://gyazo.com/14fce79ed5de14e3d41ab9c71bf86a76"><img src="https://i.gyazo.com/14fce79ed5de14e3d41ab9c71bf86a76.png" alt="ジョイント角度表示" width="300"/></a>
</div>

コマンドが送られていない状態で B ボタン（真ん中のボタン）を押すと，Get Angles コマンドを送信し，各ジョイントの角度を画面に表示します。

コマンドが送られていない状態で C ボタン（一番右のボタン）を押すと，Set Free Move コマンドを送信します。サーボが固定されてしまっているときに押すと自由に動かせるようになります。

※v0.0.3 時点では，起動時に LED が白に光るときと Free ボタンを押したときに，どちらもコマンドを直接`Serial2`に書き込む関係で，送信ではなく受信扱いになって右の丸が赤になります。

## インストール方法

[PlatformIO](https://platformio.org/)環境に対応しています。動作確認は Windows 10 上で[PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)を使用して実施しています。

```
$ git clone https://github.com/3110/mycobot-transponder
$ cd mycobot-transponder
$ platformio run --target=upload
```

Basic に書き込む際のシリアルポートは自動判別されますが，うまくいかないときは`platformio.ini`の`upload_port`を設定してください。

[Arduino IDE](https://www.arduino.cc/en/software)を使用する場合は，新たに以下のようなフォルダを用意して，`transponder.ino`を開いてください。

```
transponder
+-- transponder.ino  <-- src/main.cpp
+-- MyCobot.h        <-- lib/MyCobot/MyCobot.h
+-- MyCobot.cpp      <-- lib/MyCobot/MyCobot.cpp
```
