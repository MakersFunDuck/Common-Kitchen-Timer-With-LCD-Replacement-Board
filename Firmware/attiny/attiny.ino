
#include "TM1621D.h"

#define PIN_TM1621D_BL     PB0
#define PIN_TM1621D_CS     PB1
#define PIN_TM1621D_RW     PB3
#define PIN_TM1621D_DATA   PB4












struct DisplayConfig {
    uint8_t m4b;   // MSB 4 bits
    uint8_t l4b;   // LSB 4 bits
    bool bitHL;    // High bits or Low bits
};

uint8_t displayMemory[18];  // Display Segments Memory

DisplayConfig digMap[18] = { //High, Low
    {11,10,  true}, { 9, 8,  true}, { 7, 6,  true},  { 5, 4,  true}, { 3, 2,  true}, { 1, 0,  true}, // Row 1
    {11,10, false}, { 9, 8, false}, { 7, 6, false},  { 5, 4, false}, { 3, 2, false}, { 1, 0, false}, // Row 2
    {12,13,  true}, {14,15,  true}, {16,17,  true},  {12,13, false}, {14,15, false}, {16,17, false}  // Row 3
};

uint8_t segments[] = {
    //GBDCFAME
    0b01111101,  // 0
    0b01010000,  // 1
    0b11100101,  // 2
    0b11110100,  // 3
    0b11011000,  // 4
    0b10111100,  // 5
    0b10111101,  // 6
    0b01010100,  // 7
    0b11111101,  // 8
    0b11111100,  // 9
    0b00000000   // [Empty]
};

TM1621D ht(PIN_TM1621D_CS, PIN_TM1621D_RW, PIN_TM1621D_DATA);

void writeDigit(uint8_t d, uint8_t num){
    DisplayConfig df = digMap[d];
    writeSegments(d, segments[num]);
}

void writeSegments(uint8_t d, uint8_t segData){
    DisplayConfig df = digMap[d];
    uint8_t high4Bit = segData >> 4;
    uint8_t low4Bit  = segData & 0b1111;
    
    if( df.bitHL ){
        high4Bit = high4Bit << 4;
        low4Bit  = low4Bit << 4;
        displayMemory[df.m4b] = displayMemory[df.m4b] & 0b00001111;
        displayMemory[df.l4b] = displayMemory[df.l4b] & 0b00001111;
        displayMemory[df.m4b] = displayMemory[df.m4b] | high4Bit;
        displayMemory[df.l4b] = displayMemory[df.l4b] | low4Bit;
    }else{
        displayMemory[df.m4b] = displayMemory[df.m4b] & 0b11110000;
        displayMemory[df.l4b] = displayMemory[df.l4b] & 0b11110000;
        displayMemory[df.m4b] = displayMemory[df.m4b] | high4Bit;
        displayMemory[df.l4b] = displayMemory[df.l4b] | low4Bit;
    }
    
    ht.write(df.m4b, displayMemory[df.m4b]);
    ht.write(df.l4b, displayMemory[df.l4b]);
}


void setup() {
  
    uint8_t i;
    pinMode(PIN_TM1621D_BL, OUTPUT);
    digitalWrite(PIN_TM1621D_BL, LOW);

    ht.begin();
    
    ht.sendCommand(TM1621D::RC256K);
    ht.sendCommand(TM1621D::BIAS_THIRD_4_COM);
    ht.sendCommand(TM1621D::SYS_EN);
    ht.sendCommand(TM1621D::LCD_ON);

    //Clear display
    for(i=0; i<18; i++){
        displayMemory[i] = 0;
        ht.write(i,0);
    }
  

    digitalWrite(PIN_TM1621D_BL, HIGH); //Turn on backlight LED
    

    digitalWrite(PIN_TM1621D_BL, LOW); //Turn off backlight LED
    
    // write all segments (even dp)
    for(i=0; i<18; i++){
        ht.write(i, 0xFF);
       
        delay( 2000 );
    }

    
    //Clear display
    for(i=0; i<18; i++){
        displayMemory[i] = 0;
        ht.write(i,0);
    }
}

void loop() {

      for(int i=5; i<9; i++){
      /* 
      ht.write(i, 0b00000001); //e
        delay( 1000 );
        ht.write(i, 0b00000010); //ms
        delay( 1000 );

        ht.write(i, 0b00000100); //a
        delay( 1000 );
        ht.write(i, 0b00001000); //f
        delay( 1000 );
        ht.write(i, 0b00010000); //c
        delay( 1000 );

        ht.write(i, 0b00100000); //d
        delay( 1000 );
        ht.write(i, 0b10000000); // g
        delay( 1000 );
        ht.write(i, 0b01000000); //b
        delay( 1000 );
        */
    for(int J=0; J<10; J++){
       ht.write(i, segments[J]); //b
        delay( 1000 );
        }
    }
}



  

