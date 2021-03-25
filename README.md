[日本語](README_ja_JP.md)
# Unofficial Transponder for MyCobot

This is the unofficial transponder for MyCobot, which is added the functionality of showing data transmission to [the original transponder](https://github.com/elephantrobotics/myCobot/tree/main/Arduino/MycobotBasic/examples/Transponder).

From v0.0.5, the functionality of receiving commands via [ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) is added.
This functionality is disabled by default.

[MyCobot ATOM Controller](https://github.com/3110/mycobot-atom-controller) is also available, and it can send commands using ESP-NOW to this transponder.

# Installation

\* If you use this transponder, you have to write AtomMain 2.5 or later to the ATOM on your MyCobot.


The program can be compiled on the [PlatformIO](https://platformio.org/) environment.

```
$ git clone https://github.com/3110/mycobot-transponder
$ cd mycobot-transponder
$ platformio run --target=upload
```

I have tested the program with [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode) on Windows 10.

The serial port is automatically detected. You should specify 'upload_port' in 'platformio.ino` if it doesn't work.

If you compile the program with `-DENABLE_ESP_NOW` in [env:m5stack-basic]` section of `platformio.ini`, the program can receive commands using ESP-NOW.

If you compile the program using [Arduino IDE](https://www.arduino.cc/en/software), you have to prepare the following folder structure and open `transponder.ino`:

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

## Startup Screen

<div align="center">
    <a href="https://gyazo.com/b3b63dd836e511b7ff89201de3dd3141"><img src="https://i.gyazo.com/b3b63dd836e511b7ff89201de3dd3141.png" alt="ATOM" width="100"/></a><a href="https://gyazo.com/eec8bb522f8a654c8aed93e12165be21"><img src="https://i.gyazo.com/eec8bb522f8a654c8aed93e12165be21.png" alt="Startup Screen" width="300"/></a>
</div>

When you start Transponder, the above screen will be displayed.
The LED on MyCobot ATOM turns white if data has been sent successfully from MyCobot Basic to MyCobot ATOM.

<div align="center">
<a href="https://gyazo.com/3af92716e01062bf67813eff492dcf62"><img src="https://i.gyazo.com/3af92716e01062bf67813eff492dcf62.png" alt="Startup Screen with ESW-NOW" width="300"/></a>
</div>

The yellow circle will be displayed on the right-top corner if ESP-NOW is enabled and connected.

## Show Data Transmission Status (Button A)

<div align="center">
    <a href="https://gyazo.com/7baff24a0d9af432b7b625af7b6cd715"><img src="https://i.gyazo.com/7baff24a0d9af432b7b625af7b6cd715.png" alt="Show commands" width="300"/></a>
</div>

When the button A(left-most) is pressed, you can select whether to show the data transmission status on the screen.
If the data transmission status is 'ON', the sending command, the number of commands sent, and the byte sequence of commands are displayed on the screen.
The command number(the 4th byte) will have a blue background.

The setting of whether to display the data transmission status is saved in the non-volatile storage, and the setting will be reflected at next startup.

## Show Joint Angles(Button B)

<div align="center">
    <a href="https://gyazo.com/a7c6de94343eb087f280160a3e90d87f"><img src="https://i.gyazo.com/a7c6de94343eb087f280160a3e90d87f.png" alt="Joint Angles" width="300"/></a>
</div>

If the button B(middle) is pressed when no command is sent, the 'Get Angles' command is send to display the joint angles on the screen.
The joint angles are not updated in real time.

## Free All Joints(Button C)

If the button C(right-most) is pressed when no command is sent, the 'Set Free Move' command is send.
You can move joints freely in case joints are locked.
## Notes

From v0.1.0, The green(send) and red(receive) circles on the right corner, which display the data transmission status in real time, are removed(they are meaningless).