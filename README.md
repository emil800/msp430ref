# msp430ref

![plot](./img/msp430G2x53_pins.png)

# Compiler

Install compiler:

sudo apt install gcc-msp430 

Build:

msp430-gcc -mmcu=msp430g2553 main.c pwm.c -o main.out

# Debugger/Flash 

mspdebug rf2500
prog main.o


