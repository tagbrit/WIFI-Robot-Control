//for I2C communication
#include <Wire.h>

//added Arduino library for mpu6050
#include <MPU6050_tockn.h>

//for wifi connections
#include <ESP8266WiFi.h>

//to use mpu6050
MPU6050 mpu6050(Wire);

#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define MY_ID "Device01"
//pins for esp8266
int SCL_PIN=5;
int SDA_PIN=4;

//initialize current position and sample array
int current_Pos = 0;
int samples[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}; 
const char* ssid     = "robotics"; //OSU hidden network name



void setup() 
{
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
  delay(100);
  WiFi.mode(WIFI_STA);  //Hides the viewing of ESP as wifi hotspot
  
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

  //set up esp8266 to send and receive data from mpu6050
  Wire.begin(SDA_PIN,SCL_PIN);
  mpu6050.begin();

  pinMode(LED_BUILTIN, OUTPUT);
 
}


void loop()
{
  /*Keep filling the array with new data,
   *if there are discrepencies in the data
   *it will recalculate the position and
   *send the new position if the current 
   *position is different from the data in the
   *array
   */
  HTTPClient http;  //object of class HTTPClient
  String postData;
  while(true){
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
  digitalWrite(LED_BUILTIN, HIGH); 
  Serial.print("Position: ");
  Serial.println(current_Pos);
  
  postData = "message={\"" + String(MY_ID) + String("\":\"") + String(current_Pos) + String("\"}");
  Serial.println(postData);
  
  http.begin("http://eecs.oregonstate.edu/education/timeTracker/getPost.php"); //the php file that's the destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //specify content-type header
  
  int httpCode = http.POST(postData); //send request
  String payload = http.getString();  //get response payload

  Serial.println(httpCode); //print http return code
  Serial.println(payload);  //print request response payload
  
  Serial.print("payload length: ");
  Serial.println(payload.length());
  
  http.end(); //close connection

  if(payload.length() > 100){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);  
  }
  
  
  delay(500);
}

/*Compares each axies to see if the
 *position is either at the max or 
 *min (1 or -1)
 */
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

//fills an array with samples (positions)
//not direct data from accel
int FillArray(){
  for(int i=0; i<10; i++){
    samples[i] = PosCalc();
    delay(100);
  }
  return ArrayCheck();
}

//checks to make sure all the values in the array are equal
int ArrayCheck(){
  for(int i=0; i<9; i++){
    if(samples[i] != samples[i+1]){
      return 0;
    }
  }
  return 1;
}
