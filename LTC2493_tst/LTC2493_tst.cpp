/** 
* program to test the LTC24  class 
* for more info see the README.MD of the repository 
* https://github.com/wimbeaumont/peripheral_dev_tst.git
* copied from the mbed online compiler envionment 
* ver 0.1  copied form AD101xx test
* ver 0.5  reading with rawADC single ended works , ( lib ltc2493  ver 0.5 )
*          on Raspberry Pi 
* (C) Wim Beaumont Universiteit Antwerpen  2019 , 2024 
* License see
* https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/


#ifdef __MBED__ 
#include "mbed.h"
#endif


#define LTC2493_test_ver  "0.5"


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

//next function works for Raspberry pi , only for debugging . 
// remove it for controller ( mbed . picco) 
// is copied from LTC2493.cpp 
void decode_config(unsigned int config, int outp=1){

if ( outp) {
    printf("config 	%x \n\r",config);
    printf("CONFIG_EN =  %d \n\r", (bool) (CONFIG_EN & config));
    printf("CONFIG_SGL=  %d \n\r", (bool) (CONFIG_SGL & config));
    printf("CONFIG_ODD=  %d \n\r", (bool) (CONFIG_ODD & config));
    printf("CONFIG_A0 =  %d \n\r", (bool) (CONFIG_A0  & config));
    printf("CONFIG_EN2=  %d \n\r", (bool) (CONFIG_EN2 & config));
    printf("CONFIG_IM =  %d \n\r", (bool) (CONFIG_IM & config));
    printf("CONFIG_FA =  %d \n\r", (bool) (CONFIG_FA & config));
    printf("CONFIG_FB =  %d \n\r", (bool) (CONFIG_FB  & config));	
    printf("CONFIG_SPD=  %d \n\r", (bool) (CONFIG_SPD & config));	
}
else {
    int ch=5;
    int diff=8;
    // next doesn't check Temperature
    unsigned int ch_config = config &0b0001100100000000;
    switch (ch_config) {
        //0b10ESO21OEIABS000
        case 0b0001100000000000 : diff=0;ch=1; break;
        case 0b0001100100000000 : diff=0;ch=3; break;
        case 0b0001000100000000 : diff=0;ch=2; break;
        case 0b0001000000000000 : diff=0;ch=0; break;
        case 0b0000100000000000 : diff=2;ch=0; break;
        case 0b0000100100000000 : diff=2;ch=1; break;
        case 0b0000000100000000 : diff=1;ch=1; break;
        case 0b0000000000000000 : diff=1;ch=0; break;
    }
    printf(" diff = %d, ch =%d \n\r", diff,ch);
	
}        
}


int main(void){
    #ifdef __PICO__    
       stdio_init_all();// pico init 
	#endif 
    i2cdev->frequency(10000); 
    
    int adcv=0;
    unsigned int adcraw;
    //float voltage;
    int addr= 0x28; //8 bits address  assume CAO , CA1 are connected to ground 
    printf("test program for the LTC2493  lib , version %s compiled at %s %s  \n\r",
		LTC2493_test_ver , __DATE__ , __TIME__ );
    LTC2493  adc(i2cdev, addr,  Vdd);  
    int status=adc.get_status();
    if (status) printf(" LTC2493 returns %d after init\n\r",status);
    else {
        printf(" LTC2493  init success\n\r");
        decode_config(adc.get_config());
    }
        
        
        
        
    printf("LTC2493 lib  %s \n\r",  adc.getversioninfo());
        
    while (true) {
        int ch=3;
        
        for ( ch =0 ; ch < 4 ;ch++) { // assumes not diff setting 
            i2cdev->wait_for_ms(200);
            //status = adc.getADCvalueCCh( adcraw);
            status = adc.getRawADCvalue( adcraw,ch);
            //status = adc.getADCvalue( adcv,ch);
            if( status ) {
                printf( "error reading ADC %d :  %d \n\r",ch, status);
            }  else {
                //printf(" ADC reads %x \n\r",adcv);
                printf("ch %x: %06x |",ch,adcraw);
                //decode_config(adc.get_config(),0);
            }
          
        }
        printf("\n\r");
    /* else {
           //voltage = (float) adcv * Vdd /( float) adc.getFullRange();
           //printf( "ADC value = %d 0x%x calculate voltage %f " ,adcv, adcv, voltage);
           status = adc.getVoltage( voltage);
           printf( "ADC voltage %f\n\r" ,  voltage);
        }
    */
           
    }
}
