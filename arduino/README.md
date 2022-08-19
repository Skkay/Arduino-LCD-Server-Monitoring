## Upload to an Arduino Nano
### Using `avrdure`
https://github.com/avrdudes/avrdude

#### Find serial device
```
dmesg | grep tty
# OR
ls -l /dev/ttyUSB*
```

#### Uploading hex file
*Assuming that our device is `/dev/ttyUSB0` and the hex file is named `firware.hex` in the current directory.*

```
avrdude -v -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:./firmware.hex:i
```

## Testing serial output

### Read
```
cat /dev/ttyUSB0
```

### Write
Example of command to write *Hello World!* on the first line of the LCD screen:
```
echo 'cmd:lcd:write//0//0//Hello World!' > /dev/ttyUSB0
```

If you got a *permission denied* error, you probably need to add your user to the `dialout` group then logout/login:
```
sudo usermod -a -G dialout user
# Logout/Login required
```

If you got a *device busy* error, you can use the following command to find the PID using the device:
```
sudo lsof /dev/ttyUSB0
```

## Available commands
*NOTE: Commands arguments are separed by `//` (double slash).*
- `cmd:lcd:write//<pos_x>//<pos_y>//<text>`: Write a text to the LCD screen (The position *X=0*, *Y=0* is the upper-left corner).
- `cmd:lcd:clear`: Clears the LCD screen
- `cmd:lcd:backlight`: Turns ON LCD backlight
- `cmd:lcd:nobacklight`: Turns OFF LCD backlight
