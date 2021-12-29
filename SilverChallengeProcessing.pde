import controlP5.*;
import processing.net.*;
ControlP5 GUI;
Client myClient;
Textarea serverMessages;

int data; 
int previous_message = 0;
PFont fmessages;
Println console;


void setup() {
  size(1000, 500);
  background(0, 0, 0);
  GUI = new ControlP5(this);
  myClient = new Client(this, "192.168.2.5", 5200);

  GUI.addButton("GO")
    .setPosition(80, 50)
    .setSize(400, 150)
    .setColorBackground(color(0, 200, 0))
    .setFont(createFont("Arial", 100));

  GUI.addButton("STOP")
    .setPosition(500, 50)
    .setSize(400, 150)
    .setColorBackground(color(200, 0, 0))
    .setFont(createFont("Arial", 100));


  fmessages = createFont("Arial", 20, true);
  serverMessages = GUI.addTextarea("text area")
      .setPosition(75, 275)
      .setSize(850, 100)
      .setFont(fmessages)
      .setLineHeight(25)
      .setColor(color(255, 255, 255))
      .setColorBackground(color(189, 45, 200));
      
 console = GUI.addConsole(serverMessages);
}


void draw() {
  //Displat text on GUUI 
  textFont(fmessages, 20);
  fill(150);
  text("Incoming messages:", 75, 260);
  fill(150);
  text("PID Coefficients are Kp = 11, Ki = 16, Kd = 1", 75, 460);
  
  if(myClient.available() > 0){
    data = myClient.read();
    if(data != previous_message){
      if(data == 1){
        println("Obstacle detected 15 cm away!!! The buggy has stopped!");
      }
      else{
        print("Obtacle detected. Obstacle is ");
        print(data);
        println(" cm away from me"); 
      }
    }
    previous_message = data;
  }
}


public void GO() {
  if (myClient.active() == true ) {
    myClient.write("g");
    println("The buggy is going");
  }
}


public void STOP() {
  if (myClient.active() == true) {
    myClient.write("s");
    myClient.write("s");
    println("The buggy has stopped");
  }
}
