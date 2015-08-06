

typedef struct ultrasonic_sensor
{
  int vcc,trig,echo,gnd;
  
  }DEPTH_SENSOR,*P_DEPTH_SENSOR;

typedef struct IR_sensor
{
  int vcc,gnd,input;
 }PRESENCE_SENSOR,*P_PRESENCE_SENSOR;

typedef struct StorageStatus
{
  double item1;
  double item2;
  bool item3;
}STORAGE_STS,*P_STORAGE_STS;
