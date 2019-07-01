# peripheral_dev_tst

[TOC]


test program for the  i2c devices in PerDevices repository
Please check the date of the last commit of this file. 
The links to the data info from the manufactures could be outdated ( my experience is the manufactures change  their websites almost every year or the merge with others ) 

For most of the devices I have a PCB design. Let me know if you are interested.  But of some there also commercial once available. 

A number of these devices are used in one or more experiments of test setups. 
By example the SOLID experiment : http://solid-experiment.org/ 

All the devices use a pointer to an abstract I2C class so it easy to reuse. 

In general I try to give references to code I copied or that I used as base for the development of the code.  But in the heat of getting it ready sometimes I copied code without keeping track where I got it from.  So in case you think you are the original author of some code please let me know. ( via github) . 


## the Devices
Short description of the devices :


###ADC101_xx
 
10 bits ADC  189kSPS  1 CH 
This is for a whole series of ADC's from Texas Instruments. 
It is only tested with adc101c021  . 

### AT30TSE752

This is a cheap temperature sensor with eeprom with a wide voltage range. 
I used the eeprom by example to store calibration values and device ID's .
When I used them ( > 300)  for the SOLID experiment it was still Atmel.  Now it is taken over by Micro chip 
The AT30TSE752 is obsolete but the AT30TSE752A is still available.  
I have problems with locking and /or unlockint the eeprom contents.  It sometimes fails and seems not recoverable ( so if locked , you can not unlock it anymore) 


### HTS221 

A humidity and temperature sensor from st  ( st.com )
https://www.st.com/content/st_com/en/products/mems-and-sensors/humidity-sensors/hts221.html

For the price it is a good sensor , we use it in the SOLID experiment to monitor humidiyt and temperatures on several places. 
Pitty the package can not (at all ?) soldered manual . 

### MCP23009 

I2C IO extender . 5.5 V tollerant 
Micro chip . 
https://www.microchip.com/wwwproducts/en/MCP23009

Not sure I used this device.

### MCP4725 

DAC  12 bits resolution  1 channel  On-Board Non-Volatile Memory (EEPROM)  
web site (micro chip )
https://www.microchip.com/wwwproducts/en/en532229

### MCP4728_tst

DAC 28 bits  4 channels,  On-Board Non-Volatile Memory (EEPROM)   
https://www.microchip.com/wwwproducts/en/en541737
 
The address can be set by programming.  For the MBED on line compiler this is supported but not (yet) for this version as also IO pins and interrupt are needed.
if you want to program the I2C address compile this project
https://os.mbed.com/users/wbeaumont/code/MCP4728test/

Be sure you select the correct pins for the IO.  




## MBED 

These a simple test programs  needs also the repository https://github.com/wimbeaumont/PerDevices

This contains classes for the devices and I2C abstraction. 

the  mbed-cli is used .
MBED os  5.12  ( the latests at this moment)  As I use python 3  older MBED version ( < 5.x )  are not supported  

Assumed is that the __MBED__ variable is set 


### preparations : 

Checked on windows 7

git was already installed 
Installed python3  
installed mercurial 
Mbed_installer_v0.4.10.exe   (mbed-cli ) 

Needed some time to understand the default settings. 
 
Below the description to handle multi projects ( as each test is a project) 

C:\Users\SomeUser\My Documents\mbed_cli   is the projdir
( the location of the project directory is not important as long you have all the access rights) 

in the projdir git clone 

git clone https://github.com/wimbeaumont/PeripheralDevices.git
   
git clone https://github.com/wimbeaumont/peripheral_dev_tst.git

  
In the projdir  :   mbed import mbed-os
  
set the globals  
mbed config -G MBED_OS_DIR mbed-os
 
So the .mbed is now 
( this is in the users home directory so  C:\Users\SomeUser\.mbed ) 
 
---
 
GCC_ARM_PATH=C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin <br>
TOOLCHAIN=GCC_ARM<br>
ARM_PATH=C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin<br>
MBED_OS_DIR=C:\Users\wimb\My Documents\mbed_cli\mbed-os<br>

--- 

### generating the programing files 

now  prepare the mbed project 

cd peripheral_dev_tst 

C:\Users\SomeUser\My Documents\mbed_cli\peripheral_dev_tst>mbed new ADC101_xx_tst -scm none 

(is  mbed new  really needed ?  the --scm none is to avoid the .git is overwriten ) 

now compile for MBED

in projdir

mbed compile -m KL25Z --source peripheral_dev_tst/ADC101_xx_tst --source PeripheralDevices --source mbed-os --build BUILD/ADC101_xx_tst

not tested with the ADC101  but the KL25Z responds correctly 

mbed compile -m KL25Z --source peripheral_dev_tst/MCP4725_tst --source PeripheralDevices --source mbed-os --build BUILD/MCP4725_tst

not tested with the MCP4725  but the KL25Z responds 

As you notice in the last case the is not made a new mbed project.  So not so clear if you have to do it at all as with the paramters and global settings you almost define all needed. 


in the same way :  (most not verified ) 

mbed compile -m KL25Z --source peripheral_dev_tst/HTS221_tst --source PeripheralDevices --source mbed-os --build BUILD/HTS221_tst

mbed compile -m KL25Z --source peripheral_dev_tst/AT30TSE752_tst --source PeripheralDevices --source mbed-os --build BUILD/AT30TSE752_tst

mbed compile -m KL25Z --source peripheral_dev_tst/MCP4728_tst --source PeripheralDevices --source mbed-os --build BUILD/MCP4728_tst

mbed compile -m KL25Z --source peripheral_dev_tst/MCP23009_tst --source PeripheralDevices --source mbed-os --build BUILD/MCP23009_tst

This method will most likely not work for the FRDM-KL05Z as this device is not supported for mbed os > 2.x 
Perhaps it works when using python 2.7 

for more info this is a nice page: 

https://os.mbed.com/docs/mbed-os/v5.12/tools/working-with-mbed-cli.html

## dummy  Linux 

This is a version to compile / develop with a full develop system ( standard linux desktop environment)
Tested with CentOs  7

Uses cmake to generate the test programs

go to the cmake directory<br>
there  $>cmake . <br>
then  $>make  <br> 






## raspberry PI 

try to understand which I2C interface I have to use.  

