/** 
*  program to test the ADC101_xx  class 
*  for more info see the README.MD of the repository 
*  https://github.com/wimbeaumont/peripheral_dev_tst.git
*  copied from the mbed online compiler envionment 
*  ver 0.3  compiled in mbed-cli envionment for the KL025Z 
*  ver 0.4  code for outside mbed environment removed led control
* (C) Wim Beaumont Universiteit Antwerpen  2019
* License see
* https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/


#ifdef MBED 
#include "mbed.h"
#endif


#define ADC101_xx_test_ver  "0.4"


#ifdef MBED 
#if   defined (TARGET_KL25Z) || defined (TARGET_KL46Z)
  PinName const SDA = PTE25;
  PinName const SCL = PTE24;
#elif defined (TARGET_KL05Z)
  PinName const SDA = PTB4;
  PinName const SCL = PTB3;
#elif defined (TARGET_K20D50M)
  PinName const SDA = PTB1;
  PinName const SCL = PTB0;
#else
  #error TARGET NOT DEFINED
#endif
#endif 

#ifdef MBED 
#include "I2C.h"
#include "MBEDI2CInterface.h" 
#else 
#include <cstdio>
#include "DummyI2CInterface.h"
#endif



#include "DACInterface.h" 
#include "dev_interface_def.h"

#include "ADC101_xx.h"

#ifdef MBED 
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
#else 
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
#endif





I2CInterface* i2cdev= mbedi2cp;
const float Vdd=3.293;


#ifdef MBED 
Serial pc(USBTX, USBRX);
#endif





int main(void){
    
    i2cdev->frequency(400000); 
    ADC101_xx adc(i2cdev, 0,  Vdd);    
    int status;
    float x=0;
    int adcv=0;
    float voltage;
    
    printf("test program for the ADC101_xx  lib , version %s compiled at %s %s  \n\r",
		ADC101_xx_test_ver , __DATE__ , __TIME__ );
    printf("ADC101_xx lib  %s \n\r",  adc.getversioninfo());
        
    while (true) {
        
        x=x+.1;
       
        i2cdev->wait_for_ms(900);
        if ( x > 2.5 ) x=0;
        status = adc.getADCvalue( adcv);
        if( status ) {
            printf( "error reading ADC %d \n\r", status);
        } else {
           voltage = (float) adcv * Vdd /( float) adc.getFullRange();
           printf( "ADC value = %d 0x%x calculate voltage %f " ,adcv, adcv, voltage);
           status = adc.getVoltage( voltage);
           printf( "ADC voltage %f\n\r" ,  voltage);
        }
           
    }
}
