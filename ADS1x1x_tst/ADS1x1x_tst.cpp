/** 
* program to test the ADS1x1x  class 
* for more info see the README.MD of the repository 
* https://github.com/wimbeaumont/peripheral_dev_tst.git
* copied from the mbed online compiler envionment 
* ver 0.1  copied from ADC_101 
* ver 0.7 checked with ADC1515  
* ver 0.8  introduced the MBED DAC , for other hardware it is a dummy or you can replace the DAQ inter
* ver 2.0  test version for alert pin functionality
* (C) Wim Beaumont Universiteit Antwerpen  2019 ,2020
* License see
* https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/


#ifdef __MBED__ 
#include "mbed.h"
#endif


#define ADS1x1x_test_ver  "2.02"



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

#include "I2C.h"
#include "MBEDI2CInterface.h"  
#include "DAC_MBED.h"
//#include "DummyDigitalIn.h"
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
DAC_MBED mbeddac( PTE30); 
DAC_MBED* dacp=&mbeddac ;

//DummyDigitalIn alertstat(1);
//DigitalIn alertstat(PTE20);
//------------------ end MBED specific config
#elif defined __LINUX__

#define  OS_SELECT "linux_i2c" 

#include <cstdio>
#include <cstdlib>
#include "LinuxI2CInterface.h"
#include "DACInterface.h"
#include "DummyDigitalIn.h"
DummyDigitalIn alertstat(1);
char *filename = (char*)"/dev/i2c-1";  //hard coded for the moment 
LinuxI2CInterface  mbedi2c(filename);
LinuxI2CInterface* mbedi2cp= &mbedi2c;
DACInterface dacdummy;
DACInterface* dacp = &dacdummy;


//------------------ end Linux I2C specific config
#else 
#define  OS_SELECT "linux_dummy" 

#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
#include "DACInterface.h"
#include "DummyDigitalIn.h"
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
DACInterface dacdummy;
DACInterface* dacp = &dacdummy;
DummyDigitalIn alertstat(1);

#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 





int main(void){
    I2CInterface* i2cdev= mbedi2cp;
#if defined  __MBED__ 	
    // not clear why this can not defined as global. Gives with  MBED OS6 a fatal erro 	
    DigitalIn alertstat(PTE20);  
#endif     
    i2cdev->frequency(400000); 
    ADS1x1x adc(i2cdev,  0,   1515,1, 0);
    int status;
    int adcv=0;
    float voltage;
    
    printf("test program for the ADS1x1x  lib , version %s compiled at %s %s  \n\r",
		ADS1x1x_test_ver , __DATE__ , __TIME__ );
    printf("ADS1x1x lib  %s \n\r",  adc.getversioninfo());
    printf("compiled for  %s \n\r" , OS_SELECT);
    float  dacv=0;
    float  dacstep=.2;
    float  dacmax=3.3;    
    bool  chkalertnot=true;
  while (1) {  
    while (chkalertnot) {
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
	printf("\n\r");
        dacv+=dacstep;
        if ( dacv > dacmax) { dacv=0; } //chkalertnot=false; }
    }
	int ch=3;
	adc.getVoltage( voltage,ch );  // get mux set  to 3 
	dacp->setVoltage( 0); 
        printf("\n\r") ;
 	printf("allert tests \n\r");
 	printf("initial allert status %d  \n\r setup none window \n\r", alertstat.read() );
 	int16_t digv=adc.getDigValue( 2* 3.3/3);
 	int16_t digvl=adc.getDigValue(2* 3.3/3);
 	// normal cmp mode no latch, pol 0
	printf("before configure %d  \n\r", alertstat.read() );
 	adc.setAlertPinMode( 2,(int)digv ); 
 	printf("after configure %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax1 %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0 1 %d  \n\r", alertstat.read() );

  	// normal cmp mode no latch, pol 1, cnt 0 
	adc.setAlertPinMode( 2,digv,0x8000, 0, 1 ); 
 	printf("after configure pol 1 %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
	// normal cmp mode no latch, cnt 2  pol 0
 	adc.setAlertPinMode( 2,digv, 0x8000, 2, 0 ); 
 	printf("after configure cnt2  %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax 1 %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax 2 %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax 3 %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );

	// normal cmp mode latched ,  cnt 0 pol 0
  	adc.setAlertPinMode( 5,digv); 
  	printf("after configure latch mode %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax); 
 	printf("after dacmax %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
	adc.getVoltage( voltage,ch);// reset latch 
  	printf("after adc read %f ,  %d  \n\r",voltage, alertstat.read() );
  	adc.setAlertPinMode( 3,digv,digvl ); // window  mode  none latched ,  cnt 0 pol 0
  	printf("after configure window %d  \n\r", alertstat.read() );
  	dacp->setVoltage( dacmax); 
 	printf("after dacmax %d  \n\r", alertstat.read() );
 	dacp->setVoltage( dacmax/2); 
 	printf("after dacmax/2  %d  \n\r", alertstat.read() );
 	dacp->setVoltage( 0); 
  	printf("after dac 0x %d  \n\r", alertstat.read() );
  	
 
  }
 
}
