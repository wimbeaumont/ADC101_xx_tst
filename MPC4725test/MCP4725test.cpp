/** 
*  program to test the MCP4725 class 
*  for more info see the README.MD of the repository 
*  https://github.com/wimbeaumont/peripheral_dev_tst.git
*  copied from the mbed online compiler envionment 
*  ver 0.3  compiled in mbed-cli envionment for the KL025Z 
*  ver 0.4  version to also be used outside mbed removed the LED controls  
*  (C) Wim Beaumont Universiteit Antwerpen  2019
*  License see
*  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
**/

#define MCP4725test_ver  "0.4"

#ifdef __MBED__ 
#include "mbed.h"

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

Serial pc(USBTX, USBRX);

#endif

#ifdef __MBED__ 
#include "I2C.h"
#include "MBEDI2CInterface.h" 
#else 
#include <cstdio>
#include "DummyI2CInterface.h"
#endif

#include "DACInterface.h" 
#include "dev_interface_def.h"

#include "mcp4725.h"


#ifdef __MBED__ 
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
#else 
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
#endif



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
