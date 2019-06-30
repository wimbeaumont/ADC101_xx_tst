# peripheral_dev_tst

test program for the  i2c devices in PerDevices repository


## MBED 

These a simple test programs  needs also the repository https://github.com/wimbeaumont/PerDevices

This contains classes and I2C abstraction. 
For the moment (june 2019 ) it works only withint an MBED environment  
the  mbed-cli is used .
MBED os  5.12  ( the latests at this moment) 


Checked on windows 7

git was already installed 
Installed python3  
installed mercurial 
Mbed_installer_v0.4.10.exe   (mbed-cli ) 

Needed some time to understand the default settings. 
 
This is handled as a  multi project   

C:\Users\wimb\My Documents\mbed_cli   is the projdir

in the projdir git clone 

git clone https://github.com/wimbeaumont/PeripheralDevices.git
   
git clone https://github.com/wimbeaumont/peripheral_dev_tst.git

  
  In the projdir 

mbed import mbed-os
  
set the globals  
mbed config -G MBED_OS_DIR mbed-os
 
So the .mbed is now 
( this is in the user home directory so in this case C:\Users\wimb\.mbed ) 
 
 
GCC_ARM_PATH=C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin <br>
TOOLCHAIN=GCC_ARM<br>
ARM_PATH=C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin<br>
MBED_OS_DIR=C:\Users\wimb\My Documents\mbed_cli\mbed-os<br>

now  prepare the mbed project 

cd peripheral_dev_tst 

C:\Users\wimb\My Documents\mbed_cli\peripheral_dev_tst>mbed new ADC101_xx_tst -scm none 

(is this really needed ?  the --scm none is to avoid the .git is overwriten ) 

 now compile for MBED

in projdir

mbed compile -m KL25Z --source peripheral_dev_tst/ADC101_xx_tst --source PeripheralDevices --source mbed-os --build BUILD/ADC101_xx_tst

not tested with the ADC101  but the KL25Z responds correctly 

mbed compile -m KL25Z --source peripheral_dev_tst/MPC4725test --source PeripheralDevices --source mbed-os --build BUILD/MCP4725test

not tested with the MPC4725  but the KL25Z responds 


As you notice in the last case the is not made a new mbed project.  So not so clear if you have to do it at all as with the paramters and global settings you almost define all needed. 


This method will most likely not work for the KL02Z as this device is not supported for mbed os > 2.x 
Perhaps it works when using python 2.7 

for more info this is a nice page 

https://os.mbed.com/docs/mbed-os/v5.12/tools/working-with-mbed-cli.html

## dummy  Linux 

This is a version to compile / develop with a full develop system ( standard linux desktop environment)
Tested with CentOs  7

Uses cmake to generate the test programs

go to the cmake directory<br>
there  $>cmake . <br>
then  $>make  <br> 









## raspberry PI 



