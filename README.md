# MyCobot 用 Transponder

M5Stack Basic のシリアルから ATOM へコマンドを送信し，処理結果を ATOM から受信する[オリジナルの Transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="AtOM" width="100"/></a><a href="https://gyazo.com/556352b86c8309f99836349b492a17f1"><img src="https://i.gyazo.com/556352b86c8309f99836349b492a17f1.png" alt="起動画面" width="300"/></a>
</div>

Transponder を起動すると上記の画面が表示されます。ATOM が白色に光ることで，Basic から ATOM にデータの送信ができていることがわかります。

<div align="center">
    <a href="https://gyazo.com/6792aca0b658745d90c7b9adc90f2f82"><img src="https://i.gyazo.com/6792aca0b658745d90c7b9adc90f2f82.png" alt="送受信の表示" width="300"/></a>
</div>

シリアル経由で ATOM へデータを送信すると左の丸が緑に，ATOM からデータを受信すると右の丸が赤になります。

<div align="center">
    <a href="https://gyazo.com/078ff1d56b985e046877babb9c9e659e"><img src="https://i.gyazo.com/078ff1d56b985e046877babb9c9e659e.png" alt="コマンド表示" width="300"/></a>
</div>

A ボタン（一番左のボタン）を押すと，画面に送信コマンドを表示するかを選択できます。ON にすると，送信しているコマンドと今までに送ったコマンド数を画面に表示します。表示するかの設定は本体の不揮発性メモリに保存され，起動時に前回の設定が反映されます。

コマンドが送られていない状態で C ボタン（一番右のボタン）を押すと，Set Free Move コマンドを送信します。サーボが固定されてしまっているときに押すと自由に動かせるようになります。

※v0.0.2 時点では，起動時に LED が白に光るときと Free ボタンを押したときに，どちらもコマンドを直接`Serial2`に書き込む関係で，送信ではなく受信扱いになって右の丸が赤になります。
