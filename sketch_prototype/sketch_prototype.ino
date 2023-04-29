#include <MPU9250.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
//Define class MPU9250, setting//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MPU9250 IMU;
MPU9250Setting setting;
//Internal Time Tracker+Neofruit + avec[num]//------------------------------------------------------------------------------------------------------------------------------------
int i = 0, x, ledpin = 6, lednum = 15, leddelay = 500, num, internalreward;
long int Time = 0;
bool go = false;
//Variables for comparision, gyr acc and mag data+ internal reward funtion parameters + orientationcoefficent//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float yaw, pitch, roll, ax, ay, az, gx, gy, gz, mx, my, mz,orientation_gforce = 1.0, buffer, threshold = 0.05,rewardmetrix;
//conversion parameters protype
char axbuff[6], aybuff[6], azbuff[6], gxbuff[6], gybuff[6], gzbuff[6], mxbuff[6], mybuff[6], mzbuff[6];
float avec[3], gvec[3], mvec[3];
//Calibration data input varibales -> biases must be input here because they are called in setup callibration//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const float accbiasx = 0, accbiasy = 0, accbiasz = 0, gyrobiasx = 0, gyrobiasy = 0, gyrobiasz = 0, magbiasx = 0, magbiasy = 0, magbiasz = 0, magscalex = 0, magscaley = 0, magscalez = 0;
//filter iteration//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const size_t n = 1;
//Bluetooth//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define tx_pin 10
#define rx_pin 11
SoftwareSerial maindataport = SoftwareSerial(rx_pin,tx_pin);
String messageBuffer = "", message = "",input = "", output = "";
//ArdafruiNeopixel initialization //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Adafruit_NeoPixel LED = Adafruit_NeoPixel(lednum, ledpin, NEO_GRB + NEO_KHZ800);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  initialization();
}
//Setup complete now loop reading sequenze// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  timer();
  lesen();
  reward();
  conversion();
  usb();
}
//Time Function tracks data each 0.1 second is defined by delay statement //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void timer() {
  Time++;
  delay(100);
}
//object usb for data transfer//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void usb(){
    messagebuffer();
}
//object messagebuffer for data transfer//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void messagebuffer(){
    messageBuffer = "";
    messageBuffer += String(Time);
    messageBuffer += ",";
    messageBuffer += String(axbuff);
    messageBuffer += ",";
    messageBuffer += String(aybuff);
    messageBuffer += ",";
    messageBuffer += String(azbuff);
    messageBuffer += ",";
    messageBuffer += String(gxbuff);
    messageBuffer += ",";
    messageBuffer += String(gybuff);
    messageBuffer += ",";
    messageBuffer += String(gzbuff);
    messageBuffer += ",";
    messageBuffer += String(mxbuff);
    messageBuffer += ",";
    messageBuffer += String(mybuff);
    messageBuffer += ",";
    messageBuffer += String(mzbuff);
    Serial.print(messageBuffer);
    Serial.print("!"); //end of message
}
//object Bluetooth data transfer//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void bluetooth() {
//check if data is available on the maindataport//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (maindataport.available() > 0) {
    input = maindataport.readStringUntil('!');
    if (input != "") {
      message = input;
      messagebuffer();
    }
  }
//check if data is available on the Serialport//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('!');
    if (input != "") {
      message = input;
      messagebuffer();
    }
  }
}
//object conversion for data transfer//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void conversion() {
  dtostrf(ax, 5, 2, axbuff);
  dtostrf(ay, 5, 2, aybuff);
  dtostrf(az, 5, 2, azbuff);
  dtostrf(gx, 5, 2, gxbuff);
  dtostrf(gy, 5, 2, gybuff);
  dtostrf(gz, 5, 2, gzbuff);
  dtostrf(mx, 5, 2, mxbuff);
  dtostrf(my, 5, 2, mybuff);
  dtostrf(mz, 5, 2, mzbuff);
}
//object for LED color change//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setLedColor() {
  LED.clear();
  switch (internalreward) {
      case 0: //rot
        LED.setPixelColor(0, LED.Color(150, 0, 0));
        LED.show();
        break;
      case 1://grün
        LED.setPixelColor(1, LED.Color(0, 150, 0)); 
        LED.show();
        break;
      case 2://gelb
        LED.setPixelColor(3, LED.Color(150, 150, 0)); 
        LED.show();
        break;
      case 3: //blau
        LED.setPixelColor(2, LED.Color(0, 0, 150)); 
        LED.show();
      default:
        break;
  }
}
//object oriantation for approximating the orientation of the device//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void orientation() { 
 for (i = 0; i<3; i++){
   buffer = avec[i];
   if (abs(buffer - orientation_gforce) < threshold){
     num = i;
   }else{}
 }
}
//internal reward function//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void rewardfunction(float input){
  rewardmetrix = 1- abs(input) ;
  if (rewardmetrix < 0.5) {
    internalreward= 0;
    setLedColor();
  } else if (rewardmetrix > 0.8) {
    internalreward = 1;
    setLedColor();
  } else {
    internalreward = 2;
    setLedColor();
  } 
}
//object reward for feedback//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void reward() {
    orientation();
    switch (num) {
      case 0:
        //if num is 0 = ax nehmen
        rewardfunction(ax);
        break;
      case 1:
        // if num is 1 = ay nehmen
        rewardfunction(ay);
        break;
      case 2:
        // if num is 2 = az nehmen
        rewardfunction(az);
        break;
      default:
        Serial.println("Error");
        break;
    }
}
//object lesen for data reading//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void lesen() {
//first iteration to get the first values for the filter//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (go == false) {
    if (IMU.update()) {
      ax = IMU.getAccX();
      ay = IMU.getAccY();
      az = IMU.getAccZ();
      avec[0] = ax;
      avec[1] = ay;
      avec[2] = az;
    }
    go = true;
  }
//second iteration to get data continously//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  else {
    if (IMU.update()) {
      //read Data simple 3d orientation via yaw pitch and roll maybe usefull for commparison //--------------------------------------------------------------------------------------------------------------------------------------------------------
      yaw = IMU.getYaw();
      pitch = IMU.getPitch();
      roll = IMU.getRoll();
      //ACC Data in an-d +conversion with bandwith filter //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      ax = IMU.getAccX();
      ay = IMU.getAccY();
      az = IMU.getAccZ();
      //Gyro Data in gn-gd + full range for gyroscopes// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      gx = IMU.getGyroX();
      gy = IMU.getGyroY();
      gz = IMU.getGyroZ();
      //Mag Data  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      mx = IMU.getMagY();
      my = IMU.getMagZ();
      mz = IMU.getMagZ();
    }
  }
}
//object IMU setting//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void imusetting() {
  // needs to be adjusted !!! default settings for now (after ::____ ->look up in libary and change values to enum class variables)//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
  setting.gyro_fchoice = 0x03;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
  //VIENNA DECLINATION//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  IMU.setMagneticDeclination(+5.08);
  IMU.calibrateMag();
  IMU.calibrateAccelGyro();
  bias();
}
//initialization of all components//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initialization() {
  maindataport.begin(115200);
  Serial.begin(115200);
  LED.begin();
  internalreward = 3;
  setLedColor();
//Wire initialization for data transfer via lsa and lcb //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Wire.begin();
//initialization of imu sensor- feedback for testing systems//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  IMU.setup(0x68);
  imusetting();
  Serial.print("Setup finished");
  internalreward = 0;
  setLedColor();
}
//object for bias calibration//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void bias() {
//calibration setup //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  IMU.setAccBias(accbiasx, accbiasy, accbiasz);
  IMU.setGyroBias(gyrobiasx, gyrobiasy, gyrobiasz);
  IMU.setMagBias(magbiasx, magbiasy, magbiasz);
  IMU.setFilterIterations(n);
  IMU.selectFilter(QuatFilterSel::MADGWICK);
  IMU.setMagScale(magscalex, magscaley, magscalez);

}


//feedback on board 2 led + akustisches signal
//optional processing usb ---