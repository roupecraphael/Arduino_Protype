# Arduino_Protype
 Arudino Rep for ARDUINO UNO Rev3 - FH Project - Final Version IMU Sensor with RGB Color Output and 7 Segment LED

## Software 
<p>include Library MPU9250.h von GIT: https://github.com/hideakitai/MPU9250 
</p>
<p>include Library Adafruit_GTX.h von GIT: https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA
</p>
<p>include Library "Adafruit_LEDBackpack.h" von GIT: 
https://github.com/adafruit/Adafruit_LED_Backpack
</p>
<p>include Library Wire.h von GIT: https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/Wire/src/Wire.h
</p>
 
## Initialisierung aller Variablen<p>
### In void setup ()  
<ul>
 <li> function initialization(): </li>
    <ul>
     <li>initialize RGB (PINMODE)</li>
     <li>initialize I2C-Protocol</li>
     <li>initialize Serial-Protocol</li>
     <li>initialize 7 Segment Display</li>
     <li>initialize MPU-Sensor</li>
     <li>initialize IMU seting via function imusetting</li>
        <ul>
         <li>set Bias</li>
         <li>set magnetic Declination</li>
         <li>Callibration of Magnetometers</li>
         <li>Callibration of Accelerometers </li>
         <li>Callibration of Gyrscopes</li>
         <li>set imu setting parameters (Standard)</li>
        </ul>
    </ul>
  </li>
</ul>
  
   
### In void loop()
<ul>
 <li>Objekt timer() tracks time internaly </li>
 <li>Objekt lesen() </li>
  <ul>
   <li>-set Orientation based on selfdeveloped algorithm </li>
   <li>-if possible read data from MPU Sensor</li>
  </ul>
 <li>Objekt reward() </li>
  <ul>
   <li>reads Orientation </li>
   <li>depending on Orientation reward function chooses to provide feedback via a RGB-Diode and displays sucessfull Squatscount on a 7 Segment Display </li>
  </ul>

 

  


