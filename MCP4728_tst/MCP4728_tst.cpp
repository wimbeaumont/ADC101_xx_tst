/** example program for the use of the MCP4728 class and DACInterface.h classes
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *
 *  V 1.0  : tested on the KL05z  but without the MCP4728 connected 
 *  v 2.1  : for none MBED environment  and mbed-cli  without setaddr functionality 
 *  v 2.2  : April 2020  need dev_interface_def.h  for type defs 
 * (C) Wim Beaumont Universiteit Antwerpen 2016 2020 
 *  License see
 *  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 
 

#define MCP4728EXAMPLEVER "2.2"
#include "dev_interface_def.h"

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

// different pins can be used for the control of the programming of the MCP4728 

//DigitalOut LDAC(PTA6);
//DigitalOut CntPin(PTA7);

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

//DummyDigitalOut LDAC ;
//DummyDigitalOut CntPin ;


#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 


#include "DACInterface.h" 



// #include "MCP4728setaddr.h"
#include "mcp4728.h"


I2CInterface* i2cdev= mbedi2cp;
const float Vdd=4.5;


int main(void) {
  
   // get the version of getVersion 
   getVersion gv;
   int addr;
   printf("MCP4728 example program version %s, compile date %s time %s\n\r",MCP4728EXAMPLEVER,__DATE__,__TIME__);
   printf("getVersion :%s\n\r ",gv.getversioninfo());
 /**
   printf("\n\rMCP4728 set address program date : %s  time :%s\n\r",__DATE__,__TIME__); 
   printf("give new addr ");
   scanf("%d",&addr);
    if ( addr < 8) {
        LDAC=1;
        char oldaddr, eeaddr, regaddr, newaddr= 2;
        int newaddrint;
        // Set addr part 
        MPC4728_address_set mpc4728(PTB0,&LDAC,i2cdev, &CntPin);
        int i2cresult =mpc4728.readaddress(oldaddr, eeaddr, regaddr);
        if ( i2cresult ) {
            printf("failed to read current address error code: %d , scl cnt = %d  \n", i2cresult,mpc4728.getsclcnt() );
        }
        else { 
            printf( " current address = %d ,eeaddr %d , regaddr %d  sclcnt= %d  give new address :  ", (int) oldaddr,(int) eeaddr, (int)regaddr, mpc4728.getsclcnt());  
            scanf( "%d",&newaddrint);
            printf("\n\r");
            if ( newaddrint >= 0 && newaddrint <= 8 ) {
                newaddr= (char) newaddrint;
                i2cresult=mpc4728.setaddress(eeaddr,newaddr);
                if ( i2cresult ) printf("failed to set address to %d error : %d , sclcnt= %d  \n",newaddr, i2cresult,mpc4728.getsclcnt());
                else {
                     printf("set new  address \n");

                    i2cresult =mpc4728.readaddress(oldaddr, eeaddr, regaddr);

                    if ( i2cresult ) printf("failed to read (new)  address error : %d  \n", i2cresult);
                    else     printf( "new address = %d ,eeaddr %d , regaddr %d  sclcnt= %d  \n\r", (int) oldaddr,(int) eeaddr, (int)regaddr, mpc4728.getsclcnt());  printf( " new address = %d \n", (int) oldaddr);
                }
            }    
            else {
                printf( " new address %d is not valid reset the up to restart the program \n", (int)newaddr );
            }
        }
  } // if add < 16 
 */
  printf("give test addr ");
  scanf("%d",&addr);
 
   MCP4728  dac( i2cdev ,addr, Vdd);  // assuming the address  of the MCP4728 is set to 0 factory default 
   printf("\n\raddr %d MCP4728 :%s\n\r",addr,dac.getversioninfo());
   i2cdev->wait_for_ms(1000);
   float voltage=0;
   int cnt=0;
   while(cnt < 4096){
           // first set the 4 channels 
           for ( int cc =0 ; cc <4 ; cc++) { 
               if ( dac.setDACvalue(cnt,cc) )
                   printf("failed to set dac value %d for channel %d\n\r",cnt,cc);
            }
            printf("set to %d result ",cnt);
               // no else read anyway even if set fails 
            if(dac.update()) printf("\n\rfailed to readback channel info \n\r");
            else {
                for ( int cc =0 ; cc <4 ; cc++) { 
                 (void)dac.getVoltage(voltage,cc);//no need to test done with updat 
                  printf(" CH%d %f[V]",cc,voltage);
                }
                printf("\n\r");
            }
             cnt++;
             cnt=cnt % 4096;     
             i2cdev->wait_for_ms(1200);

  }

  // now the same with the DAC interface 
  DACInterface* di = &dac;
  cnt=0;
  while(1){
           // first set the 4 channels 
           for ( int cc =0 ; cc <4 ; cc++) { 
               if ( di->setDACvalue(cnt,cc) )
                   printf("failed to set dac value %d for channel %d\n\r",cnt,cc);
            }
            printf("set DAC value  to %d result in",cnt);
               // no else read anyway even if set fails 
            if(di->update()) printf("\n\rfailed to readback channel info \n\r");
            else {
                for ( int cc =0 ; cc <4 ; cc++) { 
                 (void)di->getVoltage(voltage,cc);// no need to test done with update
                  printf(" CH%d %f[V]",cc,voltage);
                }
                printf("\n\r");
            }
            
             cnt++;
             cnt=cnt % 4096;     
             i2cdev->wait_for_ms(200);

  }
  
    
    // never reach this   
    return 1;
}   
