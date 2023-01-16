/* JMRI-CMRI Sensor Shield Sketch with Digital and Analog Sensors.
 * by Anthony Kochevar v2.1-2023
 * This sketch is for use with an Arduino Mega and Sensor Shield
 * Code maybe modified and distributed as long as it is done so freely.
 * Code is as is with no warranty implied or offered.
 *
 * ==Please read all comments below so you understand the sketch==
 *
 * For best performance only enable the number of sensors you need 
 * using numDigiSensors.  If you only needed 30 
 * sensors, you'd set numDigiSensors to 30 and use ports 3 to 33 on the 
 * shield.
 * The Analog ports will be used as analog sensors.  Only enable what
 * you need in numAnaSensors for best performance.  These start at  
 * A0 (pin 54) and JMRI #054.
 *
 * Ports 0, 1 and 2 are not used as they are used internally 
 * for CMRI communication. 
 *
 * The JMRI +1 offset has also been taken into account in the sketch
 * and as such the hardware address in JMRI maps to the correct port.
 * If the CMRI_ADDR node address was 3 the sensor on port 3 will have
 * hardware address 3003 in JMRI.  A sensor on port 45 will be 3045.
 * A sensor on port A10 (port 54) the start of the Analog pins will be 3054.
*/


#include <CMRI.h>
#include <Auto485.h>


#define CMRI_ADDR 2 //CMRI node address in JMRI.  Change if needed.
#define DE_PIN 2  // Arduino pin used internally for CMRI communication.  Do not change.

Auto485 bus(DE_PIN);
CMRI cmri(CMRI_ADDR, 70, 0, bus);  // Initalize CMRI with up to 70 inputs and 0 outputs.

// numXXXSensors sets the total number of sensors on the board you want to use.
// First digital port that can be used is 3
// First analog port that can be used is 54 (A0)
int numDigiSensors = 50; // Maximum digital sensors is 50.  Starts at port (pin 3) on Mega.
int numAnaSensors = 16;  // Maximum analog sensors is 16.  Starts at port A0 (pin 54) on Mega.
int anaValue[16];  // Set to the same value as numAnaSensors.

int numSamples = 10;  // Number of readings to get from analog sensors.

// Adjust array for analog sensitivity. Minimum thresold for tripping the sensor. Adjust value per sensor.
const int threshold[16] = 
  { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };  

//Below array is for use of the analog pins as sensors.  Do not change.
const int anaPin[16] = 
  { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };

void setup() {
  Serial.begin(19200);
  numDigiSensors += 3;  // Add +3 to match Port 3, the first available port on the Arduino that can be used.
  for (int x = 3; x <= numDigiSensors; x++) pinMode(x, INPUT);
  for (int a = 0; a < numAnaSensors; a++) pinMode(anaPin[a], INPUT);
  
}

void loop() {
  cmri.process();
  
  // ====Digital Sensors====
  for (int x = 3; x <= numDigiSensors; x++) cmri.set_bit(x-1, !digitalRead(x)); 
    // x-1 is to account for JMRI offset.  Port 3 on board will now be address #003 in JMRI.


  // ====Analog Sensors====
  for (int a = 0; a < numAnaSensors; a++) anaValue[a] = 0;  //  Reset all Analog readings to 0;
  for (int x = 1; x <= numSamples; x++) {  // Take number of numSamples to get an average reading.
    for (int a = 0; a < numAnaSensors; a++) {
	  anaValue[a] += analogRead(anaPin[a]);
      delay(1);  // Pause slightly for debounce.
    }
  }
  int y = 53; // Starting port of analog pins with -1.
  // Starting Analog Port on Arduino is 54 (A0).  -1 Offset is to account for the JMRI offset.  
  // Port 54 will now be #054 in JMRI.
  for (int a = 0; a < numAnaSensors; a++) {
    anaValue[a] /= numSamples;  // Divide to get the average from numSamples taken. 
    if (anaValue[a] > threshold[a]) {
      cmri.set_bit(y, 1);
      Serial.print(F("Port: ")); Serial.print(anaPin[a]); Serial.print(F("  JMRI: #0")); Serial.println(y+1);
      Serial.println(anaValue[a]);
      Serial.println(F("Active"));
    }
    else {
      cmri.set_bit(y, 0);
      Serial.print(F("Port: ")); Serial.print(anaPin[a]); Serial.print(F("  JMRI: #0")); Serial.println(y+1);
      Serial.println(anaValue[a]);
      Serial.println(F("In-Active"));
    }
    y++;  // Advance y for next a.
  }
}
  

