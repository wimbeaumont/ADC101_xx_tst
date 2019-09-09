/** 
*  program to test the MCP4725 class 
*  for more info see the README.MD of the repository 
*  https://github.com/wimbeaumont/peripheral_dev_tst.git
*  copied from the mbed online compiler envionment 
*  ver 0.3  compiled in mbed-cli envionment for the KL025Z 
*  ver 0.4  version to also be used outside mbed removed the LED controls 
*  ver 0.50 version for Linux / Raspberry Pi   
*  (C) Wim Beaumont Universiteit Antwerpen  2019
*  License see
*  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/

#define MCP4725test_ver  "0.50"

#include "DACInterface.h" 
#include "dev_interface_def.h"

#include "mcp4725.h"



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

Serial pc(USBTX, USBRX);

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




const float Vdd=4.5;





int main(void) {
 
    MCP4725  dac(i2cdev, 1,  Vdd);    

     printf("test program for the MCP4725  lib , version %s compiled at %s %s  \n\r", MCP4725test_ver, __DATE__ , __TIME__ );
    printf("MCP4725 lib  %s \n\r",  dac.getversioninfo());


    int dacv=0;
    while (true) {
        printf("give dac value: ");
        scanf("%d",&dacv);
        dac.setDACvalue(dacv);     

        i2cdev->wait_for_ms(900);

    }
}
