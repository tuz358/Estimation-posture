import processing.serial.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.Color;

Serial myPort;
Button reset;
float roll = 0,pitch = 0,yaw = 0;
int NRML = 41,RST = 42;

 
void setup() {
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
 
void draw() {
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
 
void serialEvent(Serial myPort) { 
  String myString = myPort.readStringUntil('\n');
  myString = trim(myString);
  float sensors[] = float(split(myString, ','));
  if (sensors.length > 2) {
    roll = sensors[0];
    pitch = sensors[1];
    yaw = sensors[2];
  }
  myPort.write(NRML);
}

void mouseClicked(){
  myPort.write(RST);
}

class MyActionListener implements ActionListener{
  public void actionPerformed(ActionEvent e){
    myPort.write(RST);
  }
}
