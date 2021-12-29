import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;
String data="";
float roll, pitch,yaw;

void setup() {
 size (1000, 1000, P3D);
 myPort = new Serial(this, "COM5", 19200); // starts the serial communication
 myPort.bufferUntil('\n');
    
}

void draw() {
 translate(width/2, height/2, 0);
 background(0);
 
 // Rotate the object
 rotateX(radians(-pitch));
 rotateZ(radians(roll));
 rotateY(radians(yaw));
  

 // 3D 0bject
  

 
 displayBox(100, 100, 100, 380, 40, 200);
 displayBox(310, 175, 100, 40, 190, 200);
 displayBox(135, 121, 100, 310, 80, 80);

 displayWheel(-50, 50, 0, 300);
 displayWheel(-50, 50, 201, 250);

}

// Read data from the Serial Port
void serialEvent (Serial myPort) { 
 // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
 data = myPort.readStringUntil('\n');
 // if you got any bytes other than the linefeed:
 if (data != null) {
  data = trim(data);
  // split the string at "/"
  String items[] = split(data, '/');
  if (items.length > 1) {
   //--- Roll,Pitch in degrees
   roll = float(items[0]);
   pitch = float(items[1]);
     yaw = float(0);
   
  }

 }

}

void displayBox(float x, float y, float z, float r, float s, float t){
  //noStroke();
  pushMatrix();
  translate(x, y, z);
  box(r, s, t);
  popMatrix();
  fill(200, 200, 200);
  noStroke();
}



void displayWheel(int x1, int y1, int z1, int radius1){
   pushMatrix();
   translate(x1, y1, z1);
   circle(x1, y1, radius1);
   popMatrix();
   fill(200, 200, 200);
   noStroke();
}
