/* LTC2400 24 Bit ADC Test
* Connect an LTC2400 24 Bit ADC to the Arduino Board in SPI Mode
*
*
*
* KHM 2009 /  Martin Nawrath
* Kunsthochschule fuer Medien Koeln
* Academy of Media Arts Cologne

*/
#include <stdio.h>

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))
#endif


// Connections:
// Arduino pin 13 (SCK)   ->  LTC2400 pin 7 (SCK)
// Arduino pin 12 (MISO)  ->  LTC2400 pin 6 (SDO)
// Arduino pin 10 (SS)    ->  LTC2400 pin 5 (CS)

#define LTC_CS 2         // LTC2400 Chip Select Pin  on Portb 2
#define LTC_MISO  4      // LTC2400 SDO Select Pin  on Portb 4
#define LTC_SCK  5       // LTC2400 SCK Select Pin  on Portb 5

void setup() {

 cbi(PORTB,LTC_SCK);      // LTC2400 SCK low
 sbi(DDRB,LTC_CS);       // LTC2400 CS HIGH

 cbi(DDRB,LTC_MISO);
 sbi(DDRB,LTC_SCK);

 Serial.begin(115200);
 
 // init SPI Hardware
 sbi(SPCR,MSTR) ; // SPI master mode
 sbi(SPCR,SPR0) ; // SPI speed
 sbi(SPCR,SPR1);  // SPI speed
 sbi(SPCR,SPE);   //SPI enable

 Serial.println("Voltage");

}

float volt;
float v_ref = 1.0;

long int ltw = 0;         // ADC Data
int cnt;                  // counter
byte b0;                  //
byte sig;                 // sign bit flag
char st1[20];             // float voltage text


/********************************************************************/
void loop() {

 cbi(PORTB, LTC_CS);             // LTC2400 CS Low
 delayMicroseconds(1);
 
 if (!(PINB & (1 << PB4))) {    // ADC Converter ready ?
   //    cli();
   ltw=0;
   sig=0;

   b0 = SPI_read();             // read 4 bytes adc raw data with SPI
   if ((b0 & 0x20) ==0) sig=1;  // is input negative ?
   b0 &=0x1F;                   // discard bit 25..31
   ltw |= b0;
   ltw <<= 8;
   b0 = SPI_read();
   ltw |= b0;
   ltw <<= 8;
   b0 = SPI_read();
   ltw |= b0;
   ltw <<= 8;
   b0 = SPI_read();
   ltw |= b0;

   delayMicroseconds(1);

   sbi(PORTB, LTC_CS);           // LTC2400 CS hi
   delay(1);

   if (sig) {
    ltw |= 0xf0000000;    // if input negative insert sign bit
   }
   
    ltw = ltw / 16;                    // scale result down , last 4 bits have no information
    volt = ltw * v_ref / 16777216; // max scale

    cnt += 1;
     
    Serial.println(volt, 6);
  }
 
  sbi(PORTB, LTC_CS); // LTC2400 CS hi
  delay(1);
}


/********************************************************************/
byte SPI_read() {
  SPDR = 0;
  while (!(SPSR & (1 << SPIF))) ; /* Wait for SPI shift out done */
  return SPDR;
}


