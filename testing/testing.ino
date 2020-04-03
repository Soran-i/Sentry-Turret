

#include <HCSR04.h>
#include <Servo.h>

// function prototypes 
void mapping(UltraSonicDistanceSensor sensor, double mapp[],int av);
double readDist(UltraSonicDistanceSensor sensor);
bool check(UltraSonicDistanceSensor sensor, double Map []);
void playtune();

// device pins
UltraSonicDistanceSensor leftSensor(10,12); // (trigger, echo)
UltraSonicDistanceSensor rightSensor(11,13);
int BUZZER = 5;
int PIR = 4;

// left and right maps
double mapLeft[19];
double mapRight[19];

// global variables
boolean right = false;
boolean left = false;
      
int pos = 0; // stores current servo heading
int interval = 1; // angle servo increases (*10)
int moveDelay = 100;
int lastdirect = 1;
int maxAng = 18;
int minAng = 0;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);

}

void loop() {
  // put your main code here, to run repeatedly: 
  // do one at a time comment out rest
/*
  // test 1 with serial plotter compares single sensor sweep results with mapped reading on 
for(int i = 0; i < 19; i++){
    double reading = readDist(rightSensor);
    Serial.print(mapRight[i]);
    Serial.print(" ");
    Serial.println(reading);
}

for (pos = 18; pos >= 0; pos -= 1) { 
    servo.write(pos*10);              
    delay(moveDelay);                     
  } 
*/

 

/*
  
  // test 2 serial monitor  prints map over and over showing the variance of results with the ultrasonics
  for(int i = 0; i < 10 ; i++){
    mapping(rightSensor, mapRight, 1);
    printMap( mapRight);
  }
*/
  // test 3 serial monitor prints map over and over showing how the varaince decreases when an average over 50 readings is placesd into map 
  
  for(int i = 0; i < 10 ; i++){
    mapping(rightSensor, mapRight, 50);
  }






}
void printMap(double mapp []){
  Serial.print("{ ");
  for(int i; i <19; i ++){
    
     Serial.print(mapp [i]);
     Serial.print("  ,");
  }

  Serial.print(" } \n");
}
void mapping(UltraSonicDistanceSensor sensor, double mapp [], int av) {
  for (pos = 0; pos <= 18; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree

    servo.write(pos*10);              
    delay(100);

    double data = 0;
    for(int i = 0; i < av; i++){
          data = data + readDist(sensor);
          delay(5);
      }
    
    // averge over 50 readings 
    mapp[pos] = data/av;
  }
  printMap(mapp);
   // back to reset position;
  for (pos = 18; pos >= 0; pos -= 1) { 
    servo.write(pos*10);              
    delay(moveDelay);                     
  } 
}

double readDist(UltraSonicDistanceSensor sensor) {
  double dist = sensor.measureDistanceCm();
  if (dist < 0 || dist > 200)
    dist = 0;
  
  return dist;

}
