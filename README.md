# envelope
👀

Basic configuration for `receiver` and `transmitter`:

- Pin the platform version in platformio.ini: change platform = espressif32 to platform = espressif32@^7.0.1. Version 6.8.1 doesn't ship the lilygo-t3-s3 board definition.
- You may get an error when trying to build because of a missing Python dependency. The newer esptoolpy package requires `intelhex`, which can be installed by running this command:

```sh
~/.platformio/penv/bin/pip install intelhex
```

Test by building with `~/.platformio/penv/bin/pio run` or via the PlatformIO IDE button.

## PlatformIO
Useful commands:

```sh
# compile firmware
pio run

# compile specific environment
pio run -e transmitter

# upload firmware
pio run --target upload

# remove compiled artifacts
pio run --target clean

# run monitor
pio device monitor

# run monitor with specific baud rate
pio device monitor -b 115200

# run unit tests
pio test

# install/update deps from platformio.ini
pio pkg install

# update packages
pio pkg update

# list installed packages
pio pkg list

# list connected serial devices
pio device list

# search supported boards
pio boards esp32

# combo: auto-open monitor after upload:
pio run -t upload -t monitor
```