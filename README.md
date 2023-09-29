# Rose Flight Computer Firmware

This is the repository for the Rose Flight Computer's flight source code.

## Uploading Code

This project is intended to be uploaded to STM32 boards via SWD. Currently two
debugger devices have been tested: clone STLink debuggers and FreeDAP via RasPi
Pico. There is unfortunately no way to exempt individual parameters from the
configuration file from version control(that I know of) so you'll probably need
to change `platformio.ini` for your particular setup:
* For STLink (genuine or clones), use the following lines:
```
debug_tool = stlink
upload_protocol = stlink
```
* For FreeDAP, use the following lines:
```
debug_tool = cmsis-dap
upload_protocol = cmsis-dap
```

## Other useful links
Hardware design for this project:
<https://github.com/Rose-Rocketry/rose-flight-computer-hardware>

FreeDAP source repository:
<https://github.com/ataradov/free-dap>

FreeDAP Pi Pico breakout board:
link coming soon