/** 
*  program to test the ADC101_xx  class 
*  for more info see the README.MD of the repository 
*  https://github.com/wimbeaumont/peripheral_dev_tst.git
*  copied from the mbed online compiler envionment 
*  ver 0.3  compiled in mbed-cli envionment for the KL025Z 
*
* (C) Wim Beaumont Universiteit Antwerpen  2019
**/




#include "mbed.h"

#define ADC101_xx_test_ver  "0.3"

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

#include "I2C.h"
#include "I2CInterface.h" 
#include "MBEDI2CInterface.h" 
#include "DACInterface.h" 
#include "dev_interface_def.h"

#include "ADC101_xx.h"

MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
I2CInterface* i2cdev= mbedi2cp;
const float Vdd=3.293;



Serial pc(USBTX, USBRX);






int main(void)
{
    
    PwmOut rled(LED1);
    PwmOut gled(LED2);
    PwmOut bled(LED3);
    i2cdev->frequency(400000); 
    ADC101_xx adc(i2cdev, 0,  Vdd);    
    int status;
    float x=0;
    int adcv=0;
    float voltage;
    
    printf("test program for the ADC101_xx  lib , version %s compiled at %s %s  \n\r",ADC101_xx_test_ver , __DATE__ , __TIME__ );
    printf("ADC101_xx lib  %s \n\r",  adc.getversioninfo());
        
    while (true) {
        
        x=x+.1;
        rled = .4*x;
        gled = .3*x;
        bled = .2*x;
        wait(0.9);
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
