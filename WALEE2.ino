#include <HCSR04.h>
#include <Servo.h>

void mapping();
double readDist(UltraSonicDistanceSensor sensor);

double mapLeft[19];
double mapRight[19];
int pos = 0;  

UltraSonicDistanceSensor leftSensor(10,12);
UltraSonicDistanceSensor rightSensor(11,13);

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);
  mapping();
}

void loop() {
  // put your main code here, to run repeatedly:
    for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree

    double reading = readDist(rightSensor);
    Serial.print(mapRight[pos]);
    Serial.print(" ");
    Serial.println(reading);

    servo.write(pos); 
    delay(600); 
  }
  
  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(600);                       // waits 15ms for the servo to reach the position
  }

}

double readDist(UltraSonicDistanceSensor sensor) {
  double dist = sensor.measureDistanceCm();
  delay(10);
  if (dist < 0 || dist > 200)
    dist = 0;
  return dist;

}

void mapping() {
  for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
mapLeft[pos] = readDist(leftSensor);
    
    delay(100);
    mapRight[pos] = readDist(rightSensor);
    
    
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
