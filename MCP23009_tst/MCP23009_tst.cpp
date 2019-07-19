/** 
 *  example program for the use of the MCP23009 class ( IO expander) 
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *  copied from the mbed online compiler envionment 
 *  Not so clear about the test status not sure if this was tested with hardware 
 *  
 *  ver 0.20  : version for mbed-cli / git hub  
 *  ver 0.30  : version for Linux / Raspberry Pi 
 *  (C) Wim Beaumont Universiteit Antwerpen 2017 2019
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define MCP23009_TST_VER  "0.30"

#include "dev_interface_def.h"
#include "mcp23009.h"


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


int main(void) {

    MCP23009   gpio(i2cdev, 0);    
    
    printf("mcp23009 test program %s %s \n\r",__DATE__,__TIME__);
    int pincnt=0;
    int pinv;
    if( gpio.set_as_output(0,0)) {printf("(I2C) error setting pin 0 as ouput\n\r");}
    if( gpio.set_as_input(1,0))  {printf("(I2C) error setting pin 1 as input\n\r");}
    if( gpio.set_as_output(2,0)) {printf("(I2C) error setting pin 2 as ouput\n\r");}
    if( gpio.set_as_output(3,1)) {printf("(I2C) error setting pin 3 as ouput\n\r");}
    if( gpio.set_as_output(4,1)) {printf("(I2C) error setting pin 4 as ouput\n\r");}
    if( gpio.set_as_output(5,1)) {printf("(I2C) error setting pin 5 as ouput\n\r");}
    if( gpio.set_as_output(6,0)) {printf("(I2C) error setting pin 6 as ouput\n\r");}
    if( gpio.set_as_output(7,1)) {printf("(I2C) error setting pin 7 as ouput\n\r");}
    int value;
    while (true) {
        value = gpio.status(pincnt);    
        printf("pinnr : current status  : %d\n\r",value);
        value = gpio.status(8); 
        printf("all pins: current status  : 0x%02X\n\r",value);
        value = gpio.outp_status(8); 
        printf("all pins: current outputstatus  : 0x%02X\n\r",value);
        printf("give  new value for pinnr:%d ",pincnt);
        scanf("%d",&pinv); printf("\n\r");
        
        gpio.set(pincnt,pinv);     
        pincnt++;
       
        i2cdev->wait_for_ms(100);
        if ( pincnt  > 7 ) pincnt=0;
    }
}
