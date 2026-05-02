#include <avr/io.h>
#include <Arduino.h>

//Inititalize
void initI2C() 
{
    // Wake up TWI (I2C)
    PRR0 &= ~(1 << PRTWI);

    // Set prescaler = 1 → TWPS0 = 0, TWPS1 = 0
    TWSR &= ~(1 << TWPS0);
    TWSR &= ~(1 << TWPS1);

    // Set bit rate → ~100kHz
    TWBR |= 0x48;  // 72 decimal

    // Enable I2C
    TWCR |= (1 << TWEN);
}

//2. Start Condition
void StartI2C_Trans(unsigned char SLA)
{
    // Start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Load SLA + WRITE (0)
    TWDR = (SLA << 1) & 0xFE;

    // Trigger transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

//3. StopI2C_Trans()
void StopI2C_Trans()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

//4. Write(unsigned char data)
void Write(unsigned char data)
{
    TWDR = data;

    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

//5. Read_from(unsigned char SLA, unsigned char address)
uint16_t Read_from(unsigned char SLA, unsigned char address)
{
     uint8_t msb, lsb;

    // Start + send register address
    StartI2C_Trans(SLA);
    Write(address);

    // Repeated START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // SLA + READ
    TWDR = (SLA << 1) | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Read MSB with ACK
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    msb = TWDR;

    // Read LSB with NACK 
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    lsb = TWDR;

    StopI2C_Trans();

    return ((uint16_t)msb << 8) | lsb; 
}

//6. unsigned char Read_data()
unsigned char Read_data()
{
    return TWDR;
}

void Write16_to(unsigned char SLA, unsigned char address, uint16_t value)
{
    StartI2C_Trans(SLA);
    Write(address);
    Write((value >> 8) & 0xFF);  // MSB
    Write(value & 0xFF);         // LSB
    StopI2C_Trans();
}