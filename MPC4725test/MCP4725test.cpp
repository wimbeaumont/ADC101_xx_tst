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
