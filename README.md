This is a project I completed to learn more about baremetal embedded development in a Linux environment. This   
project is a continuation of a project I completed in my Computer Organization class, with the goal to get  
a Nucleo-64 board (STM32L476xxx mcu) to play a game of Rock Paper Scissors with the ATmega32U4 mcu board  
that I used in that class via UART.     

This project primarily focuses on creating custom drivers such as UART, Systick, GPIO, etc. for the Nucleo-64 board.   

I used the ARM GNU toolchain for compilation and linking, OpenOCD for programming and debugging, and the linux terminal for 
all code writing.   

Changes (11/8/2024):
- Changed the AVR code from the polling style implementation to an interrupt implementation.  
- Changed game logic on the STM32 code to work with the ATmega32U4 mcu when the ATmega32U4 is running an interrupt version of the rock paper scissors game.  
- Fixed bug where the AVR mcu wouldn't update the SendReady variable by changing  
  `UART4_RDR |= data_out` to  
  `UART4_RDR = data_out`  
  in src/uart.c.  
- Moved UART4 and EXTI ISRs to main.c to prevent the use of global variables across different files.  

Testing Version 2 (newer):  
https://youtu.be/n9kZ7tizfu0  

Testing Version 1 (older):  
https://youtu.be/cP5Dmpc3x68  

I followed this playlist to help me with this project:   
https://www.youtube.com/playlist?list=PLERTijJOmYrDiiWd10iRHY0VRHdJwUH4g  
