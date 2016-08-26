# Retro|Watch 2
 <p align="center">
 <img src="" width="400"/>
 <em>Assembled Board</em>
 
## Short description
The Retro|Watch V2 is based around the HP QDSP-6064 Bubble display, used in 70s HP calculators like the [HP-35](https://en.wikipedia.org/wiki/HP-35). The display is controlled by an Atmel ATMega328U4 MCU with Arduino code and flashed with an Adafruit Feather 32U4 bootloader, and the time is kept by a Maxim DS3231 Real-Time Clock module, that stays connected to the LiPo battery at all times, keeping time even when the microcontroller and display are not powered.
This second version of the watch uses another microcontroller, the ATMega32U4, for USB capability, and a battery recharge circuit for USB Charging. In order to fit all that added circuitry, smaller components as 0603 resistors and a QFN chip were used. **__THIS MAKES ASSEMBLY CONSIDERABLY HARDER THAN V1, and is not recommended for beginners.__**
The code is open source, licensed under the MIT License, and the harware is open-source hardware.

###V1 was Featured on:
[Hackaday.com blog post by Elliot Williams](http://hackaday.com/2016/06/25/easy-bubble-watch-oozes-retro-charm/)
  
## Hardware
###PCB
The board design was made in Eagle, and is fully tested. A 500 mAh [LiPo Battery from Adafruit](https://adafru.it/1578) fits perfectly under the board. The board design can be freely modified. It can also directly be ordered on the OSHPark website: 
<p align="center"><a href="https://oshpark.com/shared_projects/D0Ed6ISU"><img src="https://oshpark.com/assets/badge-5b7ec47045b78aef6eb9d83b3bac6b1920de805e9a0c227658eac6e19a045b9c.png" alt="Order from OSH Park"></img></a></p>

###Bill Of Materials
You can find the BOM [here](https://github.com/RafaelRiber/Retro-Watch2/blob/master/Hardware/BOM.csv).
  
##Software
The software was written in Arduino code, is licensed under MIT License, and can be uploaded on the microcontroller via ISP.
Be sure to set the desired time in the code prior to uploading, to set the time registers on the RTC.

Functions of the code include:
* Hours, minutes and seconds
* Day, month, year, day of year, day in week, week in year.
* Stopwatch (start, stop, reset)
* Brightness setting in software, saved to EEPROM to save even if the watch is powered off.
* Battery voltage check and low battery alert
 
To be able to upload the code on the watch, you will need to follow the same steps as V1 (see V1 repo), but using the Adafruit Feather 32U4 Bootloader instead of the ATMega328P one.
###Required Libraries
1. [Chrono library by thomasfredericks](https://github.com/thomasfredericks/Chrono)
2. [Button library by JChristensen](https://github.com/JChristensen/Button)
3. [RTCLib by Adafruit Industries](https://github.com/adafruit/RTClib)
4. [SevSeg by SparkFun](https://github.com/sparkfun/SevSeg)
