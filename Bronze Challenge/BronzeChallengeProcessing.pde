import controlP5.*;
import processing.net.*;
ControlP5 cp5;
Client myClient;
String data; 
int previous_message = 0;

void setup(){
  size(500,220);
  cp5 = new ControlP5(this);
  myClient = new Client(this, "192.168.2.6" , 5200);
  
  cp5.addButton("GO")
      .setPosition(50,50)
      .setSize(200, 150);
      
  cp5.addButton("STOP")
      .setPosition(270, 50)
      .setSize(200, 150);
}


void draw(){
  while(myClient.available() > 0){
    int obi = myClient.read();
    
    if(obi == 1 && obi != previous_message){
      println("Obstacle detected");
    }
    if(obi == 2 && obi != previous_message){
      println("Its all clear");
    }     
    previous_message = obi;
  }
}


public void GO(){
   if(myClient.active() == true ) {
    myClient.write("g");
    println("The buggy is going");
  }
}


public void STOP(){
    if(myClient.active() == true){
      myClient.write("s");
      myClient.write("s");
      println("The buggy has stopped"); 
  }
}
