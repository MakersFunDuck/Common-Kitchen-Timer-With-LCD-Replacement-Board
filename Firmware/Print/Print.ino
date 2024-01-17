
#include "TM1621D.h"

#define PIN_TM1621D_BL     2
#define PIN_TM1621D_CS     13
#define PIN_TM1621D_RW     12
#define PIN_TM1621D_DATA   14









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
    0b00000000,   // [Empty]
    
};



char charToSegBits(char character) {
	switch (character) {
	case '*': // For TM for now
		return 0b00000010;
	case '|':
		return 0b01010000;
	case '-':
		return 0b10000000;
	case '_':
		return 0b00100000;
	case '0':
		return 0b01111101;
	case '1':
		return 0b01010000;
	case '2':
		return 0b11100101;
	case '3':
		return 0b11110100;
	case '4':
		return 0b11011000;
	case '5':
		return 0b10111100;
	case '6':
		return 0b10111101;
	case '7':
		return 0b01010100;
	case '8':
		return 0b11111101;
	case '9':
		return 0b11111100;
	case 'A':
	case 'a':
		return 0b11011101;
	case 'b':
	case 'B':
		return 0b10111001;
	case 'c':
		return 0b10100001;
	case 'C':
		return 0b00101101;
	case 'd':
	case 'D':
		return 0b11110001;
	case 'e':
	//	return 0b0001110;
	case 'E':
		return 0b10101101;
	case 'f':
	//	return 0b0000111;
	case 'F':
		return 0b10001101;
	case 'G':
	case 'g':
		return 0b11111100;
	case 'h':
	//	return 0b1000111;
	case 'H':
		return 0b11011001;
	case 'i':
	//	return 0b1000000;
	case 'I':
		return 0b01010000;
	case 'J':
	case 'j':
		return 0b01110000;
  case 'k':
  case 'K':
		return 0b10011101;  
	case 'l':
	//	return 0b1100000;
	case 'L':
		return 0b00101001;
	case 'm':
	case 'M':
		return 0b00010101;
	case 'n':
	case 'N':
		return 0b01011101;
	case 'O': // we can keep this for zero
		return 0b01111101;
	case 'o':
		return 0b10110001;
	case 'P':
	case 'p':
		return 0b11001101;
	case 'q':
	case 'Q':
		return 0b11111100;
	case 'r':
  	return 0b10000001;
	case 'R':
		return 0b01001101;
	case 'S':
	case 's':
		return 0b10111100;
	case 't':
	case 'T':
		return 0b10101001;
	case 'u':
		return 0b00110001;
	case 'U':
		return 0b01111001;
	case 'Y':
	case 'y':
		return 0b11111000;
	case 'z':
	case 'Z':
		return 0b11100101;
	case ' ':
	default:
		return 0b00000000;
	}
  }
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





void Print(const char* str){


    //Clear display
    for(int i=0; i<18; i++){
        displayMemory[i] = 0;
        ht.write(i,0);
    }

      for(int i=8; i>4; i--){
  
       ht.write(i, charToSegBits(str[8-i])); 
        
        }
delay( 1500 );


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
  


    // write all segments (even dp)
    for(i=0; i<18; i++){
        ht.write(i, 0xFF);
       
        delay( 500 );
    }

    
    //Clear display
    for(i=0; i<18; i++){
        displayMemory[i] = 0;
        ht.write(i,0);
    }




}

void loop() {
/*
      for(int i=5; i<9; i++){
  
       ht.write(i, charToSegBits('d')); //b
        delay( 1000 );
        }
*/
Print("tiny");
Print("13aa");







    }

