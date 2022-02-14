#include <ESP8266WiFi.h>
#include <Adafruit_PWMServoDriver.h>
// NOTES REGARDING THE SERVOS
/* This project uses continuous servos for the wheels
 *  for the right servo the PWM value which makes it stop is 305, for the left servo it is 306. Looks like there is a margin of error of 1 here. Kind of annoying but to match the two servos
 *  subtract/add 1
 */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define CONTROLPORT 8888
#define SERVOMIN  110
#define SERVOMAX  500
#define SERVO_FREQ 50

uint8_t servoLeft = 15;
uint8_t servoRight = 14;
/* replace the SSID and SSID pass with your credentials */
const char ssid[] = "--SSID--";
const char ssid_pass[] = "--PASSWORD--";

int CurrentServo = 0;

WiFiServer ControlServer(CONTROLPORT);

void setup() {
  Serial.begin(115200);
  /* Connecting to WiFi */
  WiFi.begin(ssid, ssid_pass);
  int timeout = 0;
  Serial.println(); // Clear the new line
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    timeout += 1;
    if (timeout >= 100) {
      Serial.println("Attempting to connect for > 10 seconds\nSomething may be wrong...");
    }
  }
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Control Server Port: ");
  Serial.println(CONTROLPORT);
  ControlServer.begin();
  /* PCA9685 Settings */
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
}

double angleToServoRange(double degree){
  double pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  return pulseLen;
}

void loop() {
  String Data;
  String c_char;
  WiFiClient c_client = ControlServer.available();
  if(c_client){
    while(c_client.connected()){
        while(c_client.available() > 0){ // fix potential overflow error. Current code serves as a POC
            c_char = c_client.read();
            if(c_char == "\n" and Data.length() >= 1){
                int movement = Data.toInt(); // will default to 0 if no valid conversion
                Data= "";
                if(CurrentServo){
                  CurrentServo = 0;
                  pwm.setPWM(servoRight, 0, angleToServoRange(movement));
                  } 
                else{
                  CurrentServo = 1;
                  pwm.setPWM(servoLeft, 0, angleToServoRange(movement));
                  }
                
              }
            else{
              Data+=c_char;
            }
              
          }
          
      }
      c_client.stop();
   }
    
}
