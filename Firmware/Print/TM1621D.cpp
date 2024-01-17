/**
 * \file TM1621D.cpp
 * \brief Implementation of a class for dealing with the Holtek TM1621D chip.
 * \author Enrico Formenti
 * \date 31 january 2015
 * \version 1.0
 * \copyright BSD license, check the License page on the blog for more information. All this text must be
 *  included in any redistribution.
 *  <br><br>
 *  See macduino.blogspot.com for more details.
 *
 */

#include "TM1621D.h"

void TM1621D::begin()
{
    pinMode(_DATA_pin, OUTPUT);
    pinMode(_RW_pin, OUTPUT);
    pinMode(_CS_pin, OUTPUT);
 
    digitalWrite(_CS_pin, HIGH);
    digitalWrite(_RW_pin, HIGH);
    digitalWrite(_DATA_pin, HIGH);

#ifndef __TM1621D_READ
    register uint8_t i;
    
    for(i=0; i<16; i++)
        ram[i] = 0;
#endif
    
}

// OCIO !!!
// nell'esempio dopo ogni write viene dato un delay
// di 20 microsecondi...
void TM1621D::writeBits(uint8_t data, uint8_t cnt)
{
    register uint8_t i;

    for(i=0;i<cnt;i++,data <<=1)
    {
        digitalWrite(_RW_pin, LOW);
        delayMicroseconds(20);
        digitalWrite(_DATA_pin, data&0x80 ? HIGH : LOW);
        delayMicroseconds(20);
        digitalWrite(_RW_pin, HIGH);
        delayMicroseconds(20);
    }
}

#ifdef __TM1621D_READ

uint8_t TM1621D::readBits(uint8_t cnt)
{
    uint8_t data, i, state;
    
    
    pinMode(_DATA_pin, INPUT);
    
    for(i=0, data=0; i<cnt; data <<= 1, i++) {
        digitalWrite(_RW_pin, LOW);
        delayMicroseconds(20);
        data |= (digitalRead(_DATA_pin) == HIGH);
        digitalWrite(_RW_pin, HIGH);
        delayMicroseconds(20);
    }
    
    pinMode(_DATA_pin, OUTPUT);
    
    return data;
}

#endif

void TM1621D::sendCommand(uint8_t cmd, bool first, bool last)
{
    if (first) {
        TAKE_CS();
        writeBits(COMMAND_MODE, 4);
    }
    
    writeBits(cmd, 8);
    
    if (last)
        RELEASE_CS();
}

void TM1621D::write(uint8_t address, uint8_t data)
{
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6); // 6 is because max address is 128
    writeBits(data, 8);
#ifndef __TM1621D_READ
    ram[address] = data;
#endif
    
    RELEASE_CS();
}

void TM1621D::write(uint8_t address, uint8_t data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++) {
        writeBits(data, 8);
#ifndef __TM1621D_READ
        ram[i] = data;
#endif
    }
    
    RELEASE_CS();
}

void TM1621D::write(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++) {
        writeBits(data[i], 8);
#ifndef __TM1621D_READ
        ram[i] = data[i];
#endif
    }
    
    RELEASE_CS();
}

#ifdef __TM1621D_READ

uint8_t TM1621D::read(uint8_t address)
{
    uint8_t data;
    
    TAKE_CS();
    
    writeBits(READ_MODE, 3);
    writeBits(address<<3, 6);
    data = readBits(8);
    
    RELEASE_CS();
    
    return data;
}




void TM1621D::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(READ_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++)
        data[i] = readBits(8);
    
    RELEASE_CS();
}

#else

uint8_t TM1621D::read(uint8_t address)
{
    return ram[address];
}

void TM1621D::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    for (i = 0; i < cnt; i++)
        data[i] = ram[address+i];
}

#endif
