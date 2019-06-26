//for I2C communication
#include <Wire.h>
#include <MPU6050_tockn.h>

//for wifi connections
#include <ESP8266WiFi.h>



MPU6050 mpu6050(Wire);

int SCL_PIN=5;
int SDA_PIN=4;

int current_Pos = 0;
int samples[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}; 


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


  Wire.begin(SDA_PIN,SCL_PIN);
  mpu6050.begin();
 
}


void loop()
{
  
  while(true){
   // Serial.println("loop1");
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
    }
    delay(100);
  }
 
  
  
  Serial.print("Position: ");
  Serial.println(current_Pos);
//  stop_Send = true;

  delay(500);
}

int PosCalc(){
  mpu6050.update();
  int sample = 0;
  if(mpu6050.getAccX() > (0.5)){
    sample = 1;
    return sample;
  }
    if(mpu6050.getAccZ() > (0.5)){
    sample = 2;
    return sample;
  }
    if(mpu6050.getAccX() < (-0.5)){
    sample = 3;
    return sample;
  }
    if(mpu6050.getAccZ() < (-0.5)){
    sample = 4;
    return sample;
  }
    if(mpu6050.getAccY() < (-0.5)){
    sample = 5;
    return sample;
  }
    if(mpu6050.getAccY() > (0.5)){
    sample = 6;
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
