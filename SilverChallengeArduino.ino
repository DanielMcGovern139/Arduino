#include <PID_v1.h>
#include <WiFiNINA.h>

char ssid[] = "eir77447448-2.4G";

char pass[] = "x2qzxtps";
WiFiServer server(5200);
WiFiClient client;
char command;
double repeating_input = 100;


const int motorL1 = 2; // LEFT WHEEL
const int motorL2 = 3;
const int motorR1 = 5; // RIGHT WHEEL
const int motorR2 = 6;

const int L_EYE = 9;
const int R_EYE = 10; //if USB port is on RIGHT ie perspective of buggy

const int US_ECHO = 11;
const int US_TRIG = 12;

double setPoint = 30;
double Input, Output;

double Kp = 11;
double Ki = 16; // 17/18 good also
double Kd = 1;

PID myPID(&Input, &Output, &setPoint, Kp, Ki, Kd, DIRECT);




void setup() {
  Serial.begin(9600);

  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);

  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);

  //Initialise the variables we're linked to
  Input = analogRead(US_ECHO);
  //Turn PID on
  myPID.SetMode(AUTOMATIC);

  WiFi.begin(ssid, pass);
  IPAddress arduino_IP = WiFi.localIP();
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client.connected()) {
    command = client.read();
    Serial.println(command);
    switch (command) {
      case 'g':
        go();
        break;
      case 's':
        off();
        break;
      default:
        off();
    }
  }
}


void go() {
  bool running1 = true;
  while (running1) {
    sendPing();
    PIDCommand();
    if (Input < 15 || Output < 0) { // Stopping distance from the object
      off();
      server.write(1); 
    }
    else {
      if(Input < 30 && Input != repeating_input){
        server.write(Input); //Only report if obstacle comes in 30cm proximity
        Serial.println(Input);
      }
      repeating_input = Input;
      
      if (digitalRead(R_EYE) == LOW) {
        if (Output == 0) { // Condition satisfied when far away from object
          analogWrite(motorR1, 250);
          analogWrite(motorR2, 0);
        }
        else {
          analogWrite(motorR1, 260 - Output); // Causes the differential speed. Is set to 260 and not 255 so that it can slowly trail up to stopping distance of stationary object
          analogWrite(motorR2, 0);
        }
      }
      else if (digitalRead(R_EYE) == HIGH) {
        analogWrite(motorR1, 0);
        analogWrite(motorR2, 0);
      }
      if (digitalRead(L_EYE) == LOW) {
        if (Output == 0) { // Condition satisfied when far away from object
          analogWrite(motorL1, 250);
          analogWrite(motorL2, 0);
        }
        else {
          analogWrite(motorL1, 260 - Output); // Causes the differential speed. Is set to 260 and not 255 so that it can slowly trail up to stopping distance of stationary object
          analogWrite(motorL2, 0);
        }
      }
      else if (digitalRead(L_EYE) == HIGH) {
        analogWrite(motorL1, 0);
        analogWrite(motorL2, 0);
      }
    }
    WiFiClient client = server.available();
    command = client.read();
    if (command == 's') {
      break;
    }
  }
}


void PIDCommand() {
  Input = (pulseIn(US_ECHO, HIGH) / 58);
  myPID.Compute();
}


void sendPing() {
  digitalWrite(US_TRIG, LOW);
  delay(2);
  digitalWrite(US_TRIG, HIGH);
  delay(10);
  digitalWrite(US_TRIG, LOW);
}



void off() {
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
  analogWrite(motorR1, 0);
  analogWrite(motorL2, 0);
}
