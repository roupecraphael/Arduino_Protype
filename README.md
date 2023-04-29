# Arduino_Protype
 Arudino Rep for ARDUINO UNO Rev3 - FH Project - Protoype IMU Sensor with Bluetooth data transmission

## Software 
<p>Use Library <MPU9250.h> von GIT: https://github.com/hideakitai/MPU9250 </p>
<p>Use Library <Software Serial.h> DOC: https://docs.arduino.cc/learn/built-in-libraries/software-serial</p>
<p>Use Library <Adafruit_Neopixel> von GIT: https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA</p>
 
## Initialisierung aller Variablen<p>
### In void setup ()  Objekt initialize():
<ul>
 <li>*initialize LED</li>
 <li>*initialize I2C-Protocol</li>
 <li>*initialize Serial-Protocol</li>
 <li>*initialize Bluetoothserial- Protocol</li>
 <li>*initialize MPU-Sensor</li>
 <li>*initialize IMU seting via object imusetting</li>
    <ul>
     <li>-set Bias</li>
     <li>-set magnetic Declination</li>
     <li>-Callibration of Magnetometers</li>
     <li>-Callibration of Accelerometers </li>
     <li>-Callibration of Gyrscopes</li>
     <li>-set imu setting parameters</li>
    </ul>
  </li>
</ul>
  
   
### In void loop()
<ul>
 <li>*Objekt timer() tracks time internaly each 0.1 sec</li>
 <li>*Objekt lesen() does</li>
  <ul>
   <li>-set Orientation </li>
   <li>-if possible read data from MPU Sensor</li>
  </ul>
 <li>*Objekt reward() </li>
  <ul>
   <li>-reads Orientation </li>
   <li>-depending on Orientation reward function chooses to provide feedback via LED</li>
  </ul>
 <li>*Objekt conversion() converts float in string</li>
 <li>*Objekt usb()/bluetooth() data transmission of buffer to Serial or bluetooth port</li>

  


