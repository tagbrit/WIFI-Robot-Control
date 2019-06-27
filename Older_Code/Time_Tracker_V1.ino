//for I2C communication
#include <Wire.h>

//for wifi connections
#include <ESP8266WiFi.h>

//MPU6050 helper library
#include <MPU6050.h>

MPU6050 mpu;
int SCL_PIN=5;
int SDA_PIN=4;

float p1_Z_LowBound = 8;
float p1_Z_HighBound = 12;
float p1_Y_LowBound = 37;
float p1_Y_HighBound = 39.9;

float p2_Z_LowBound = 37;
float p2_Z_HighBound = 39.9;
float p2_Y_LowBound = 37;
float p2_Y_HighBound = 39.9;

float p3_Z_LowBound = 27;
float p3_Z_HighBound = 31;
float p3_Y_LowBound = 37;
float p3_Y_HighBound = 39.9;

float p4_Z_LowBound = 0;
float p4_Z_HighBound = 2;
float p4_Y_LowBound = 0;
float p4_Y_HighBound = 2;

float p5_Z_LowBound = 37;
float p5_Z_HighBound = 39.9;
float p5_Y_LowBound = 8;
float p5_Y_HighBound = 12;

float p6_Z_LowBound = 37;
float p6_Z_HighBound = 39.9;
float p6_Y_LowBound = 27;
float p6_Y_HighBound = 31;

int current_Pos = 0;
int samples[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}; 
//bool stop_Send = true;

const char* ssid     = "robotics"; //OSU hidden network name

void setup() 
{
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  delay(1);

  Serial.println("Initialize MPU6050");

  while(!mpu.beginSoftwareI2C(SCL_PIN,SDA_PIN,MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  
  checkSettings();
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
}

void loop()
{
  while(true){
    Serial.println("loop1");
    while(FillArray() == 0){
      Serial.print("[");
      for(int j=0; j<10; j++){
        Serial.print(samples[j]);
        Serial.print(",");
      }
      Serial.println("]");
    }
    if(current_Pos != samples[0]){
      current_Pos = samples[0];
      break;
//      stop_Send = false;
    }
    delay(100);
  }
 
  
  
  Serial.print("Position: ");
  Serial.println(current_Pos);
//  stop_Send = true;

  delay(500);
}

int PosCalc(){
  int sample = 0;
  Vector normAccel = mpu.readNormalizeAccel();
  //Serial.print(" Ynorm = ");
  //Serial.print(normAccel.YAxis);
  //Serial.print(" Znorm = ");
  //Serial.println(normAccel.ZAxis);
  
  if(((p1_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p1_Z_HighBound)) && ((p1_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p1_Y_HighBound))){
    sample = 1;
    //Serial.println("Pos1 Case");
    return sample;
  }
  if(((p2_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p2_Z_HighBound)) && ((p2_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p2_Y_HighBound))){
    sample = 2;
    //Serial.println("Pos2 Case");
    return sample;  
  }
  if(((p3_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p3_Z_HighBound)) && ((p3_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p3_Y_HighBound))){
    sample = 3;
    //Serial.println("Pos3 Case");
    return sample;  
  }
  if(((p4_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p4_Z_HighBound)) && ((p4_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p4_Y_HighBound))){
    sample = 4;
    //Serial.println("Pos4 Case");
    return sample;
  }
  if(((p5_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p5_Z_HighBound)) && ((p5_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p5_Y_HighBound))){
    sample = 5;
    //Serial.println("Pos5 Case");
    return sample;
  }
  if(((p6_Z_LowBound < normAccel.ZAxis) && (normAccel.ZAxis < p6_Z_HighBound)) && ((p6_Y_LowBound < normAccel.YAxis) && (normAccel.YAxis < p6_Y_HighBound))){
    sample = 6;
    //Serial.println("Pos6 Case");
    return sample;
  }

  return sample;  
}

int FillArray(){
  for(int i=0; i<10; i++){
    samples[i] = PosCalc();
    delay(100);
  }
  return ArrayCheck();
}

int ArrayCheck(){
  for(int i=0; i<9; i++){
    if(samples[i] != samples[i+1]){
      return 0;
    }
  }
  return 1;
}
