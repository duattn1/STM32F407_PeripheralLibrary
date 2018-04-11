Bootloader support CMDs
- Start communication
-

+ ACK:	0x7C
+ NACK:	0x1F

-----------------------
1. Start communication
			Byte 1		Byte 2
Host:		0xED		checksum
STM32F4: 	-			ACK/NACK
-----------------------
2. Write to memory
Byte		1			2			3			4			5 -> 8				9			[length] byts of data	x		
Host:		0x11		CS			length		CS			address (MSB first)	CS			data bytes				CS		
STM32F4: 	-			ACK/NACK	-			ACK/NACK	-					ACK/NACK	-						ACK/NACK
