# msp430ref

![plot](./img/msp430G2x53_pins.png)

# Compiler

### Install compiler:

sudo apt install gcc-msp430

### Build:

- with make
  make all
- with gcc
  msp430-gcc -mmcu=msp430g2553 main.c pwm.c -o main.out

# Debugger/Flash

Required - Debugger from https://dlbeer.co.nz/mspdebug/

./flash.sh
