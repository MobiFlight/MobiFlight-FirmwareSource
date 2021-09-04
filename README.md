# MobiFlight-FirmwareSource

PlatformIO version of the MobiFlight firmware source.

To build:

1. Install [Visual Studio Code](https://code.visualstudio.com/Download)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Edit `platformio.ini` and change the `default_envs` line to be the comma separated list of boards you want to build for.
4. From the Command Palette select `PlatformIO: Build`.

To upload to a device:

1. From the Command Palette select `PlatformIO: Upload`. Make sure the `default_envs` line of `platformio.ini` is the board type you want to upload to.

To view serial monitor output:

1. From the Command Palette select `PlatformIO: Serial Monitor`. Note that when
   the serial montior is open you cannot connect the MobiFlight desktop app to the
   Arduino. You must close the serial monitor by pressing `Ctrl+C` before attempting
   to connect the desktop app.
