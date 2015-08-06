#include <IoTkit.h>
#include <Ethernet.h>
#include "MyDefs.h"

//create object for Iot kit
IoTkit iotkit;
DEPTH_SENSOR box1 =   {2,3,4,5};
DEPTH_SENSOR box2 =   {A0,A1,A2,A3};
PRESENCE_SENSOR ir1 = {8,9,10};
int interrupt_pin   = 6;
FILE *fromarduino;
int i = 0;
  
void setup() 
{
  pinMode(interrupt_pin, OUTPUT); //Notification pin
  // Init all sensor pins
  InitUltraSonicPins(&box1);
  InitUltraSonicPins(&box2);
  InitPresenceSensor(&ir1);
  //RegisterSensors();
  
  //iotkit.begin();
  delay(1000);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  char buff[40]={0};
  STORAGE_STS CurrentStatus = {0,0,0};

  fromarduino = fopen ("/arduino_notification_out.txt", "w+");
  //Fetch the data from all sensors
  GetSensorData(&CurrentStatus,&box1,&box2,&ir1);
  
  /*sprintf(buff,"iotkit-admin observation wheat %f",CurrentStatus.item1);
  system(buff);

  sprintf(buff,"iotkit-admin observation rice %f",CurrentStatus.item2);
  system(buff);

  sprintf(buff,"iotkit-admin observation cookies %d",CurrentStatus.item3);
  system(buff);*/
  //send data of sensor
  //iotkit.send("wheat",CurrentStatus.item1);
  //iotkit.send("rice", CurrentStatus.item2);
  //iotkit.send("cookies",CurrentStatus.item3);
  //iotkit.send("wheat",55);

  
  Serial.println("PublishData");
  fprintf(fromarduino, "%f, %f, %d", CurrentStatus.item1, CurrentStatus.item2, CurrentStatus.item3);
  fclose(fromarduino);
  
  //publishData();
  notifyWorld();
  delay(1000);
}
/*
void RegisterSensors()
{
  system("iotkit-admin test");
  system("systemctl stop iotkit-agent");
  system("systemctl start iotkit-agent");

  system("iotkit-admin register wheat wheat.v1.0"); 
  system("iotkit-admin register rice rice.v1.0"); 
  system("iotkit-admin register cookies cookies.v1.0");

}
*/
void GetSensorData(P_STORAGE_STS pStore,P_DEPTH_SENSOR item1,P_DEPTH_SENSOR item2,P_PRESENCE_SENSOR item3)
{
  pStore->item1 = GetPercentage(item1);
  pStore->item2 = GetPercentage(item2);
  pStore->item3 = digitalRead(item3->input);
  //Print sensor data
  Serial.print("box 1: ");
  Serial.print(pStore->item1);
  Serial.print("\tbox 2: ");
  Serial.print(pStore->item2);  
  Serial.print("\tbox 3: ");
  Serial.print(pStore->item3);
  Serial.println();
}

void InitUltraSonicPins(P_DEPTH_SENSOR box)
{
  pinMode(box->vcc,OUTPUT);
  pinMode(box->gnd,OUTPUT);
  pinMode(box->trig,OUTPUT);
  pinMode(box->echo,INPUT);

  digitalWrite(box->vcc,HIGH);
  digitalWrite(box->gnd,LOW);

}

void InitPresenceSensor(P_PRESENCE_SENSOR ir)
{
  pinMode(ir->vcc,OUTPUT);
  pinMode(ir->gnd,OUTPUT);
  pinMode(ir->input,INPUT);

  digitalWrite(ir->vcc,HIGH);
  digitalWrite(ir->gnd,LOW);
}

double GetPercentage(P_DEPTH_SENSOR box)
{
    double duration, inches, cm;
 //Trigger the ultrasonic sensor to send pulse
 
  digitalWrite(box->trig, LOW);
  delayMicroseconds(2);
  digitalWrite(box->trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(box->trig, LOW);

  //after getting echo record the time for which echo pin was high
  duration = pulseIn(box->echo, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  if(cm<20)
  {
    return (((20-cm)/20)*100);
  }
  else
  {
    return 0;
    //Serial.print("OR");
  }
  delay(10);
}

 double microsecondsToCentimeters(double microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  return microseconds / 29 / 2;
}

void publishData()
{
  fromarduino = fopen ("/arduino_notification_out.txt", "w+");
  Serial.println("PublishData");
  i = 10;
  fprintf(fromarduino, "[%d]", i);
  fclose(fromarduino);  
}

//Nofity any body connected to this interrupt  (C++ program and NodeJS) program
void notifyWorld()
{
    digitalWrite(interrupt_pin, HIGH);
    delay(200);
    digitalWrite(interrupt_pin, LOW);
}

