JMRI-CMRI Sensor Shield Sketch with Digital and Analog Sensors.
by Anthony Kochevar v2.1-2023
This sketch is for use with an Arduino Mega and Sensor Shield
Code maybe modified and distributed as long as it is done so freely.
Code is as is with no warranty implied or offered.

==Please read all comments below so you understand the sketch==

For best performance only enable the number of sensors you need 
using numDigiSensors.  If you only needed 30 
sensors, you'd set numDigiSensors to 30 and use ports 3 to 33 on the 
shield.
The Analog ports will be used as analog sensors.  Only enable what
you need in numAnaSensors for best performance.  These start at  
A0 (pin 54) and JMRI #054.

Ports 0, 1 and 2 are not used as they are used internally 
for CMRI communication. 

The JMRI +1 offset has also been taken into account in the sketch
and as such the hardware address in JMRI maps to the correct port.
If the CMRI_ADDR node address was 3 the sensor on port 3 will have
hardware address 3003 in JMRI.  A sensor on port 45 will be 3045.
A sensor on port A10 (port 54) the start of the Analog pins will be 3054.
