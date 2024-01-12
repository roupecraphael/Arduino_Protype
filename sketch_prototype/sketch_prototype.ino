#include <MPU9250.h>
#include <Adafruit_NeoPixel.h>
/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
//Define class MPU9250, setting//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MPU9250 IMU;
MPU9250Setting setting;
Adafruit_7segment matrix = Adafruit_7segment();

//Internal Time Tracker+Neofruit + avec[num]//------------------------------------------------------------------------------------------------------------------------------------
int i = 0, x, ledpin = 6, lednum = 15, leddelay = 500, num, internalreward, sucnum=0 , diff2time = 0;
long int Time = 0, diff1time = 0;
bool go = false, rewardset= false;
//Variables for comparision, gyr acc and mag data+ internal reward funtion parameters + orientationcoefficent//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float yaw, pitch, roll, ax, ay, az, gx, gy, gz, mx, my, mz,orientation_gforce = 1.0, buffer, threshold = 0.05,rewardmetrix;
//conversion parameters protype
char axbuff[6], aybuff[6], azbuff[6], gxbuff[6], gybuff[6], gzbuff[6], mxbuff[6], mybuff[6], mzbuff[6];
float avec[3], gvec[3], mvec[3];
//Calibration data input varibales -> biases must be input here because they are called in setup callibration//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const float accbiasx = 0, accbiasy = 0, accbiasz = 0, gyrobiasx = 0, gyrobiasy = 0, gyrobiasz = 0, magbiasx = 0, magbiasy = 0, magbiasz = 0, magscalex = 0, magscaley = 0, magscalez = 0;
//filter iteration//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const size_t n = 1;
//ArdafruiNeopixel initialization //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Adafruit_NeoPixel LED = Adafruit_NeoPixel(lednum, ledpin, NEO_GRB + NEO_KHZ800);

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// setup function
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void setup() {
  initialization();
}
//initialization of all components//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initialization() {
  Serial.begin(115200);
  feedbackinit();
  imuinit();
  feedbackfin();
}
// feedback is initialized
void feedbackinit(){
  sevensegsetup();
  LED.begin();
  internalreward = 3;
  setLedColor();
}
void imuinit(){
//Wire initialization for data transfer via lsa and lcb //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  Wire.begin();
//initialization of imu sensor- feedback for testing systems//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  IMU.setup(0x68);
  imusetting();
}
void feedbackfin(){
  internalreward = 0;
  setLedColor();
}
//setup for 7 Segment 
void sevensegsetup(){
  #ifndef __AVR_ATtiny85__
    Serial.println("7 Segment Backpack Test");
  #endif
    matrix.begin(0x70);
  matrix.print("INIT");
  matrix.writeDisplay();
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
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Setup complete now loop reading sequenze// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void loop() {
  timer();
  lesen();
  definescunum();
  reward();
  Serial.print(Time);
  Serial.print("-");
  Serial.print(internalreward);
  Serial.print("-");
  Serial.print(sucnum);
  Serial.print("-");
  Serial.print(diff1time);
  //Serial.print("-");
  //Serial.print(diff2time);
  Serial.println();
}
//Time Function tracks data each 0.1 second is defined by delay statement //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void timer() {
  Time = millis();
  Time = Time - 26949;
  Time = Time / 1000;
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
    setMatrix();
  } else if (rewardmetrix > 0.8) {
    internalreward = 1;
    setLedColor();
    setMatrix();
    delay(4000);
  } else {
    internalreward = 2;
    setLedColor();
    setMatrix();
  } 
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
// non functional 
void definescunum(){
  if (internalreward == 1 || internalreward == 2){
    if (!rewardset){
      sucnum++;
      rewardset = true;
      if ((Time - diff1time) >= 15){
        diff1time = Time;
        rewardset = false;
        } 
    }
  if (internalreward == 0) {
      rewardset = false;
    }
  }
}
void setMatrix(){
matrix.clear();
  switch (internalreward) {
      case 0: //rot
          matrix.print(sucnum, DEC);
          matrix.writeDisplay();
        break;
      case 1://grün
          sucnum = sucnum +1;
          matrix.print(sucnum, DEC);
          matrix.writeDisplay();
        break;
      case 2://gelb
          matrix.print(sucnum, DEC);
          matrix.writeDisplay();
        break;
      case 3: //blau
        matrix.print("_GO_");
        matrix.writeDisplay();
      default:
        break;
  }
}

