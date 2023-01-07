/** 
 *  example program for the use of the hts221  class 
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *  copied from the mbed online compiler envionment 
 *  V 0.1  : initial version for software development ( dummy i2c interface 
 *  V 1.6x :  worked with mbed 
 *  V 1.70  :  check with linux RP
 *  V 1.80 :  added pico support ,not tested with hardware 
 * (C) Wim Beaumont Universiteit Antwerpen 2017 2022
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define VEML7700EXAMPLEVER "1.80"



#include "dev_interface_def.h"
#include "veml7700.h"



// OS / platform  specific  configs 

// OS / platform  specific  configs 
#ifdef __PICO__ 
#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "PicoI2CInterface.h"
PicoI2CInterface mbedi2c;
PicoI2CInterface*  mbedi2cp = &mbedi2c;
#elif  defined  __MBED__ 

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


int main(void) {
#ifdef __PICO__    
       stdio_init_all();// pico init 
#endif 
   
   // get the version of getVersion 
   char dummystr[50];
   getVersion gv;
  int err;
   printf("VEML7700 lux meter    example program version %s,\n\rcompile date %s time %s\n\r", 
        VEML7700EXAMPLEVER,__DATE__,__TIME__);
   printf("getVersion :%s\n\r",gv.getversioninfo());
   gv.dev_interface_def_version(dummystr);
   printf("%s\n\r ",dummystr);
   printf( "i2c dev info:%s\n\r",i2cdev->getversioninfo());
   VEML7700 luxm ( i2cdev);
   printf ( "VEML7700 lib version :%s\n\r ",luxm.getversioninfo());
   err=luxm.get_status( );
   if( err) {
        printf("get error %d after init \n\r", err);
    }
  
    
   luxm.set_integrationtime(1000); // > max  
   float gain=.125;//default 
    bool sd ,ie; u16 reg, pers_protect;int IntT=8; float gainrb;
   while ( 1) {
        int alsbits = luxm.get_als_bits ( );
        err=luxm.get_status( );
        if( err) {
            printf("get error %d after get als init \n\r", err);
        }else {
            if ( alsbits < 0xFF ) {
                  if (gain <    2.0 ) { 
                    gain=gain*2;
                    printf("increase gain to %f \n\r", gain);
                    luxm.set_gain(gain);
                    if( gain > 2.0 ) gain =2.0;
                    
                  } //else max gain 
            }
            if ( alsbits > 0x2FFF) {
                 if( gain > .125 ) {
                        gain = gain/2;
                        printf("decrease gain to %f \n\r", gain);
                        luxm.set_gain(gain); 
                        if (gain <  .125) gain=0.125;
                } // else min gain 
            }
         } // else if err    
        float white = luxm.get_lux (false  );// get the white ch in lux 
        if( err) {
            printf("get error %d after get white init \n\r", err);
        }
        reg = luxm.decode_Reg0(   sd , ie, pers_protect, IntT,  gainrb  );
        err=luxm.get_status( );
        if( err) {
            printf("get error %d after read cmd reg \n\r", err);
        }
        printf( " als %04x , white %f @gain %f reg0 %04x: sd %d, ie %d , P_prot %d, IT %d, G %f \n\r", 
                alsbits, white,gain,reg, (int) sd ,(int) ie,(int) pers_protect, IntT,  gainrb  );
        i2cdev->wait_for_ms(1000);
    }
    return 1;
}   


