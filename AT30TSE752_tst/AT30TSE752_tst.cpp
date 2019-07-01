/** example program for the use of the AT30TSE7xx class 
 *
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *  ver  0.22  updated to the last mbed , see if mbed studio accepts now
 * 
 *  check addresses connected, read them 
 * (C) Wim Beaumont Universiteit Antwerpen 2017 2019 
 *
 *  ver  0.22  updated to the last mbed , see if mbed studio accepts now
 *  ver  0.30  usage for github and other environments
 *  License see
 *  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define AT30TSE753EXAMPLEVER "0.22"

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
#else  // __MBED__ 
#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
#endif  // __MBED__ 


#include "dev_interface_def.h"
#include "AT30TSE75x.h"


#ifdef __MBED__ 
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;
#else 
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;
#endif


I2CInterface* i2cdev= mbedi2cp;



void print_buf_hex( char *data, int length){
    int nr;
    char *ptr=data;
    for ( int lc=0; lc < length ; lc++){
        nr= (int) *(ptr++);
        printf( "%02x ",nr);
    }
    printf("\n\r");
}        


int main(void) { 

   // get the version of getVersion 
   getVersion gv;
   int addr=0;
   int i2cerr;
   
   printf("AT30TSE752  example program version %s, compile date %s time %s\n\r",AT30TSE753EXAMPLEVER,__DATE__,__TIME__);
   printf("getVersion :%s\n\r ",gv.getversioninfo());
   
   
   
   AT30TSE75x tid[8] ={ AT30TSE75x( i2cdev ,0), AT30TSE75x( i2cdev ,1),AT30TSE75x( i2cdev ,2) ,AT30TSE75x( i2cdev ,3),
                        AT30TSE75x( i2cdev ,4), AT30TSE75x( i2cdev ,5),AT30TSE75x( i2cdev ,6) ,AT30TSE75x( i2cdev ,7)};
   bool addrfound[8];
   for (int lc=0; lc <7 ;lc++) {
        printf ( "AT30SE75x version :%s\n\r ",tid[lc].getversioninfo());
        printf( "Taddr %x , Eaddr %x  subaddr %d\n\r ", tid[lc].getTaddr(),tid[lc].getEaddr(), addr);
        if( tid[lc].getInitStatus() ){ printf("reading config registers failed \n\r");addrfound[lc]=false; } 
        else {
            addrfound[lc]=true;   
            tid[lc].set_resolution(12 , i2cerr );
            tid[lc].set_FaultTollerantQueue('6', i2cerr );
            tid[lc].set_AlertPinPolarity(0,i2cerr);
            tid[lc].set_AlarmThermostateMode(0,i2cerr);
            tid[lc].set_config(i2cerr,0);            
            int configrd= tid[lc].read_config( i2cerr, 0);
            printf( " config %x  I2cerr %d \n\r", configrd,i2cerr ); 
        }
    }
   int pagenr=0;
   const int nrstrs=4;
   char str[nrstrs][16];
    
   while(1) {
    for (int lc=0; lc<7 ;lc++) {
      if( addrfound[lc]) { 
        pagenr=0;
        for ( int sc=0 ; sc <nrstrs;sc++){
            i2cerr=tid[lc].read_eeprompage(str[sc], 16, 0, (uint8_t) pagenr++);
            if(i2cerr) printf("eeprom read error %d addr %d  \n\r",i2cerr,lc);
        }
        
        for (int sc=0 ; sc < nrstrs;sc ++) { 
            str[sc][15]='\0'; //make sure it ends
            printf("%d:%15s ",sc,str[sc]);
        }
        // temperature  correction
        str[3][7]='\0';
        float tempcor=atof(str[3]);
        if ( tempcor < -2.5 || tempcor > 2.5) tempcor=0.0; 
        float Tmp= tid[lc].get_temperature(i2cerr);      
        printf ("T=%f %f I2cerr %d addr %d\n\r", Tmp,Tmp+tempcor, i2cerr,lc);
       } // if
    } // for 
    printf("\n\r");
    i2cdev->wait_for_ms(500);
  } //while
    
}   
