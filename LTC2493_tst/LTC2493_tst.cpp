/** 
* program to test the LTC24  class 
* for more info see the README.MD of the repository 
* https://github.com/wimbeaumont/peripheral_dev_tst.git
* copied from the mbed online compiler envionment 
* ver 0.1  copied form AD101xx test
* (C) Wim Beaumont Universiteit Antwerpen  2019 , 2024 
* License see
* https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/


#ifdef __MBED__ 
#include "mbed.h"
#endif


#define LTC2493_test_ver  "0.10"


#include "dev_interface_def.h"
#include "LTC2493.h"


// OS / platform  specific  configs 
// OS / platform  specific  configs 
#ifdef __PICO__ 
#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/i2c.h"
//#include "hardware/timer.h"
//#include "hardware/watchdog.h"
//#include "hardware/clocks.h"
#include "PicoI2CInterface.h"
PicoI2CInterface mbedi2c;
PicoI2CInterface*  mbedi2cp = &mbedi2c;
#elif defined  __MBED__ 
#define  OS_SELECT "MBED" 

#if   defined (TARGET_KL25Z) || defined (TARGET_KL46Z)
  PinName const SDA = PTE0;
  PinName const SCL = PTE1;
#elif defined (TARGET_KL05Z)
  PinName const SDA = PTB4;
  PinName const SCL = PTB3;
#elif defined (TARGET_K20D50M)
  PinName const SDA = PTB1;
  PinName const SCL = PTB0;
#else
  #error TARGET NOT DEFINED
#endif

#if  MBED_MAJOR_VERSION > 5 
BufferedSerial pc(USBTX, USBRX);
#else 
Serial pc(USBTX, USBRX);
#endif 

#include "I2C.h"
#include "MBEDI2CInterface.h"  
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;

//------------------ end MBED specific config
#elif defined __LINUX__

#define  OS_SELECT "linux_i2c" 

#include <cstdio>
#include <cstdlib>
#include "LinuxI2CInterface.h"

char *filename = (char*)"/dev/i2c-1";  //hard coded for the moment 
LinuxI2CInterface  mbedi2c(filename);
LinuxI2CInterface* mbedi2cp= &mbedi2c;

//------------------ end Linux I2C specific config
#else 
#define  OS_SELECT "linux_dummy" 

#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;

#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 



I2CInterface* i2cdev= mbedi2cp;
const float Vdd=5.0;



int main(void){
    #ifdef __PICO__    
       stdio_init_all();// pico init 
	#endif 
    i2cdev->frequency(400000); 
    
    float x=0;
    int adcv=0;
    float voltage;
    
    printf("test program for the LTC2493  lib , version %s compiled at %s %s  \n\r",
		LTC2493_test_ver , __DATE__ , __TIME__ );
    LTC2493  adc(i2cdev, 0,  Vdd);  
    int status=adc.get_status();
    if (status) printf(" LTC2493 returns %d after init\n\r",status);
        
        
    printf("LTC2493 lib  %s \n\r",  adc.getversioninfo());
        
    while (true) {
        
        x=x+.1;
       
        i2cdev->wait_for_ms(900);
        if ( x > 2.5 ) x=0;
        status = adc.getADCvalue( adcv,0);
        if( status ) {
            printf( "error reading ADC %d \n\r", status);
        } else {
           //voltage = (float) adcv * Vdd /( float) adc.getFullRange();
           //printf( "ADC value = %d 0x%x calculate voltage %f " ,adcv, adcv, voltage);
           status = adc.getVoltage( voltage);
           printf( "ADC voltage %f\n\r" ,  voltage);
        }
           
    }
}
