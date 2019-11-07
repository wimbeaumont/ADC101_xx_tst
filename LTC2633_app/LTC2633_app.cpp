/** program to controll the bias and trigger levels for a two channel 
 *  SiPm readout board 
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *
 *  V 1.0  : copied from LTC2633_tst.cpp 
 *  V 1.24  : working with the hardware simple increasing
 * (C) Wim Beaumont Universiteit Antwerpen 2019
 *  License see
 *  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 
 

#define LTC2633_SIPM_CTRL "1.30"


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

//DummyDigitalOut LDAC ;
//DummyDigitalOut CntPin ;


#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 


#include "DACInterface.h" 

#include "dev_interface_def.h"

// #include "LTC2633setaddr.h"
#include "ltc2633.h"


I2CInterface* i2cdev= mbedi2cp;
const float Vrefext=5.0;


int main(void) {
  
   // get the version of getVersion 
   getVersion gv;
   int bias_ctrl_addr= 0x12;  //CAO at 5 Vdd
   int discr_lvl_addr= 0x10;  //CAO at 0 Vdd
   printf("SiPm ctrl version %s, compile date %s time %s for OS %s\n\r",LTC2633_SIPM_CTRL,__DATE__,__TIME__,OS_SELECT);
   printf("getVersion :%s\n\r ",gv.getversioninfo());
   printf("I2C interface version  :%s\n\r ",i2cdev->getversioninfo());
   int Vreftype=1 , resolution=12;
   LTC2633  biasctrl(i2cdev, bias_ctrl_addr,  Vrefext ,Vreftype , resolution  );
   LTC2633  discr_lvl(i2cdev, discr_lvl_addr,  Vrefext ,Vreftype , resolution  );
   printf("\n\raddr %d LTC2633 :%s\n\r", bias_ctrl_addr,biasctrl.getversioninfo());
   i2cdev->wait_for_ms(1000);
      int errcode;
   int cnt=0;
   while(cnt < 4096){
           // first set the 2 channels 
           for ( int cc =0 ; cc <2 ; cc++) { 
               errcode= biasctrl.setDACvalue(cnt,cc);  
               if (errcode )
                   printf("failed to set biasctrl value %d for channel %d errcode %d\n\r",cnt,cc,errcode);
                errcode= discr_lvl.setDACvalue(cnt,cc);
                if ( errcode )
                   printf("failed to set  dsicr lvl value %d for channel%d errcode %d \n\r",cnt,cc,errcode);
            }
            printf("set to %d result\n\r ",cnt);
               // no else read anyway even if set fails 
/*            if(biasctrl.update()) printf("\n\rfailed to readback channel info (should fail not implemented for LT2633  \n\r");
            else {
                for ( int cc =0 ; cc <4 ; cc++) { 
                 (void)biasctrl.getVoltage(voltage,cc);//no need to test done with updat 
                  printf(" CH%d %f[V]",cc,voltage);
                }
                printf("\n\r");
            }
*/             cnt++;
             cnt=cnt % 4096;     
             i2cdev->wait_for_ms(200);

  }

  // now the same with the DAC interface 
  DACInterface* bi = &biasctrl;
  DACInterface* li = &discr_lvl;
  cnt=0;
  while(1){
           // first set the 4 channels 
           for ( int cc =0 ; cc <2 ; cc++) { 
               if ( bi->setDACvalue(cnt,cc) )
                   printf("failed to set dac value %d for channel %d\n\r",cnt,cc);
			   if ( li->setDACvalue(cnt,cc) )
                   printf("failed to set dac value %d for channel %d\n\r",cnt,cc);
            }
            printf("set DAC value  to %d result in",cnt);
               // no else read anyway even if set fails 
/*            if(bi->update()) printf("\n\rfailed to readback channel info \n\r");
            else {
                for ( int cc =0 ; cc <4 ; cc++) { 
                 (void)bi->getVoltage(voltage,cc);// no need to test done with update
                  printf(" CH%d %f[V]",cc,voltage);
                }
                printf("\n\r");
            }
*/            
             cnt++;
             cnt=cnt % 4096;     
             i2cdev->wait_for_ms(200);

  }
  
    
    // never reach this   
    return 1;
}   
