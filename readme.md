# amiHealthy
amiHealthy is not a device to measure if you're healthy. I just thought it was a fun name

What amiHealthy IS is a device that "measures" your heart rate and pulse oxygen levels (shoots light and reabsorbs it)
It's based off of the MAX30102 sensor that uses the difference between red and IR light to determine how oxygenated the blood is. By reading this a LOT of times per second, it can determine what your heart rate is by sensing how that level changes. And it detects SpO2 by detecting the difference in how much IR vs red is absorbed vs reflected by the blood. Really cool concept. 

### Images

NOTE: I switched to kicad 9 for this project and it broke some of the rendering so the front and back of the board have some issues that I wasn't able to work out before hermes deadline

![sch](https://github.com/Omegon0/hermes-hrm/blob/main/sch.jpg?raw=true)
![PCB](https://github.com/Omegon0/hermes-hrm/blob/main/pcb.jpg?raw=true)
![r1](https://github.com/Omegon0/hermes-hrm/blob/main/r1.jpg?raw=true)
![r2](https://github.com/Omegon0/hermes-hrm/blob/main/r2.jpg?raw=true)
