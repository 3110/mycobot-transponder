# MyCobot 用 Transponder

M5Stack Basic のシリアルから ATOM へコマンドを送信し，処理結果を ATOM から受信する[オリジナルの Transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="AtOM" width="100"/></a><a href="https://gyazo.com/1efae34d2c922ad8e92b43453d971049"><img src="https://i.gyazo.com/1efae34d2c922ad8e92b43453d971049.png" alt="起動画面" width="300"/></a>
</div>

Transponder を起動すると上記の画面が表示されます。ATOM が白色に光ることで，Basic から ATOM にデータの送信ができていることがわかります。

<div align="center">
    <a href="https://gyazo.com/7058c5abed3b6e779e8e7ba966516c70"><img src="https://i.gyazo.com/7058c5abed3b6e779e8e7ba966516c70.png" alt="送受信の表示" width="300"/></a>
</div>

シリアル経由で ATOM へデータを送信すると左の丸が緑に，ATOM からデータを受信すると右の丸が赤になります。

<div align="center">
    <a href="https://gyazo.com/7e486322bc0ce261d38140716a926011"><img src="https://i.gyazo.com/7e486322bc0ce261d38140716a926011.png" alt="コマンド表示" width="300"/></a>
</div>

A ボタン（一番左のボタン）を押すと，画面に送信コマンドを表示するかを選択できます。ON にすると，送信しているコマンドを画面に表示します。この設定は本体の不揮発性メモリに保存され，起動時に前回の設定が反映されます。
