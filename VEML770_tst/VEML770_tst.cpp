/** 
 *  example program for the use of the hts221  class 
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *  copied from the mbed online compiler envionment 
 *  V 0.1  : initial version for software development ( dummy i2c interface 
 *  
 * (C) Wim Beaumont Universiteit Antwerpen 2017 2019
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define VEML770EXAMPLEVER "0.1"

#ifdef __MBED__ 

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
Serial pc(USBTX, USBRX);

#include "I2C.h"
#include "MBEDI2CInterface.h"  

#else 

#include <cstdio>
#include "DummyI2CInterface.h"	

#endif //__MBED__

#include "dev_interface_def.h"
#include "veml770.h"




#ifdef __MBED__ 
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
#else 
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
#endif //__MBED__


I2CInterface* i2cdev= mbedi2cp;


int main(void) {
   
   // get the version of getVersion 
   char dummystr[50];
   getVersion gv;
  
   printf("VEML770 lux meter    example program version %s, compile date %s time %s\n\r", 
	VEML770EXAMPLEVER,__DATE__,__TIME__);
   
   printf("getVersion :%s ",gv.getversioninfo());
   gv.dev_interface_def_version(dummystr);
   printf(" def type version  :%s\n\r ",dummystr);
   VEML770 luxm ( i2cdev);
   printf ( "VEML770 lib version :%s\n\r ",luxm.getversioninfo());

    return 1;
}   


