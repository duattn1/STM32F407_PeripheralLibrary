#include "../Inc/FLASH_driver.h"

uint32_t uwStartSector = 0;
uint32_t uwEndSector = 0;
uint32_t uwAddress = 0;
uint32_t uwSectorCounter = 0;

__IO uint32_t uwData32 = 0;
__IO uint32_t uwMemoryProgramStatus = 0;

uint8_t *utoa(const uint8_t *numbox, uint32_t num, uint32_t base){
    static uint8_t buf[32]={0};
    uint32_t i;
    uint32_t hex_num = sizeof(num) * 2;

    if(num==0){
        buf[30]='0';
        return &buf[30];
    }
    for(i=30;hex_num ; --hex_num,--i, num/=base)
        buf[i] = numbox [num % base];
    return buf+i+1;
}


void USART_printHex(uint32_t num){
    uint8_t *s;
    s = utoa("0123456789ABCDEF", num, 16);
		for(uint8_t i = 0; i < 8; i++, s++){
    sendChar(USART3, *s);
		}
}

void print_data(uint32_t uwStartAddress, uint32_t uwEndAddress)
{
  uint32_t uwAddress = uwStartAddress;
  uint32_t newline_count = 1;

  while (uwAddress < uwEndAddress)
  {
    uwData32 = *(__IO uint32_t*)uwAddress;
    USART_printHex(uwData32);
    if(newline_count %= 4){
			sendChar(USART3, ' ');
		}
    else {
			sendChar(USART3, 13); // \r
			sendChar(USART3, 10); // \n
		}
    uwAddress = uwAddress + 4;
    newline_count ++;
  }
	sendChar(USART3, 13); // \r
	sendChar(USART3, 10); // \n
	sendChar(USART3, 13); // \r
	sendChar(USART3, 10); // \n
}


uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else
  {
    sector = FLASH_Sector_11;  
  }
  return sector;
}

void FLASH_Unlock(void)
{
  if((FLASH->CR & FLASH_CR_LOCK) != 0) //RESET
  {
    /* Authorize the FLASH Registers access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }  
}

void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  FLASH->SR = FLASH_FLAG;
}

void FLASH_OB_Unlock(void)
{
  if((FLASH->OPTCR & FLASH_OPTCR_OPTLOCK) != 0) //RESET
  {
    /* Authorizes the Option Byte register programming */
    FLASH->OPTKEYR = FLASH_OPT_KEY1;
    FLASH->OPTKEYR = FLASH_OPT_KEY2;
  }  
}

FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
    { 
      flashstatus = FLASH_ERROR_WRP;
    }
    else
    {
      if((FLASH->SR & (uint32_t)0xEF) != (uint32_t)0x00)
      {
        flashstatus = FLASH_ERROR_PROGRAM; 
      }
      else
      {
        if((FLASH->SR & FLASH_FLAG_OPERR) != (uint32_t)0x00)
        {
          flashstatus = FLASH_ERROR_OPERATION;
        }
        else
        {
          flashstatus = FLASH_COMPLETE;
        }
      }
    }
  }
  /* Return the FLASH Status */
  return flashstatus;
}



FLASH_Status FLASH_WaitForLastOperation(void)
{ 
  __IO FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the FLASH Status */
  status = FLASH_GetStatus();

  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
  while(status == FLASH_BUSY)
  {
    status = FLASH_GetStatus();
  }
  /* Return the operation status */
  return status;
}

void FLASH_OB_WRP1Config(uint32_t OB_WRP, uint8_t NewState)
{ 
  FLASH_Status status = FLASH_COMPLETE;
    
  status = FLASH_WaitForLastOperation();

  if(status == FLASH_COMPLETE)
  { 
    if(NewState != 0)
    {
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS &= (~OB_WRP);
    }
    else
    {
      *(__IO uint16_t*)OPTCR1_BYTE2_ADDRESS |= (uint16_t)OB_WRP;
    }
  }
}

void FLASH_OB_Lock(void)
{
  /* Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access */
  FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
}


void FLASH_Sector_Erase(void){
	FLASH_Unlock();
	/* Erase */
	//1. Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH_SR register
	if(FLASH->SR & (1 << 16)){
		//there is operation on Flash => quit
		sendString(USART3, "Flash is busy. Skip for now", 27);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
	} else {
		sendString(USART3, "Start erasing", 13);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
		
		//2. Set the SER bit and select the sector out of the 12 sectors (for STM32F405xx/07xx and
		//STM32F415xx/17xx) and out of 24 (for STM32F42xxx and STM32F43xxx) in the main
		//memory block you wish to erase (SNB) in the FLASH_CR register
		FLASH->CR |= 1 << 1; //select setor erase mode
		FLASH->CR |= 11 << 3; //choose sector 11 to delete
		
		//3. Set the STRT bit in the FLASH_CR register
		FLASH->CR |= 1 << 16;
		
		//4. Wait for the BSY bit to be cleared
		while(FLASH->SR & (1 << 16));
		
		sendString(USART3, "Erasing complete", 16);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
	}
}
void FLASH_Write(uint32_t startAddress, uint32_t endAddress){
	uint32_t currentAddress = startAddress;
	FLASH_Unlock();
	//1. Check that no main Flash memory operation is ongoing by checking the BSY bit in the FLASH_SR register
	if(FLASH->SR & (1 << 16)){
		//there is operation on Flash => quit
		sendString(USART3, "Flash is busy. Skip for now", 27);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
	} else {
		sendString(USART3, "Start writing", 13);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
		//2. Set the PG bit in the FLASH_CR register
		FLASH->CR |= 1 << 0;
		
		//3. Perform the data write operation(s) to the desired memory address (inside main memory block or OTP area):
		//– Byte access in case of x8 parallelism
		//– Half-word access in case of x16 parallelism
		//– Word access in case of x32 parallelism
		//– Double word access in case of x64 parallelism
		FLASH->CR |= 2 << 8; //word access
		
		//4. Write the data
		while (currentAddress < endAddress)
  {
    //if ( !FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE)
    //  program_error_count ++ ;
    //uwAddress = uwAddress + 4;
		*(__IO uint32_t*)currentAddress = 0x12343210;
		currentAddress += 4;
  }
		
		
		//5. Wait for the BSY bit to be cleared.
		while(FLASH->SR & (1 << 16));
		
		//6. Disable programming for safe
		FLASH->CR &= ~(1 << 0);
		sendString(USART3, "Writing complete", 16);	
		sendChar(USART3, 13); // \r
		sendChar(USART3, 10); // \n	
	}
}
