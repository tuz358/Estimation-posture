
#include<Wire.h>

const int INTERVAL = 20;//ms
const int MPU = 0x68;
const int RATIO = 0.99;//For Complementary Filter

uint32_t timer;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float roll_acc,pitch_acc,yaw_acc,gyroX,gyroY,gyroZ;
double angleX,angleY,angleZ,pre_X = 0.0,pre_Y=0.0,pre_Z=0.0,firstZ;
double dt;
int inByte = 0;


void Get_Sensor_Data(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void Calc_Angle(){
  roll_acc = atan2(AcY,AcZ) * 180 / PI;
  pitch_acc = atan2(-1.0 * AcX,AcZ) * 180 / PI;
  //yaw_acc = (atan2(AcX,AcY) * 180 / PI) - firstZ;

  gyroX += GyX / 131.0 * dt;
  gyroY += GyY / 131.0 * dt;
  gyroZ += GyZ / 131.0 * dt;
}

void Complementary_Filter(){
  angleX = RATIO*(pre_X + gyroX) + (1.0 - RATIO)*roll_acc;
  angleY = RATIO*(pre_Y + gyroY) + (1.0 - RATIO)*pitch_acc;
  angleZ = gyroZ;
}



void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);
  
  Get_Sensor_Data();
  Calc_Angle();

  pre_X = RATIO*(pre_X + gyroX) + (1.0 - RATIO)*roll_acc;
  pre_Y = RATIO*(pre_Y + gyroY) + (1.0 - RATIO)*pitch_acc;
  pre_Z = RATIO*(pre_Z + gyroZ) + (1.0 - RATIO)*yaw_acc;
  
  firstZ = pre_Z;

  timer = micros();
  establishContact();
}

void loop(){
  dt = (double)(micros() - timer) / 1000000;
  timer = micros();

  Get_Sensor_Data();
  Calc_Angle();
  Complementary_Filter();
   
  if(Serial.available() > 0){
    
    inByte = Serial.read();
    
    if(inByte == 42){
      gyroX = 0.0;
      gyroY = 0.0;
      gyroZ = 0.0;
    }
    
    Serial.print((float)angleX);
    Serial.print(",");
    Serial.print((float)angleY);
    Serial.print(",");
    Serial.println((float)angleZ);
  }
  
  pre_X = angleX;
  pre_Y = angleY;
  pre_Z = angleZ;
  
  delay(INTERVAL);
}

void establishContact() {
  // Waiting for sending some charcters from Processing
  while (Serial.available() <= 0) {
	// Initialize buffer
    Serial.println("0,0"); 
    delay(300);
  }
}
