
;***********************************************************
;*
;*	This is the file for the Lab 7 of ECE 375
;*
;*  	Rock Paper Scissors
;* 	Requirement:
;* 	1. USART1 communication
;* 	2. Timer/counter1 Normal mode to create a 1.5-sec delay
;***********************************************************
;*
;*	 Author: Gabriel Rodgers and Will Corby
;*	   Date: 3/16/24
;*
;***********************************************************

.include "m32U4def.inc"         ; Include definition file

;***********************************************************
;*  Internal Register Definitions and Constants
;***********************************************************
.def    mpr = r16               ; Multi-Purpose Register
.def	hand = r17				; register to hold the current hand value
.def	opp = r25				; register to hold the opponent's hand value

; this part copied from Lab 1
.def	waitcnt = r18			; Wait Loop Counter
.def	ilcnt = r19				; Inner Loop Counter
.def	olcnt = r23				; Outer Loop Counter
.def	leds = r24				; register to hold current led preset
.def	gamephase = r15			; register to hold the current game phase

; CANNOT USE r20-r22!!! USED IN LCDDriver.asm



; Use this signal code between two boards for their game ready
.equ    SendReady = 0b11111111

; bunch of variables to store the number values of various hands
.equ	rock = 1
.equ	paper = 2
.equ	scissors = 3

.equ	WTime = 40				; Time to wait in wait loop





;***********************************************************
;*  Start of Code Segment
;***********************************************************
.cseg                           ; Beginning of code segment

;***********************************************************
;*  Interrupt Vectors
;***********************************************************
.org    $0000                   ; Beginning of IVs
	    rjmp    INIT            	; Reset interrupt

.org	$0002					; INT0 IV; mapped to PD4
		rjmp	change_hand

.org	$0004					; INT1 IV; mapped to PD7
		rjmp	start_game

.org	$0032					; USART1, Rx complete interrupt
		rjmp	USART_recv

.org    $0056                   ; End of Interrupt Vectors

;***********************************************************
;*  Program Initialization
;***********************************************************
INIT:
	
	; Initialize the Stack Pointer (VERY IMPORTANT!!!!)
	ldi		mpr, low(RAMEND) ; this limits the stack to RAMEND low
	out		SPL, mpr		; Load SPL with low byte of RAMEND (Stack Pointer Low)
	ldi		mpr, high(RAMEND) ; this limits the stack to RAMEND high
	out		SPH, mpr		; Load SPH with high byte of RAMEND (Stach Pointer High)

	; initialize PORTD and PORTB
	ldi		mpr, 0b01101000	; set PD7, PD4, and PD2 (RXD1) to inputs (also PD0 and PD1), and PD3 (TXD1) to output
	out		DDRD, mpr
	ldi		mpr, 0b10011011	; set PD7, PD4, and PD3 (TXD1) to active low (also PD0 and PD1); set PD2 (RXD1) to active high 
	out		PORTD, mpr

	ldi		mpr, $FF		; set DDRB to all outputs
	out		DDRB, mpr
	ldi		mpr, $00		; set PORTB LEDs to active high outputs
	out		PORTB, mpr

	; initialize t/c control registers
	ldi		mpr, $00		; set TCCR1A to 0 because we are operating in normal mode
	sts		TCCR1A, mpr		

	ldi		mpr, 0b00000100	; set TCCR1B to this value to set normal mode and prescalar to 256
	sts		TCCR1B, mpr

	; initialize tcnt value and write it to TCNT1H, then TCNT1L; use 18661 from calculations using prescalar of 256
	ldi		mpr, high(18661)
	sts		TCNT1H, mpr
	ldi		mpr, low(18661)
	sts		TCNT1L, mpr

	; LCD initialization
	rcall	LCDInit			; initialize the LCD screen
	rcall	LCDBacklightOn	; set backlight on

	; set Z register to point to the beginning of the start message string
	; create a pointer from ZL and ZH to the 16 bits of start_string
	ldi		ZL, low(start_string << 1) 
	ldi		ZH, high(start_string << 1)
	
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	; interrupt initialization
	cli						; not needed, but clear interrupt flag 

	ldi		mpr, $00		; ignore all interrupts by masking INT7-0
	out		EIMSK, mpr

	ldi		mpr, 0b00001010 ; set INT1 and INT0 to trigger on falling edges
	sts		EICRA, mpr

	ldi		hand, 0			; set hand to 0 (initial state)
	ldi		opp, 0			; set opponent hand to 0 (initial state)
	ldi		mpr, 0			; set the gamephase to 0 (game state state)
	mov		gamephase, mpr	
	
	; USART1 initalization; use the following parameters: 
	; double data speed, so UBRR = 416
	; Rx interrupt enable
	; Rx and Tx enable
	; set to asynchronous mode
	; disable parity bits
	; set two stop bits
	; set frame size to 8 bits

	ldi		mpr, high(416)	; set UBRR1H to high 8 bits of 416, which is the value required to get a baud rate of 2400 bps at double transmission speed
	sts		UBRR1H, mpr
	ldi		mpr, low(416)	; set UBRR1L to low 8 bits of 416, which is the value required to get a baud rate of 2400 bps at double transmission speed
	sts		UBRR1L, mpr

	ldi		mpr, 0b00000010	; set double transmission speed
	sts		UCSR1A, mpr
	ldi		mpr, 0b10011000 ; set Rx interrupt enable, Rx and Tx enable, and 8 bit frame
	sts		UCSR1B, mpr
	ldi		mpr, 0b00001110 ; set USART async mode, disable parity bits, 2 stop bits, and 8 bit frame
	sts		UCSR1C, mpr

	ldi		mpr, 0b00000010 ; set EIMSK to only act on the INT1 (PD7) interrupt (we do not want PD4 inputs to be registered upon the game start part)
	out		EIMSK, mpr

	; Enable global interrupts 
	sei
	

;***********************************************************
;*  Main Program
;***********************************************************
MAIN:
	
	; check if we have already sent the sendready signal or not
	mov		mpr, gamephase	; copy gamephase contents to mpr
	cpi		mpr, 1			; compare mpr to 1
	brne	MAIN			; loop if we have not sent the signal yet

	; set EIMSK to ignore all interrupts
	ldi		mpr, $00
	out		EIMSK, mpr

	; compare opp with sendready
	cpi		opp, SendReady
	brne	MAIN			; loop until opp is sendready: only break out of this loop once we have received the start signal
	
	; set EIMSK to ignore INT1 interrupts
	ldi		mpr, 0b00000001
	out		EIMSK, mpr

	; after these two loops, it is fair to assume that we can initiate the game start screen.

	; clear the second line of the LCD screen 
	rcall	LCDClrLn2

	; set the game start message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of ready_string
	ldi		ZL, low(game_string << 1) 
	ldi		ZH, high(game_string << 1)

	; call the write functions to display the ready message to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)

	; call the led_delay function; this allows for 7.5 seconds between the game start message and the transmission
	rcall	led_delay

	; after the function returns from led_delay, we now need to transmit the data we got.
	rcall	USART_trans		; call the transmission function

loadstate:
	cpi		opp, 0b11111111
	breq	loadstate
	cpi		opp, 0
	breq	loadstate

	; disable INT0 interrupts (no more changing your hand!)
	ldi		mpr, $00
	out		EIMSK, mpr
	
	; now we need to write the opp value to the LCD screen:
	cpi		opp, rock
	breq	rockwr			; branch to the rockwr section if the opponent's hand is rock

	cpi		opp, paper		
	breq	paperwr			; branch to the paperwr section if the opponent's hand is paper

	cpi		opp, scissors
	breq	scissorswr		; branch to the scissorswr section if the opponent's hand is scissors

rockwr:
	
	; set the rock message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of rock_string
	ldi		ZL, low(rock_string << 1) 
	ldi		ZH, high(rock_string << 1)

	; call the write functions to display the opponent's hand to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rjmp	champ				; jump to the champ section

paperwr:
	
	; set the paper message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of paper_string
	ldi		ZL, low(paper_string << 1) 
	ldi		ZH, high(paper_string << 1)

	; call the write functions to display the opponent's hand to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rjmp	champ				; jump to the champ section

scissorswr:
	
	; set the scissors message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of scissors_string
	ldi		ZL, low(scissors_string << 1) 
	ldi		ZH, high(scissors_string << 1)

	; call the write functions to display the opponent's hand to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rjmp	champ				; jump to the champ section

; this section is the next section:
champ:

	; first have to start the delay again
	rcall	led_delay

	; out of the delay function, need to calculate the winner/loser
	cp		hand, opp			; compare the two hands; if equal, branch to the drawwr section
	breq	drawwr

	cpi		hand, scissors		; check if current hand is 3
	brne	checkone	
	breq	checktwo
	
drawwr:
		
	; set the draw message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of draw_string
	ldi		ZL, low(draw_string << 1) 
	ldi		ZH, high(draw_string << 1)

	; call the write functions to display the ready message to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)

	rjmp	restart			; jump to the last section of the main routine

winwr:

	; set the win message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of win_string
	ldi		ZL, low(win_string << 1) 
	ldi		ZH, high(win_string << 1)

	; call the write functions to display the lose message to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)

	rjmp	restart			; jump to the last section of the main routine

losewr:

	; set the lose message on the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of lose_string
	ldi		ZL, low(lose_string << 1) 
	ldi		ZH, high(lose_string << 1)

	; call the write functions to display the lose message to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)

	rjmp	restart			; jump to the last section of the main routine

checkone:

	; check if opp is 3
	cpi		opp, scissors
	breq	checkthree

	; neither opp nor hand is 3, meaning we can use normal conventions:
	cp		hand, opp		; if hand > opp, then we win. if hand > opp, C flag is cleared in SREG.
	brsh	winwr			; branch to the winwr section if hand > opp

	rjmp	losewr			; branch to the losewr section if hand < opp

checktwo:

	; hand = 3, opp != 3
	; check if opp is 1, which is the only case that we lose
	cpi		opp, rock
	breq	losewr			; if opp = rock, then we lose and we branch to losewr section

	; jump to the win section because in this case hand = scissors and opp = paper
	rjmp	winwr

checkthree:

	; opp = 3 and hand != 3
	; check if hand is 1, which is the only case that we win
	cpi		hand, rock
	breq	winwr			; if hand = rock, then we win and we branch to winwr section

	; jump to the lose section because in this case opp = scissors and hand = paper
	rjmp	losewr

restart:
	
	; call led_delay for the last time
	rcall	led_delay

	; set Z register to point to the beginning of the start message string
	; create a pointer from ZL and ZH to the 16 bits of start_string
	ldi		ZL, low(start_string << 1) 
	ldi		ZH, high(start_string << 1)
	
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	; set gamephase, hand, and opp to 0
	ldi		hand, 0
	ldi		opp, 0
	ldi		mpr, 0		
	mov		gamephase, mpr	

	; set EIMSK to only care about INT1
	ldi		mpr, 0b00000010
	out		EIMSK, mpr
	


	rjmp	MAIN

;***********************************************************
;*	Functions and Subroutines
;***********************************************************

;-----------------------------------------------------------
; Func: USART_recv
; Desc: This function receives data via USART
;-----------------------------------------------------------
USART_recv:
	
	; store the data from UDR1 into the opp register
	lds		opp, UDR1

	reti

;-----------------------------------------------------------
; Func: USART_trans
; Desc: This function transmits data via USART
;-----------------------------------------------------------
USART_trans:

	; load the UCSR1A register into mpr
	lds		mpr, UCSR1A

	; check the UDRE1 bit in the mpr register
	; if the UDRE1 bit is set, then that means that there is no data in the transmit buffer
	sbrs	mpr, UDRE1		; skip the next instruction if there is no data in the transmit buffer

	rjmp	USART_trans		; loop if there is data in the UDR1 register

	; send the hand to the opponent by putting hand into the UDR1 buffer
	sts		UDR1, hand

	; change the gamephase value to 2
	ldi		mpr, 2
	mov		gamephase, mpr

	ret

;-----------------------------------------------------------
; Func: write_top
; Desc: This function moves data from program memory to the register and then to the data memory that points to the top of the LCD screen
; and then the function writes the data to the LCD screen
;-----------------------------------------------------------
write_top:		

	; clear the LCD screen top line
	rcall	LCDClrLn1

	; Initialize the Y register to the first data memory address for the LCD
	; $0100-$010F is the range of addresses for the first/top line of the LCD
	; $0110-$011F is the range of addresses for the second/bottom line of the LCD
	; load the lower 8 bits of the first line of the LCD data memory into YL
	ldi		YL, low($0100) 
	; load the higher 8 bits of the first line of the LCD data memory into YH
	ldi		YH, high($0100)

	; after this, now the Y register points to the same part of memory as the first line of the LCD screen
	; now we need to copy the string from progmem[z] to mpr to datamem[y]
	; start a while loop
	tloop: 
		; load program memory in 8 bit chunks from progmem[z] to mpr, then increment z by 1
		lpm		mpr, Z+ ; post increment Z

		; store mpr to datamem[y], then increment y by 1
		; store indirect command because we are using a pointer as an address rather than a constant register
		st		Y+, mpr ; post increment y

		; now we must check both Y and Z for if they reached their respective ends:

		; compare YL to the low byte of the end memory location of the 1st line of the LCD screen data memory + 1
		cpi		YL, low($0110)

		; branch back into the loop if the SReg (status register) does not have the Z (zero) flag set
		brne tloop

		; compare YH to the high byte of the end memory location of the 1st line of the LCD screen data memory + 1
		cpi		YH, high($0110)

		; branch back into the loop if the SReg does not have the Z flag set
		brne tloop

	; write to the LCD screen
	rcall	LCDWrLn1

	ret						; End a function with RET


;-----------------------------------------------------------
; Func: write_bottom
; Desc: This function moves data from program memory to the register and then to the data memory that points to the bottom of the LCD screen
; and then the function writes the data to the LCD screen
;-----------------------------------------------------------
write_bottom:		

	; clear the LCD screen bottom line
	rcall	LCDClrLn2

	; Initialize the Y register to the first data memory address for the LCD
	; $0100-$010F is the range of addresses for the first/top line of the LCD
	; $0110-$011F is the range of addresses for the second/bottom line of the LCD
	; load the lower 8 bits of the first line of the LCD data memory into YL
	ldi		YL, low($0110) 
	; load the higher 8 bits of the first line of the LCD data memory into YH
	ldi		YH, high($0110)

	; after this, now the Y register points to the same part of memory as the first line of the LCD screen
	; now we need to copy the string from progmem[z] to mpr to datamem[y]
	; start a while loop
	bloop: 
		; load program memory in 8 bit chunks from progmem[z] to mpr, then increment z by 1
		lpm		mpr, Z+ ; post increment Z

		; store mpr to datamem[y], then increment y by 1
		; store indirect command because we are using a pointer as an address rather than a constant register
		st		Y+, mpr ; post increment y

		; now we must check both Y and Z for if they reached their respective ends:

		; compare YL to the low byte of the end memory location of the 2nd line of the LCD screen data memory + 1
		cpi		YL, low($0120)

		; branch back into the loop if the SReg (status register) does not have the Z (zero) flag set
		brne bloop

		; compare YH to the high byte of the end memory location of the 2nd line of the LCD screen data memory + 1
		cpi		YH, high($0120)

		; branch back into the loop if the SReg does not have the Z flag set
		brne bloop

	; write to the LCD screen
	rcall	LCDWrLn2

	ret						; End a function with RET

;-----------------------------------------------------------
; Func: led_delay
; Desc: This function counts the PB7-4 LEDS from 0b1111 to 0b0000 in increments of 1.5 seconds for 7.5 seconds total (4 light stages + 1 off light stage)
;-----------------------------------------------------------
led_delay:	

	push mpr

	; first set PB7:4 to 1111
	ldi		mpr, 0b11110000
	out		PORTB, mpr

	; set leds to 16
	ldi		leds, 16

	; load and store 18661 to TCNT
	ldi		mpr, high(18661)
	sts		TCNT1H, mpr
	ldi		mpr, low(18661)
	sts		TCNT1L, mpr	

	rjmp	wtloop			; jump to wtloop after initializing the counter and leds

cntloop:

	; on the last loop, leds should be 1
	cpi		leds, 1

	; branch to the exit section when leds is equal to 1
	breq	exitloop
	
	; load and store 18661 to TCNT
	ldi		mpr, high(18661)
	sts		TCNT1H, mpr
	ldi		mpr, low(18661)
	sts		TCNT1L, mpr	

	; set mpr to leds
	mov		mpr, leds
	dec		mpr

	lsr		leds			; logic right shift of leds (divide leds by 2)
	sub		mpr, leds		; subtract leds from mpr

	; shift mpr 4 times to the left 
	lsl		mpr
	lsl		mpr
	lsl		mpr
	lsl		mpr

	; write mpr to PORTB after left shifts
	out		PORTB, mpr

wtloop:

	; check for TOV1 flag
	in		mpr, TIFR1		; copy TIFR1 register to mpr

	; mask out the unimportant bits in mpr
	andi	mpr, 0b00000001

	; the above instruction sets the Z flag to 1 if TOV1 flag is not set

	; loop in wtloop if TOV1 flag is not set
	breq	wtloop			; loop if Z flag is set

	; clear the TOV1 flag by writing a one to its bit location
	ldi		mpr, 1			; set mpr to 1
	out		TIFR1, mpr		; set TIFR1 to the modified mpr value

	; jump to cntloop if TOV1 flag is set
	rjmp	cntloop

exitloop:

	pop mpr

	ret
	
;-----------------------------------------------------------
; Func: change_hand
; Desc: This function changes the hand from its current state to the next state and writes the changed hand to the LCD screen
;-----------------------------------------------------------
change_hand:	

	; push mpr and sreg to the stack
	push	mpr
	in		mpr, sreg
	push	mpr

	ldi		waitcnt, Wtime	; set the waitcnt register for waiting later
	
	; check if the hand is in its initial state (0) 
	cpi		hand, 0
	brne	hand_is_rock	; branch to the next section if the hand is not in its default state 

	ldi		hand, rock		; set hand to 1 (rock)

	; create a pointer from ZL and ZH to the 16 bits of rock_string
	ldi		ZL, low(rock_string << 1) 
	ldi		ZH, high(rock_string << 1)

	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	rcall	Wait			; call wait function to prevent button bouncing

	; clear queued interrupts
	ldi		mpr, 0b00000011
	out		EIFR, mpr

	pop		mpr
	out		sreg, mpr
	pop		mpr

	reti

hand_is_rock:
	
	; check if the hand is in its first state (1, rock)
	cpi		hand, rock
	brne	hand_is_paper	; branch to the next section if the hand is not 1 (rock)

	ldi		hand, paper		; set hand to 2 (paper)

	; create a pointer from ZL and ZH to the 16 bits of paper_string
	ldi		ZL, low(paper_string << 1) 
	ldi		ZH, high(paper_string << 1)

	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	rcall	Wait			; call the wait function to prevent button bouncing

	; clear queued interrupts
	ldi		mpr, 0b00000011
	out		EIFR, mpr

	pop		mpr
	out		sreg, mpr
	pop		mpr

	reti

hand_is_paper:
	
	; check if the hand is in its second state (2, paper)
	cpi		hand, paper
	brne	hand_is_scissors; branch to the next section if the hand is not 2 (paper)

	ldi		hand, scissors		; set hand to 3 (scissors)

	; create a pointer from ZL and ZH to the 16 bits of scissors_string
	ldi		ZL, low(scissors_string << 1) 
	ldi		ZH, high(scissors_string << 1)

	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	rcall	Wait			; call the wait function to prevent button bouncing

	; clear queued interrupts
	ldi		mpr, 0b00000011
	out		EIFR, mpr

	pop		mpr
	out		sreg, mpr
	pop		mpr

	reti

hand_is_scissors:
	
	ldi		hand, rock		; set hand to 1 (rock)

	; create a pointer from ZL and ZH to the 16 bits of rock_string
	ldi		ZL, low(rock_string << 1) 
	ldi		ZH, high(rock_string << 1)
	
	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

	rcall	Wait			; call the wait function to prevent button bouncing

	; clear queued interrupts
	ldi		mpr, 0b00000011
	out		EIFR, mpr

	pop		mpr
	out		sreg, mpr
	pop		mpr

	reti

;-----------------------------------------------------------
; Func: start_game
; Desc: This function starts the game by changing the screen and stuff
;-----------------------------------------------------------
start_game:	

	; store hand, mpr, and sreg to the stack
	push	hand
	push	mpr
	in		mpr, sreg
	push	mpr

	; load SendReady into hand then transmit
	ldi		hand, SendReady	; send the start signal to the other board
	rcall	USART_trans

	; store a 1 to gamephase
	ldi		mpr, 1
	mov		gamephase, mpr

	; compare opp to the required value to actually start the game
	cpi		opp, SendReady

	; branch to the exit section if the start signal has already been received
	breq	exitsec

	; set the ready. waiting for opponent message to the LCD screen
	; create a pointer from ZL and ZH to the 16 bits of ready_string
	ldi		ZL, low(ready_string << 1) 
	ldi		ZH, high(ready_string << 1)

	; call the write functions to display the ready message to the LCD screen
	rcall	write_top		; load (and write) the top string into the LCD screen memory (from program memory to data memory of the LCD)
	rcall	write_bottom	; load (and write) the bottom string into the LCD screen memory (from program memory to data memory of the LCD)

exitsec:

	; clear queued interrupts
	ldi		mpr, 0b00000011
	out		EIFR, mpr

	pop		mpr
	out		sreg, mpr
	pop		mpr
	pop		hand

	reti


; this function was copied from Lab 1
;----------------------------------------------------------------
; Sub:	Wait
; Desc:	A wait loop that is 16 + 159975*waitcnt cycles or roughly
;		waitcnt*10ms.  Just initialize wait for the specific amount
;		of time in 10ms intervals. Here is the general eqaution
;		for the number of clock cycles in the wait loop:
;			(((((3*ilcnt)-1+4)*olcnt)-1+4)*waitcnt)-1+16
;----------------------------------------------------------------
Wait:
		push	waitcnt			; Save wait register
		push	ilcnt			; Save ilcnt register
		push	olcnt			; Save olcnt register

Loop:	ldi		olcnt, 224/2		; load olcnt register
OLoop:	ldi		ilcnt, 237		; load ilcnt register
ILoop:	dec		ilcnt			; decrement ilcnt
		brne	ILoop			; Continue Inner Loop
		dec		olcnt		; decrement olcnt
		brne	OLoop			; Continue Outer Loop
		dec		waitcnt		; Decrement wait
		brne	Loop			; Continue Wait loop

		pop		olcnt		; Restore olcnt register
		pop		ilcnt		; Restore ilcnt register
		pop		waitcnt		; Restore wait register
		ret				; Return from subroutine


;***********************************************************
;*	Stored Program Data
;***********************************************************

;-----------------------------------------------------------
; An example of storing a string. Note the labels before and
; after the .DB directive; these can help to access the data
;-----------------------------------------------------------

start_string:
	.db		"Welcome!        "
	.db		"Please press PD7"

ready_string:
	.db		"READY. Waiting  "
	.db		"for the opponent"

win_string:
	.db		"You won!        "

lose_string:
	.db		"You Lost        "

draw_string:
	.db		"Draw            "

game_string:
	.db		"GAME START      "

rock_string:
	.db		"Rock            "

paper_string:
	.db		"Paper           "

scissors_string:
	.db		"Scissors        "

;***********************************************************
;*	Additional Program Includes
;***********************************************************

.include "LCDDriver.asm"		; Include the LCD Driver



