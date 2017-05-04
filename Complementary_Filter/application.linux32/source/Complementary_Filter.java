import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import java.awt.*; 
import java.awt.event.*; 
import java.awt.Color; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Complementary_Filter extends PApplet {






Serial myPort;
Button reset;
float roll = 0,pitch = 0,yaw = 0;
int NRML = 41,RST = 42;

 
public void setup() {
  size(800,600,P3D);
  print(Serial.list());
  myPort = new Serial(this, "COM4", 115200);
  myPort.bufferUntil('\n');

  setLayout(null);
  reset = new Button("RESET");
  reset.setSize(100,50);
  reset.setLocation(600,400);
  reset.addActionListener(new MyActionListener());
  //reset.setFont(new Font("Century",Font.PLAIN,30));
  this.add(reset);
  //reset.repaint();

  strokeWeight(3);  
}
 
public void draw() {
  background(0);
  
  textSize(24);
  text("Attitude Angle",50,120);
  textSize(20);
  text("roll  :",60,150);
  text("pitch  :",60,170);
  text("yaw  :",60,190);
  text(pitch,150,150);
  text(roll,150,170);
  text(yaw,150,190);
  
  translate(width / 2, height / 2);
  rotateX(radians(pitch));//-roll
  rotateY(radians(yaw));//yaw
  rotateZ(radians(roll));//pitch
  box(150, 150, 150);

  reset.show();
}
 
public void serialEvent(Serial myPort) { 
  String myString = myPort.readStringUntil('\n');
  myString = trim(myString);
  float sensors[] = PApplet.parseFloat(split(myString, ','));
  if (sensors.length > 2) {
    roll = sensors[0];
    pitch = sensors[1];
    yaw = sensors[2];
  }
  myPort.write(NRML);
}

public void mouseClicked(){
  myPort.write(RST);
}

class MyActionListener implements ActionListener{
  public void actionPerformed(ActionEvent e){
    myPort.write(RST);
  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Complementary_Filter" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
