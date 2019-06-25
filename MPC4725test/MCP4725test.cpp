/** 
*  program to test the MCP4725 class 
*  for more info see the README.MD of the repository 
*  https://github.com/wimbeaumont/peripheral_dev_tst.git
*  copied from the mbed online compiler envionment 
*  ver 0.3  compiled in mbed-cli envionment for the KL025Z 
*
* (C) Wim Beaumont Universiteit Antwerpen  2019
**/


#include "mbed.h"


#define MCP4725test_ver  "0.3"

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

#include "mcp4725.h"

MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
I2CInterface* i2cdev= mbedi2cp;
const float Vdd=4.5;

Serial pc(USBTX, USBRX);


int main(void)
{
    
    PwmOut rled(LED1);
    PwmOut gled(LED2);
    PwmOut bled(LED3);

    MCP4725  dac(i2cdev, 1,  Vdd);    

     printf("test program for the MCP4725  lib , version %s compiled at %s %s  \n\r", MCP4725test_ver, __DATE__ , __TIME__ );
    printf("MCP4725 lib  %s \n\r",  dac.getversioninfo());

    float x=0;
    int dacv=0;
    while (true) {
        printf("give dac value: ");
        scanf("%d",&dacv);
        dac.setDACvalue(dacv);     
        x=x+.1;
        rled = .4*x;
        gled = .3*x;
        bled = .2*x;
        wait(0.1f);
        if ( x > 2.5 ) x=0;
    }
}
