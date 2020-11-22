/** 
* program to test the ADS1x1x  class 
* for more info see the README.MD of the repository 
* https://github.com/wimbeaumont/peripheral_dev_tst.git
* copied from the mbed online compiler envionment 
* ver 0.1  copied from ADC_101 
* ver 0.7 checked with ADC1515  
* ver 0.8  introduced the MBED DAC , for other hardware it is a dummy or you can replace the DAQ inter
* (C) Wim Beaumont Universiteit Antwerpen  2019 ,2020
* License see
* https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/


#ifdef __MBED__ 
#include "mbed.h"
#endif


#define ADS1x1x_test_ver  "0.74"



#include "dev_interface_def.h"

#include "ADS1x1x.h"


// OS / platform  specific  configs 
#if defined  __MBED__ 
#define  OS_SELECT "MBED" 

#include "mbed.h"

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
#include "DAC_MBED.h"
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
DAC_MBED mbeddac( PTE30); 
DAC_MBED* dacp=&mbeddac ;
//------------------ end MBED specific config
#elif defined __LINUX__

#define  OS_SELECT "linux_i2c" 

#include <cstdio>
#include <cstdlib>
#include "LinuxI2CInterface.h"
#include "DacInterface.h"

char *filename = (char*)"/dev/i2c-1";  //hard coded for the moment 
LinuxI2CInterface  mbedi2c(filename);
LinuxI2CInterface* mbedi2cp= &mbedi2c;
DACInterface dacdummy();
DACInterface* dac=&dacdummy();
//------------------ end Linux I2C specific config
#else 
#define  OS_SELECT "linux_dummy" 

#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
#include "DacInterface.h"
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
DACInterface dacdummy();
DACInterface* dac=&dacdummy();


#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 



I2CInterface* i2cdev= mbedi2cp;



int main(void){
    
    i2cdev->frequency(400000); 
    ADS1x1x adc(i2cdev,  0,   1515,1, 0);
    int status;
    int adcv=0;
    float voltage;
    
    printf("test program for the ADS1x1x  lib , version %s compiled at %s %s  \n\r",
		ADS1x1x_test_ver , __DATE__ , __TIME__ );
    printf("ADS1x1x lib  %s \n\r",  adc.getversioninfo());
    float  dacv=0;
    float  dacstep=.2;
    float  dacmax=3.3;    
    while (true) {
        
      
        i2cdev->wait_for_ms(900);
        dacp->setVoltage( dacv); 
        printf("Vout %f ",dacv);
	for (int ch =0; ch < 4; ch++) {
	        status = adc.getADCvalue( adcv,ch);
        	if( status ) {
	            printf( "error reading ADC %d CH %d \n\r", status,ch );
        	} else {
	           printf( "ADC%d = 0x%04x ",ch , adcv);
	           status = adc.getVoltage( voltage,ch );
	           printf( "%fV " ,voltage);
        	}
	}        	
        printf("\n\r") ;
        dacv+=dacstep;
        if ( dacv > dacmax) dacv=0;
           
    }
}
