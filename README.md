# MyCobot用Transponder

M5Stack BasicのシリアルからATOMへコマンドを送信し，処理結果をATOMから受信する[オリジナルのTransponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="AtOM" width="100"/></a><a href="https://gyazo.com/da2d87b3179030b0e93cc03d73465b50"><img src="https://i.gyazo.com/da2d87b3179030b0e93cc03d73465b50.png" alt="起動画面" width="300"/></a>
</div>

Transponderを起動すると上記の画面が表示されます。ATOMが白色に光ることで，BasicからATOMにデータの送信ができていることがわかります。

<div align="center">
    <a href="https://gyazo.com/3d41f42c274f6635b05672cf70dc0d22"><img src="https://i.gyazo.com/3d41f42c274f6635b05672cf70dc0d22.png" alt="送受信の様子" width="300"/></a>
</div>

シリアル経由でATOMへデータを送信すると左の丸が緑に，ATOMからデータを受信すると右の丸が赤になります。

## TODO

* 画面に送受信情報を表示する。
