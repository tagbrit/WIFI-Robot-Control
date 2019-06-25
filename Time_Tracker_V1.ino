//for I2C communication
#include <Wire.h>

//for wifi connections
#include <ESP8266WiFi.h>

//MPU6050 helper library
#include <MPU6050.h>

MPU6050 mpu;
int SCL_PIN=5;
int SDA_PIN=4;

float p1_Z_LowBound = 0;
float p1_Z_HighBound = 0;
float p1_Y_LowBound = 0;
float p1_Y_HighBound = 0;

float p2_Z_LowBound = 0;
float p2_Z_HighBound = 0;
float p2_Y_LowBound = 0;
float p2_Y_HighBound = 0;

float p3_Z_LowBound = 0;
float p3_Z_HighBound = 0;
float p3_Y_LowBound = 0;
float p3_Y_HighBound = 0;

float p4_Z_LowBound = 0;
float p4_Z_HighBound = 0;
float p4_Y_LowBound = 0;
float p4_Y_HighBound = 0;

float p5_Z_LowBound = 0;
float p5_Z_HighBound = 0;
float p5_Y_LowBound = 0;
float p5_Y_HighBound = 0;

float p6_Z_LowBound = 0;
float p6_Z_HighBound = 0;
float p6_Y_LowBound = 0;
float p6_Y_HighBound = 0;

int current_Pos = 0;
int prev_Pos;

void setup() 
{
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
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
  Serial.begin(115200);

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
  PosCalc();
  if(current_Pos != prev_Pos){
    /*insert get/post request here*/
  }
  
  delay(500);
}

void PosCalc(){
  prev_Pos = current_Pos;
  Vector normAccel = mpu.readNormalizeAccel();

  if((p1_Z_LowBound < normAccel.Zaxis < p1_Z_HighBound) && (p1_Y_LowBound < normAccel.Yaxis < p1_Y_HighBound)){
    current_Pos = 1;
  }
  if((p2_Z_LowBound < normAccel.Zaxis < p2_Z_HighBound) && (p2_Y_LowBound < normAccel.Yaxis < p2_Y_HighBound){
    current_Pos = 2;  
  }
  if((p3_Z_LowBound < normAccel.Zaxis < p3_Z_HighBound) && (p3_Y_LowBound < normAccel.Yaxis < p3_Y_HighBound){
    current_Pos = 3;
  }
  if((p4_Z_LowBound < normAccel.Zaxis < p4_Z_HighBound) && (p4_Y_LowBound < normAccel.Yaxis < p4_Y_HighBound){
    current_Pos = 4;
  }
  if((p5_Z_LowBound < normAccel.Zaxis < p5_Z_HighBound) && (p5_Y_LowBound < normAccel.Yaxis < p5_Y_HighBound){
    current_Pos = 5;
  }
  if((p6_Z_LowBound < normAccel.Zaxis < p6_Z_HighBound) && (p6_Y_LowBound < normAccel.Yaxis < p6_Y_HighBound){
    current_Pos = 6;
  }  
}
