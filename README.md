# ADC101_xx_tst
test program for the ADC101_xx  i2c devices


This a simple test program for the class ADC101_xx  
It needs also the repository https://github.com/wimbeaumont/PerDevices

This contains classes and I2C abstraction. 
For the moment (june 2019 ) it works only withint an MBED environment  mbed-cli is used .

Checked on windows 7
Installed python3  and Mbed_installer_v0.4.10.exe   (mbed-cli ) 
Needed some time to understand the default settings. 

so in the file .mbed ( in  ~user\.mbed ) 

GCC_ARM_PATH=C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin
TOOLCHAIN=GCC_ARM

testing now. more install info will follow 


starting directory  C:\Users\wimb\Documents\mbed_cli\ADC101_xx_tst   == HOME
starting with ( git bash ) 
git clone https://github.com/wimbeaumont/ADC101_xx_tst.git
creates the sub dir  ADC101_xx_tst  with the main rpgoram 

then ( in cmd  ) in HOME dir 
mbed new ADC101_xx_tst --scm none   , same dir name so it adds the mbed  stuff there 

now the dir looks like 
 Directory of C:\Users\wimb\My Documents\mbed_cli\

25-Jun-19  14:27    <DIR>          .
25-Jun-19  14:27    <DIR>          ..
25-Jun-19  14:27                 8 .mbed
25-Jun-19  14:24             1,793 main.cpp
25-Jun-19  14:27    <DIR>          mbed-os
25-Jun-19  14:27                78 mbed-os.lib
25-Jun-19  14:27               114 mbed_app.json
25-Jun-19  14:27             1,384 mbed_settings.py
25-Jun-19  14:24               652 README.md
  
cd ADC101_xx_tst   
now add the lib's
 git clone https://github.com/wimbeaumont/PerDevices.git
 
 and then 
 C:\Users\wimb\My Documents\mbed_cli\ADC101_xx_tst>mbed compile -m KL25Z
 
 will add .gitignore for all mbed specic stuff. 
 




