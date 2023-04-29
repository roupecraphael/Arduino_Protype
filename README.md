# Arduino_Protype
 Arudino Rep for ARDUINO UNO Rev3 - FH Project - Protoype IMU Sensor with Bluetooth data transmission

## Software 
<p>Verwendung der Library <MPU9250.h> von GIT: https://github.com/hideakitai/MPU9250 </p>
<p>Verwendung der Library <Software Serial.h> DOC: https://docs.arduino.cc/learn/built-in-libraries/software-serial</p>
<p>Verwendung der Library <Adafruit_Neopixel> von GIT: https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA</p>
 
## Initialisierung aller Variablen<p>
### In void setup ()  Objekt Initialisierung:
<ul>
 <li>*Initialisierung der LED</li>
 <li>*Initialisierung des I2C-Protokolls</li>
 <li>*Initialisierung des Serial-Protokolls</li>
 <li>*Initialisierung des Bluetoothserial- Protokolls</li>
 <li>*Initialisierung des MPU-Sensors</li>
 <li>*Initialisierung der IMU Sensoreinstellungen mithilfe des Objektes imusetting</li>
    <ul>
     <li>-Setzen des Bias</li>
     <li>-Setzen der Magnetischen Deklination</li>
     <li>-Callibration des Magnetometers</li>
     <li>-Kallibration des Accelerometers </li>
     <li>-Kallibration des Gyrscopes</li>
     <li>-Setzen der setting Parameters</li>
    </ul>
  </li>
</ul>
  
   
### In void loop()
<ul>
 <li>*Objekt timer() trackt 0.1 Sekunde intern die Zeit</li>
 <li>*Objekt lesen() tut</li>
  <ul>
   <li>-Orientierung im Raum feststellen</li>
   <li>-Wenn möglich registrierte Daten aus MPU9250 auslesen</li>
  </ul>
 <li>*Objekt reward() </li>
  <ul>
   <li>-Orientierung im Raum auslesen</li>
   <li>-Abhängig von Orientierung im Raum Bewegung anhand des Objektes rewardfunction Feedback über LED ausgeben</li>
  </ul>
 <li>*Objekt conversion() konvertiert float in string</li>
 <li>*Objekt usb()/bluetooth() data transmission of buffer to Serial or bluetooth port</li>

  


