# Arduino_Protype
 Arudino Rep for ARDUINO UNO Rev3 - FH Project - Protoype IMU Sensor with Bluetooth data transmission

## Software 
Verwendung der Library <MPU9250.h> von GIT: https://github.com/hideakitai/MPU9250 
Verwendung der Library <Software Serial.h> DOC: https://docs.arduino.cc/learn/built-in-libraries/software-serial
Verwendung der Library <Adafruit_Neopixel> von GIT: https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA
 
Initialisierung aller Variablen
In void setup ()  Objekt Initialisierung:
  *Initialisierung der LED	
  *Initialisierung des I2C-Protokolls
  *Initialisierung des Serial-Protokolls
  *Initialisierung des Bluetoothserial- Protokolls
  *Initialisierung des MPU-Sensors
  *Initialisierung der IMU Sensoreinstellungen mithilfe des Objektes imusetting
    -Setzen des Bias
    -Setzen der Magnetischen Deklination
    -Callibration des Magnetometers
    -Kallibration des Accelerometers 
    -Kallibration des Gyrscopes
    -Setzen der setting Parameters
In void loop()
  *Objekt timer() trackt 0.1 Sekunde intern die Zeit
  *Objekt lesen() tut
     -Orientierung im Raum feststellen
     -Wenn möglich registrierte Daten aus MPU9250 auslesen
  *Objekt reward() 
     -Orientierung im Raum auslesen
     -Abhängig von Orientierung im Raum Bewegung anhand des Objektes rewardfunction Feedback über LED ausgeben
  *Objekt conversion() konvertiert float in string
  *Objekt usb()/bluetooth() speichert in buffer und schickt an Serial und bluetooth port

