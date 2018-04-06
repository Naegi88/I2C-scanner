////////////////////////////////////////
//									  //
//	Silvan Nägeli					  //
//	29.03.2018						  //
//	eeprom / TWI funktionen			  //
//									  //
////////////////////////////////////////

#define EEWrite 0xA0
#define EERead 0xA1
#define SUCCESS 0
#define ERROR 1

//////////////////////////////////////////////////////////////////////////////////////////TWI///////////////////////////////////////////////////////////////////////////
void TWIInit(void)
{
	//set SCL to 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	//enable TWI
	TWCR = (1<<TWEN);
}


void TWIStart(void)
{
	//send start signal 
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
}


void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}


void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}


uint8_t TWIReadACK(void)
{
	//read byte with NACK
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (TWINT)) == 0);
		return TWDR;
}


uint8_t TWIReadNACK(void)
{
	//read byte with NACK
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
		return TWDR;
}


uint8_t TWIGetStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
		return status;
}




//////////////////////////////////////////////////////////////////////////////////////////EEPROM///////////////////////////////////////////////////////////////////////////

uint8_t EEWriteByte ( uint16_t u16addr, uint8_t u8data )
{
	uint8_t addr_l, addr_h;
	addr_l = u16addr;
	addr_h = (u16addr>>8);
	
    TWIStart();  
	TWIWrite(EEWrite); 
	TWIWrite(addr_l);
	TWIWrite(addr_h);
	TWIWrite(u8data);
	TWIStop();
	delay_ms(10);
    return SUCCESS;
}


uint8_t EEReadByte ( uint16_t u16addr )
{
	uint8_t addr_l, addr_h;
	addr_l = u16addr;
	addr_h = (u16addr>>8);
	uint8_t u8data = 0;
	
    
    TWIStart();
	TWIWrite(EEWrite);
	TWIWrite(addr_l);
	TWIWrite(addr_h);
    TWIStart();
	TWIWrite(EERead);
	u8data = TWIReadNACK();
    TWIStop();
	delay_ms(10);
	
    return u8data;
}

/*
uint8_t EEWritePage(uint8_t page, uint8_t u8data)
{
    //calculate page address
    uint8_t u8paddr = 0;
    uint8_t i;
    u8paddr = page<<4;
	
    TWIStart();
    //select page start address and send A2 A1 A0 bits send write command
    TWIWrite(((EEWrite)|(u8paddr>>3))&(~1));
    //send the rest of address
    TWIWrite((u8paddr<<4));
    //write page to eeprom
    for (i=0; i<16; i++)
    {
        TWIWrite(u8data++);
            if (TWIGetStatus() != 0x28)
                return ERROR;
    }
    TWIStop();
    return SUCCESS;
}


uint8_t EEReadPage(uint8_t page, uint8_t u8data)
{
    //calculate page address
    uint8_t u8paddr = 0;
    uint8_t i;
    u8paddr = page<<4;
	
    TWIStart();
    //select page start address and send A2 A1 A0 bits send write command
    TWIWrite(((EERead)|(u8paddr>>3))&(~1));
    //send the rest of address
    TWIWrite((u8paddr<<4));
    //send start
    TWIStart();
    //select devise and send read bit
    TWIWrite(((EERead)|(u8paddr>>3))|1);
    for (i=0; i<15; i++)
    {
        u8data++ = TWIReadACK();
            if (TWIGetStatus() != 0x50)
                return ERROR;
    }  
    u8data = TWIReadNACK();
    TWIStop();
    return SUCCESS;
}*/



void EEWriteWord (uint16_t speiste, uint32_t var32)
{

	uint8_t speicher1 = 0;
	uint8_t speicher2 = 0;
	uint8_t speicher3 = 0;
	uint8_t speicher4 = 0;
	
	speicher1 = var32;
	speicher2 = var32 >> 8;
	speicher3 = var32 >> 16;
	speicher4 = var32 >> 24;
	
	EEWriteByte(speiste, speicher1);
	speiste++;
	EEWriteByte(speiste, speicher2);
	speiste++;
	EEWriteByte(speiste, speicher3);
	speiste++;
	EEWriteByte(speiste, speicher4);
	
}


uint32_t EEReadWord(uint16_t speiste)
{
	uint32_t var32 = 0;
	
	speiste += 3;
	var32 = EEReadByte(speiste);
	var32 <<= 8;
	
	speiste--;
	var32 += EEReadByte(speiste);
	var32 <<= 8;
	
	speiste--;
	var32 += EEReadByte(speiste);
	var32 <<= 8;
	
	speiste--;
	var32 += EEReadByte(speiste);
	
	return var32;
}