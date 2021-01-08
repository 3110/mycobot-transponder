# MyCobot用Transponder

M5Stack BasicのシリアルからATOMへコマンドを送信し，処理結果をATOMから受信する[オリジナルのTransponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder)に起動画面とデータ送受信の様子を表示する機能を付けました。

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="AtOM" width="100"/></a><a href="https://gyazo.com/49414edc56b8c616abb08437db533ddb"><img src="https://i.gyazo.com/49414edc56b8c616abb08437db533ddb.png" alt="起動画面" width="300"/></a>
</div>

Transponderを起動すると上記の画面が表示されます。ATOMが白色に光ることで，BasicからATOMにデータの送信ができていることがわかります。

<div align="center">
    <a href="https://gyazo.com/a5bf381f60a159fb0fd05c45d7828e53"><img src="https://i.gyazo.com/a5bf381f60a159fb0fd05c45d7828e53.png" alt="送受信の様子" width="300"/></a>
</div>

シリアル経由でATOMへデータを送信すると左の丸が緑に，ATOMからデータを受信すると右の丸が赤になります。

## TODO

* 画面に送受信情報を表示する。
