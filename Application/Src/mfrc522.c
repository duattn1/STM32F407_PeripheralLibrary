#include "../Include/mfrc522.h"

int doNothing = 0;

void mfrc522PinSelect(uint8_t selectPin, uint8_t resetPin){
	_selectPin = selectPin;
	_resetPin = resetPin;
}

void PCD_Init(void){
	Bool hardReset = False;
	///xxxx
	/* Config _selectPin & _resetPin as OUTPUT*/

	/* DO NOT select the slave yet = set _selectPin to LOW*/ 

	/* Hard reset */
	
	/*  Init SPI: 8-bits data mode*/
	spiInit(SPI2, SPI_MASTER, SPI_CLOCK_DIV_2, SPI_UNIDIRECTION, SPI_FIRST_CLOCK, SPI_CK_TO_0, SPI_8BITS_DATA, SPI_MSB_FIRST);
	
	/* Reset baudrate */
	
	/* Reset ModWidthReg */
}

uint8_t PCD_ReadRegister(PCD_Register reg){
	uint8_t value;
	
	/* Select slave */
	gpioPinReset(GPIOB, _selectPin);

	/* Send the reg over SPI */
	spiSend(SPI2, 0x80 | reg);					// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	/* Read the value back. Send 0 to stop reading */
	//value = SPI.transfer(0);		
	
	/* Deselect slave */
	gpioPinSet(GPIOB, _selectPin);
	return value;
}

void PCD_ReadRegisterValues(PCD_Register reg, uint8_t count, uint8_t *values, uint8_t rxAlign){
	if (count == 0) {
		return;
	}
	//Serial.print(F("Reading ")); 	Serial.print(count); Serial.println(F(" bytes from register."));
	uint8_t address = 0x80 | reg;				// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	uint8_t index = 0;							// Index in values array.
	
	/* Select slave */
	//digitalWrite(_chipSelectPin, LOW);
	
	/* Deselect slave */
	count--;								// One read is performed outside of the loop
	//SPI.transfer(address);					// Tell MFRC522 which address we want to read
	
	if (rxAlign) {		// Only update bit positions rxAlign..7 in values[0]
		// Create bit mask for bit positions rxAlign..7
		uint8_t mask = (0xFF << rxAlign) & 0xFF;
		// Read value and tell that we want to read the same address again.
		uint8_t value = 0;//duat SPI.transfer(address);
		// Apply mask to both current value of values[0] and the new data in value.
		values[0] = (values[0] & ~mask) | (value & mask);
		index++;		
	}
	while (index < count) {
		values[index] = 0;//duat SPI.transfer(address);	// Read value and tell that we want to read the same address again.
		index++;
	}
	values[index] = 0;//duatSPI.transfer(0);			// Read the final uint8_t. Send 0 to stop reading.
	
	/* Deselect slave */
}

void PCD_WriteRegister(PCD_Register reg, uint8_t value){
	/* Select slave */
	gpioPinReset(GPIOB, _selectPin);	

	/* Send the reg over SPI */
	
	/* Send the value over SPI */
	
	/* Deselect slave */
	gpioPinSet(GPIOB, _selectPin);
	/////* Stop SPI */
}

void PCD_WriteRegisterValues(PCD_Register reg, uint8_t count, uint8_t *values){
	/* Select slave */
	//digitalWrite(_chipSelectPin, LOW);	

	/* Send the reg over SPI */
	
	/* Send the values over SPI */
	for (uint8_t index = 0; index < count; index++) {
		//SPI.transfer(values[index]);
	}
	/* Deselect slave */
	
	/////* Stop SPI */
}

void PCD_AntennaOn(void){
	uint8_t value = PCD_ReadRegister(TxControlReg);
	if ((value & 0x03) != 0x03) {
		PCD_WriteRegister(TxControlReg, value | 0x03);
	}
}

void PCD_SetRegisterBitMask(PCD_Register reg, uint8_t mask){
	uint8_t temp;
	temp = PCD_ReadRegister(reg);
	PCD_WriteRegister(reg, temp | mask);			// set bit mask
}

void PCD_ClearRegisterBitMask(PCD_Register reg, uint8_t mask){
	uint8_t temp;
	temp = PCD_ReadRegister(reg);
	PCD_WriteRegister(reg, temp & (~mask));		// clear bit mask
}

StatusCode PCD_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result){
	PCD_WriteRegister(CommandReg, PCD_Idle);		// Stop any active command.
	PCD_WriteRegister(DivIrqReg, 0x04);				// Clear the CRCIRq interrupt request bit
	PCD_WriteRegister(FIFOLevelReg, 0x80);			// FlushBuffer = 1, FIFO initialization
	PCD_WriteRegisterValues(FIFODataReg, length, data);	// Write data to the FIFO
	PCD_WriteRegister(CommandReg, PCD_CalcCRC);		// Start the calculation
	
	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73µs.
	// TODO check/modify for other architectures than Arduino Uno 16bit

	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73us.
	for (uint16_t i = 5000; i > 0; i--) {
		// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		uint8_t readValue = PCD_ReadRegister(DivIrqReg);
		if (readValue & 0x04) {									// CRCIRq bit set - calculation done
			PCD_WriteRegister(CommandReg, PCD_Idle);	// Stop calculating CRC for new content in the FIFO.
			// Transfer the result from the registers to the result buffer
			result[0] = PCD_ReadRegister(CRCResultRegL);
			result[1] = PCD_ReadRegister(CRCResultRegH);
			return STATUS_OK;
		}
	}
	// 89ms passed and nothing happend. Communication with the MFRC522 might be down.
	return STATUS_TIMEOUT;
}

StatusCode PCD_CommunicateWithPICC(uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, Bool checkCRC){
	// Prepare values for BitFramingReg
	uint8_t txLastBits = validBits ? *validBits : 0;
	uint8_t bitFraming = (rxAlign << 4) + txLastBits;		// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
	
	PCD_WriteRegister(CommandReg, PCD_Idle);			// Stop any active command.
	PCD_WriteRegister(ComIrqReg, 0x7F);					// Clear all seven interrupt request bits
	PCD_WriteRegister(FIFOLevelReg, 0x80);				// FlushBuffer = 1, FIFO initialization
	PCD_WriteRegisterValues(FIFODataReg, sendLen, sendData);	// Write sendData to the FIFO
	PCD_WriteRegister(BitFramingReg, bitFraming);		// Bit adjustments
	PCD_WriteRegister(CommandReg, command);				// Execute the command
	if (command == PCD_Transceive) {
		PCD_SetRegisterBitMask(BitFramingReg, 0x80);	// StartSend=1, transmission of data starts
	}
	
	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86µs.
	// TODO check/modify for other architectures than Arduino Uno 16bit
	uint16_t i;
	for (i = 2000; i > 0; i--) {
		uint8_t readValue = PCD_ReadRegister(ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (readValue & waitIRq) {					// One of the interrupts that signal success has been set.
			break;
		}
		if (readValue & 0x01) {						// Timer interrupt - nothing received in 25ms
			return STATUS_TIMEOUT;
		}
	}
	// 35.7ms and nothing happend. Communication with the MFRC522 might be down.
	if (i == 0) {
		return STATUS_TIMEOUT;
	}
	
	// Stop now if any errors except collisions were detected.
	uint8_t errorRegValue = PCD_ReadRegister(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
		return STATUS_ERROR;
	}
	
	uint8_t _validBits = 0;
	
	// If the caller wants data back, get it from the MFRC522.
	if (backData && backLen) {
		uint8_t readValue = PCD_ReadRegister(FIFOLevelReg);	// Number of bytes in the FIFO
		if (readValue > *backLen) {
			return STATUS_NO_ROOM;
		}
		*backLen = readValue;											// Number of bytes returned
		PCD_ReadRegisterValues(FIFODataReg, readValue, backData, rxAlign);	// Get received data from FIFO
		_validBits = PCD_ReadRegister(ControlReg) & 0x07;		// RxLastBits[2:0] indicates the number of valid bits in the last received uint8_t. If this value is 000b, the whole uint8_t is valid.
		if (validBits) {
			*validBits = _validBits;
		}
	}
	
	// Tell about collisions
	if (errorRegValue & 0x08) {		// CollErr
		return STATUS_COLLISION;
	}
	
	// Perform CRC_A validation if requested.
	if (backData && backLen && checkCRC) {
		// In this case a MIFARE Classic NAK is not OK.
		if (*backLen == 1 && _validBits == 4) {
			return STATUS_MIFARE_NACK;
		}
		// We need at least the CRC_A value and all 8 bits of the last uint8_t must be received.
		if (*backLen < 2 || _validBits != 0) {
			return STATUS_CRC_WRONG;
		}
		// Verify CRC_A - do our own calculation and store the control in controlBuffer.
		uint8_t controlBuffer[2];
		StatusCode status = PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
		if (status != STATUS_OK) {
			return status;
		}
		if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
			return STATUS_CRC_WRONG;
		}
	}
	
	return STATUS_OK;
}

StatusCode PCD_TransceiveData(uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, Bool checkCRC){
	uint8_t waitIRq = 0x30;		// RxIRq and IdleIRq
	return PCD_CommunicateWithPICC(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
}

StatusCode PICC_REQA_or_WUPA(uint8_t command, uint8_t *bufferATQA, uint8_t *bufferSize){
	uint8_t validBits;
	StatusCode status;
	
	if (bufferATQA == 0 || *bufferSize < 2) {	// The ATQA response is 2 bytes long.
		return STATUS_NO_ROOM;
	}
	
	PCD_ClearRegisterBitMask(CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.
	validBits = 7;									// For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) uint8_t. TxLastBits = BitFramingReg[2..0]
	status = PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits, 0, False);
	if (status != STATUS_OK) {
		return status;
	}
	if (*bufferSize != 2 || validBits != 0) {		// ATQA must be exactly 16 bits.
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

StatusCode PICC_RequestA(uint8_t *bufferATQA, uint8_t *bufferSize){
	
	return PICC_REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
}

Bool PICC_IsNewCardPresent(void){
	Bool isCardPresent = False;
	uint8_t bufferATQA[2];
	uint8_t bufferSize = sizeof(bufferATQA);

	/* Reset baudrate */
	PCD_WriteRegister(TxModeReg, 0x00);
	PCD_WriteRegister(RxModeReg, 0x00);
	// Reset ModWidthReg
	PCD_WriteRegister(ModWidthReg, 0x26);

	StatusCode result = PICC_RequestA(bufferATQA, &bufferSize);
	
	if(result == STATUS_OK || result == STATUS_COLLISION)
		isCardPresent = True;
	return isCardPresent;
}

static PICC_Type PICC_GetType(uint8_t sak){
	// http://www.nxp.com/documents/application_note/AN10833.pdf 
	// 3.2 Coding of Select Acknowledge (SAK)
	// ignore 8-bit (iso14443 starts with LSBit = bit 1)
	// fixes wrong type for manufacturer Infineon (http://nfc-tools.org/index.php?title=ISO14443A)
	sak &= 0x7F;
	switch (sak) {
		case 0x04:	return PICC_TYPE_NOT_COMPLETE;	// UID not complete
		case 0x09:	return PICC_TYPE_MIFARE_MINI;
		case 0x08:	return PICC_TYPE_MIFARE_1K;
		case 0x18:	return PICC_TYPE_MIFARE_4K;
		case 0x00:	return PICC_TYPE_MIFARE_UL;
		case 0x10:
		case 0x11:	return PICC_TYPE_MIFARE_PLUS;
		case 0x01:	return PICC_TYPE_TNP3XXX;
		case 0x20:	return PICC_TYPE_ISO_14443_4;
		case 0x40:	return PICC_TYPE_ISO_18092;
		default:	return PICC_TYPE_UNKNOWN;
	}
}

StatusCode PICC_HaltA(void){
	StatusCode result;
	uint8_t buffer[4];
	
	// Build command buffer
	buffer[0] = PICC_CMD_HLTA;
	buffer[1] = 0;
	// Calculate CRC_A
	result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
	if (result != STATUS_OK) {
		return result;
	}
	
	// Send the command.
	// The standard says:
	//		If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
	//		HLTA command, this response shall be interpreted as 'not acknowledge'.
	// We interpret that this way: Only STATUS_TIMEOUT is a success.
	result = 0;//duatPCD_TransceiveData(buffer, sizeof(buffer), NULL, 0);
	if (result == STATUS_TIMEOUT) {
		return STATUS_OK;
	}
	if (result == STATUS_OK) { // That is ironically NOT ok in this case ;-)
		return STATUS_ERROR;
	}
	return result;
}

void PCD_StopCrypto1(void){
	// Clear MFCrypto1On bit
	PCD_ClearRegisterBitMask(Status2Reg, 0x08); // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved MFCrypto1On ModemState[2:0]
}

StatusCode MIFARE_Read(uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize){
	StatusCode result;
	
	// Sanity check
	if (buffer == NULL || *bufferSize < 18) {
		return STATUS_NO_ROOM;
	}
	
	// Build command buffer
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddr;
	// Calculate CRC_A
	result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
	if (result != STATUS_OK) {
		return result;
	}
	
	// Transmit the buffer and receive the response, validate CRC_A.
	return PCD_TransceiveData(buffer, 4, buffer, bufferSize, NULL, 0, True);
}

StatusCode PCD_Authenticate(uint8_t command, uint8_t blockAddr, MIFARE_Key *key, Uid *uid){
	uint8_t waitIRq = 0x10;		// IdleIRq
	
	// Build command buffer
	uint8_t sendData[12];
	sendData[0] = command;
	sendData[1] = blockAddr;
	for (uint8_t i = 0; i < MF_KEY_SIZE; i++) {	// 6 key bytes
		sendData[2+i] = key->keyByte[i];
	}
	// Use the last uid bytes as specified in http://cache.nxp.com/documents/application_note/AN10927.pdf
	// section 3.2.5 "MIFARE Classic Authentication".
	// The only missed case is the MF1Sxxxx shortcut activation,
	// but it requires cascade tag (CT) byte, that is not part of uid.
	for (uint8_t i = 0; i < 4; i++) {				// The last 4 bytes of the UID
		sendData[8+i] = uid->uidByte[i+uid->size-4];
	}
	
	// Start the authentication.
	return PCD_CommunicateWithPICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData), NULL, NULL, NULL, 0, False);
}

void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, uint8_t sector){
	StatusCode status;
	uint8_t firstBlock;		// Address of lowest address to dump actually last block dumped)
	uint8_t no_of_blocks;		// Number of blocks in sector
	Bool isSectorTrailer;	// Set to true while handling the "last" (ie highest address) in the sector.
	
	// The access bits are stored in a peculiar fashion.
	// There are four groups:
	//		g[3]	Access bits for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)
	//		g[2]	Access bits for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
	//		g[1]	Access bits for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
	//		g[0]	Access bits for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
	// Each group has access bits [C1 C2 C3]. In this code C1 is MSB and C3 is LSB.
	// The four CX bits are stored together in a nible cx and an inverted nible cx_.
	uint8_t c1, c2, c3;		// Nibbles
	uint8_t c1_, c2_, c3_;		// Inverted nibbles
	Bool invertedError;		// True if one of the inverted nibbles did not match
	uint8_t g[4];				// Access bits for each of the four groups.
	uint8_t group;				// 0-3 - active group for access bits
	Bool firstInGroup;		// True for the first block dumped in the group
	
	// Determine position and size of sector.
	if (sector < 32) { // Sectors 0..31 has 4 blocks each
		no_of_blocks = 4;
		firstBlock = sector * no_of_blocks;
	}
	else if (sector < 40) { // Sectors 32-39 has 16 blocks each
		no_of_blocks = 16;
		firstBlock = 128 + (sector - 32) * no_of_blocks;
	}
	else { // Illegal input, no MIFARE Classic PICC has more than 40 sectors.
		return;
	}
	
	// Dump blocks, highest address first.
	uint8_t uint8_tCount;
	uint8_t buffer[18];
	uint8_t blockAddr;
	isSectorTrailer = True;
	invertedError = False;	// Avoid "unused variable" warning.
	
	for (int8_t blockOffset = no_of_blocks - 1; blockOffset >= 0; blockOffset--) {
		blockAddr = firstBlock + blockOffset;
		// Sector number - only on first line
		if (isSectorTrailer) {
			if(sector < 10)
				doNothing++;//duatSerial.print(F("   ")); // Pad with spaces
			else
				doNothing++;//duatSerial.print(F("  ")); // Pad with spaces
			//duatSerial.print(sector);
			//duatSerial.print(F("   "));
		} else {
			doNothing++;//duatSerial.print(F("       "));
		}
		// Block number
		if(blockAddr < 10)
			doNothing++;//duatSerial.print(F("   ")); // Pad with spaces
		else {
			if(blockAddr < 100)
				doNothing++;//duatSerial.print(F("  ")); // Pad with spaces
			else
				doNothing++;//duatSerial.print(F(" ")); // Pad with spaces
		}
		//duatSerial.print(blockAddr);
		//duatSerial.print(F("  "));
		// Establish encrypted communications before reading the first block
		if (isSectorTrailer) {
			status = PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, firstBlock, key, uid);
			if (status != STATUS_OK) {
				//duatSerial.print(F("PCD_Authenticate() failed: "));
				//duatSerial.println(GetStatusCodeName(status));
				return;
			}
		}
		
		// Read block
		uint8_tCount = sizeof(buffer);
		status = MIFARE_Read(blockAddr, buffer, &uint8_tCount);
		if (status != STATUS_OK) {
			//duatSerial.print(F("MIFARE_Read() failed: "));
			//duatSerial.println(GetStatusCodeName(status));
			continue;
		}
		// Dump data
		for (uint8_t index = 0; index < 16; index++) {
			if(buffer[index] < 0x10)
				doNothing++;//duatSerial.print(F(" 0"));
			else
				doNothing++;//duatSerial.print(F(" "));
			//duatSerial.print(buffer[index], HEX);
			if ((index % 4) == 3) {
				//duatSerial.print(F(" "));
			}
		}
		
		// Parse sector trailer data
		if (isSectorTrailer) {
			c1  = buffer[7] >> 4;
			c2  = buffer[8] & 0xF;
			c3  = buffer[8] >> 4;
			c1_ = buffer[6] & 0xF;
			c2_ = buffer[6] >> 4;
			c3_ = buffer[7] & 0xF;
			invertedError = (c1 != (~c1_ & 0xF)) || (c2 != (~c2_ & 0xF)) || (c3 != (~c3_ & 0xF));
			g[0] = ((c1 & 1) << 2) | ((c2 & 1) << 1) | ((c3 & 1) << 0);
			g[1] = ((c1 & 2) << 1) | ((c2 & 2) << 0) | ((c3 & 2) >> 1);
			g[2] = ((c1 & 4) << 0) | ((c2 & 4) >> 1) | ((c3 & 4) >> 2);
			g[3] = ((c1 & 8) >> 1) | ((c2 & 8) >> 2) | ((c3 & 8) >> 3);
			isSectorTrailer = False;
		}
		// Which access group is this block in?
		if (no_of_blocks == 4) {
			group = blockOffset;
			firstInGroup = True;
		}
		else {
			group = blockOffset / 5;
			firstInGroup = (group == 3) || (group != (blockOffset + 1) / 5);
		}
		if (firstInGroup) {
			// Print access bits
			//duatSerial.print(F(" [ "));
			//duatSerial.print((g[group] >> 2) & 1, DEC); Serial.print(F(" "));
			//duatSerial.print((g[group] >> 1) & 1, DEC); Serial.print(F(" "));
			//duatSerial.print((g[group] >> 0) & 1, DEC);
			//duatSerial.print(F(" ] "));
			if (invertedError) {
				//duatSerial.print(F(" Inverted access bits did not match! "));
			}
		}
		
		if (group != 3 && (g[group] == 1 || g[group] == 6)) { // Not a sector trailer, a value block
			int32_t value = ((int32_t)buffer[3]<<24) | ((int32_t)buffer[2]<<16) | ((int32_t)buffer[1]<<8) | (int32_t)buffer[0];
			//duatSerial.print(F(" Value=0x")); Serial.print(value, HEX);
			//duatSerial.print(F(" Adr=0x")); Serial.print(buffer[12], HEX);
		}
		//duatSerial.println();
	}
	
	return;	
}

void PICC_DumpMifareClassicToSerial(Uid *uid, PICC_Type piccType, MIFARE_Key *key){
	uint8_t no_of_sectors = 0;
	switch (piccType) {
		case PICC_TYPE_MIFARE_MINI:
			// Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.
			no_of_sectors = 5;
			break;
			
		case PICC_TYPE_MIFARE_1K:
			// Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.
			no_of_sectors = 16;
			break;
			
		case PICC_TYPE_MIFARE_4K:
			// Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.
			no_of_sectors = 40;
			break;
			
		default: // Should not happen. Ignore.
			break;
	}
	
	// Dump sectors, highest address first.
	if (no_of_sectors) {
		//duatSerial.println(F("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits"));
		for (int8_t i = no_of_sectors - 1; i >= 0; i--) {
			PICC_DumpMifareClassicSectorToSerial(uid, key, i);
		}
	}
	PICC_HaltA(); // Halt the PICC before stopping the encrypted session.
	PCD_StopCrypto1();
}

void PICC_DumpToSerial(Uid *uid){
	MIFARE_Key key;
	
	// Dump UID, SAK and Type
	//PICC_DumpDetailsToSerial(uid);
	
	// Dump contents
	PICC_Type piccType = PICC_GetType(uid->sak);
	switch (piccType) {
		case PICC_TYPE_MIFARE_MINI:
		case PICC_TYPE_MIFARE_1K:
		case PICC_TYPE_MIFARE_4K:
			// All keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
			for (uint8_t i = 0; i < 6; i++) {
				key.keyByte[i] = 0xFF;
			}
			PICC_DumpMifareClassicToSerial(uid, piccType, &key);
			break;
			
		case PICC_TYPE_MIFARE_UL:
			//duatPICC_DumpMifareUltralightToSerial();
			break;
			
		case PICC_TYPE_ISO_14443_4:
		case PICC_TYPE_MIFARE_DESFIRE:
		case PICC_TYPE_ISO_18092:
		case PICC_TYPE_MIFARE_PLUS:
		case PICC_TYPE_TNP3XXX:
			//duatSerial.println(F("Dumping memory contents not implemented for that PICC type."));
			break;
			
		case PICC_TYPE_UNKNOWN:
		case PICC_TYPE_NOT_COMPLETE:
		default:
			break; // No memory dump here
	}
	
	//duatSerial.println();
	PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
}

