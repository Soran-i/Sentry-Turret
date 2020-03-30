#include <HCSR04.h>
#include <Servo.h>

// function prototypes 
void mapping(UltraSonicDistanceSensor sensor, double mapp[]);
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
  pinMode(PIR, INPUT);
  mapping(leftSensor, mapLeft);
  mapping(rightSensor, mapRight);
  playtune();
}

void loop() {
 
      // while PIR dosesnt detect anyone
      if(digitalRead(PIR) == LOW){
           Serial.println("No one here");
           Serial.println(digitalRead(PIR));
        }
      else{

       Serial.println("Intruder here");
       Serial.println(digitalRead(PIR));

      // pings both ultrasonic senosrs and checks differnce in enviroment
      boolean right = check(rightSensor, mapRight);
      boolean left = check(leftSensor, mapLeft);

      // if both detect differnce dont change posistion
      if( right == true  && left == true ){
            tone(BUZZER,1000, 100);
            interval = 0; 
         }
         
      // if right sensor detects go right
      if (right == true && left == false){ 
            interval = -1; 
        } 

      // if left sensor detects go left  
      if (left == true && right == false){ 
            interval = 1;
        }

      // if no difference continue last heading before a stop
      if( left == false && right == false){
           interval = lastdirect;  
         } 
         
       //up dates position
        pos = pos + interval; 


       // makes sure the new position is within angle bounds
        if(pos <= minAng) {
         interval = 1;
         pos = minAng + 2; 
         }
      
         if(maxAng <= pos) {
         interval =  -1;  
         pos = maxAng -2;
         }


         if(interval != 0){
            lastdirect = interval;
          }

         // move to angle pos
          servo.write(pos*10); 
         
      }


      delay(moveDelay); 
  }


void playtune() {

tone(BUZZER,613, 100); 
  delay(100);

tone(BUZZER,590, 100); 
  delay(100);  

tone(BUZZER,1080, 300); 
  delay(200);  

tone(BUZZER,670, 100); 
  delay(300);

tone(BUZZER,743, 200); 
  delay(100);

tone(BUZZER,850, 300); 
  delay(100);  
  
}

// compares current positions map reading with current readings
// true == difference between mapped and reading
// false == diference between mapped and reading
bool check(UltraSonicDistanceSensor sensor, double Map []){
     int count = 0;
   
     // checks ultrasonic 5 times to elimated noisey results 
     for(int i =0; i<5; i++){
      
     double dist = readDist(sensor);
     
     // if scan result is different from mapped distance
     if(!(dist > (Map[pos]-10) && dist < (Map[pos] + 10))){
        count++;  
     }
     }

     // if all readings is not simular as mapped return true
     if(count == 5){
      return true;
      }
      
     return false;
}

// limits ultrasonic range and eliminates strange results eg negative and way too large numbers
double readDist(UltraSonicDistanceSensor sensor) {
  double dist = sensor.measureDistanceCm();
  if (dist < 0 || dist > 200)
    dist = 0;
  
  return dist;

}

// takes a sensor and maps its readings between 0 and 180 degrees 
void mapping(UltraSonicDistanceSensor sensor, double mapp []) {
  for (pos = 0; pos <= 18; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree

    servo.write(pos*10);              
    delay(100);

    double data = 0;
    for(int i = 0; i < 50; i++){
          data = data + readDist(sensor);
          delay(5);
      }
    
    // averge over 50 readings 
    mapp[pos] = data/50;
  }


  // back to reset position;
  for (pos = 18; pos >= 0; pos -= 1) { 
    servo.write(pos*10);              
    delay(moveDelay);                     
  } 
}
