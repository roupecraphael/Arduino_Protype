# Arduino_Protype
 Arudino Rep for ARDUINO UNO Rev3 - FH Project - Protoype IMU Sensor with Bluetooth data transmission

## Software 
<p>Verwendung der Library <MPU9250.h> von GIT: https://github.com/hideakitai/MPU9250 </p>
<p>Verwendung der Library <Software Serial.h> DOC: https://docs.arduino.cc/learn/built-in-libraries/software-serial</p>
<p>Verwendung der Library <Adafruit_Neopixel> von GIT: https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA</p>
 
## Initialisierung aller Variablen<p>
<p>In void setup ()  Objekt Initialisierung:</p>
  <p>*Initialisierung der LED</p>	
  <p>*Initialisierung des I2C-Protokolls</p>
  <p>*Initialisierung des Serial-Protokolls</p>
  <p>*Initialisierung des Bluetoothserial- Protokolls</p>
  <p>*Initialisierung des MPU-Sensors</p>
  <p>*Initialisierung der IMU Sensoreinstellungen mithilfe des Objektes imusetting</p>
    <p>-Setzen des Bias</p>
    <p>-Setzen der Magnetischen Deklination</p>
    <p>-Callibration des Magnetometers</p>
    <p>-Kallibration des Accelerometers </p>
    <p>-Kallibration des Gyrscopes</p>
    <p>-Setzen der setting Parameters</p>
<p>In void loop()</p>
  <p>*Objekt timer() trackt 0.1 Sekunde intern die Zeit</p>
  <p>*Objekt lesen() tut</p>
     <p>-Orientierung im Raum feststellen</p>
     <p>-Wenn möglich registrierte Daten aus MPU9250 auslesen</p>
  <p>*Objekt reward() </p>
     <p>-Orientierung im Raum auslesen</p>
     <p>-Abhängig von Orientierung im Raum Bewegung anhand des Objektes rewardfunction Feedback über LED ausgeben</p>
  <p>*Objekt conversion() konvertiert float in string</p>
  <p>*Objekt usb()/bluetooth() speichert in buffer und schickt an Serial und bluetooth port</p>

