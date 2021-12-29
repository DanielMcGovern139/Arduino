#include <WiFiNINA.h>

char ssid[] = "VMBB6769F";
char pass[] = "Vrrttv5ujjtb";
WiFiServer server(5200);
WiFiClient client;

bool detected = false;

const int motorL1 = 2; // LEFT WHEEL
const int motorL2 = 3;

const int motorR1 = 5; // RIGHT WHEEL
const int motorR2 = 6;

const int L_EYE = 9;
const int R_EYE = 10; //if USB port is on RIGHT ie perspective of buggy


const int US_ECHO = 11;
const int US_TRIG = 12;

long duration;
int distance;

long previousMillis = 0;
long interval = 1000;


void setup() {
  Serial.begin(9600);

  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);

  pinMode(L_EYE, INPUT);
  pinMode(R_EYE, INPUT);

  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);


  WiFi.begin(ssid, pass);
  IPAddress arduino_IP = WiFi.localIP();
  Serial.print("The IP address of the arduino is ");
  Serial.println(arduino_IP);

  server.begin();
}


void loop() {
  WiFiClient client = server.available();

  if (client.connected()) {
    char c = client.read();
    Serial.println(c);

    digitalRead(L_EYE); // Take in the readings
    digitalRead(R_EYE);

    switch (c) {
      case 'g':
        go();
        break;
      case 's':
        Serial.println("I'm stopping");
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
    unsigned long currentMillis = millis();

    digitalRead(L_EYE);
    digitalRead(R_EYE);

    if (digitalRead(L_EYE) == LOW) {
      analogWrite(motorL1, 180);
      analogWrite(motorL2, 0);
    } else {
      analogWrite(motorL1, 0);
      analogWrite(motorL2, 0);
    }

    if (digitalRead(R_EYE) == LOW) {
      analogWrite(motorR1, 160);
      analogWrite(motorR2, 0);
    } else {
      analogWrite(motorR1, 10);
      analogWrite(motorR2, 0);
    }
    if (currentMillis - previousMillis > interval) {
      delayMicroseconds(2);
      digitalWrite(US_TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(US_TRIG, LOW);

      duration = pulseIn(US_ECHO, HIGH);
      distance = duration / 58;
      
      previousMillis = currentMillis;


      while (distance < 25) {
        analogWrite(motorR1, 0);
        analogWrite(motorR2, 0);
        
        analogWrite(motorL1, 0);
        analogWrite(motorL2, 0);

        digitalWrite(US_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(US_TRIG, LOW);

        duration = pulseIn(US_ECHO, HIGH);
        distance = duration / 58;

        server.write(1);
      }
      server.write(2);
    }

WiFiClient client = server.available();
    char c = client.read();
    if (c == 's') {
      break;
    }
  }
}

void off() {
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}
