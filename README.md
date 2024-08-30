This is a project I completed to learn more about baremetal embedded development in a Linux environment. This project
is a continuation of a project I completed in my Computer Organization class, with the goal to
get an Nucleo-64 board (STM32L476xxx mcu) to play a game of Rock Paper Scissors with the ATmega32U4 
mcu board that I used in that class via UART. 

This project primarily focuses on creating custom drivers such as UART, Systick, GPIO, etc. for the Nucleo-64 board. 

I used the ARM GNU toolchain for compilation and linking, OpenOCD for programming and debugging, and the linux terminal for 
all code writing. 

I followed this playlist to help me with this project:
youtube.com

This is the link to a Youtube video showing the project in action:
https://youtu.be/cP5Dmpc3x68

Setbacks:
I initially planned to only write code for the STM32, since I had already verified in my Computer Organization class
that the Rock Paper Scissors game already worked for two ATmega32U4 microcontrollers. However, disaster struck when 
I realized that the ATmega32U4 mcu that I was using from that class had gotten its interrupt controller (and possibly some other
components) fried at some point after completing the class. I did not want to buy a new board, so I just rewrote the ATmega32U4
code to work via polling, entirely removing any interrupts from the code. While this solution was pretty jank, I still got 
both sides to work, so I would count that as a win. You can see this code in the avr_files folder. 
