#include <SharpIR.h>

//infrarouge pin
#define IRPinLeft A3
#define IRPinRight A2

#define left 0
#define right !left

#define ligneCapteurLeft 6
#define ligneCapteurRight 2 

#define minDis 20 //la distance minimum ultrason
#define minDisInfrarouge 20//la distance minimum Infrarouge
#define maxDis 150 //la distance maximum ultrason
#define sec 1200  //delai
#define vSon 59 //valeur de temps en µs d'un aller retour du son sur 1cm

//********//

bool running_ = true;
int vitesse =0;
int turningDelay =50;
int turningSpeed =125;

//

SharpIR sensorLeft( SharpIR::GP2Y0A02YK0F, IRPinLeft );
SharpIR sensorRight( SharpIR::GP2Y0A02YK0F, IRPinRight );

//structure 
struct Motor{
  int brake,direction_,speed_;
  } ;
struct Motor motor2,motor1;


void setup() {
  Serial.begin(9600);
  initMotor(&motor1,9,12,3);
  initMotor(&motor2,8,13,11);
  pinMode(ligneCapteurRight,INPUT);
  pinMode(ligneCapteurLeft,INPUT);
}
void initMotor(Motor* motor,int br,int dir,int spd){
  motor->brake = br;
  motor->direction_= dir;
  motor->speed_ = spd;
  
  pinMode(motor->brake, OUTPUT); //Initiates Motor Channel A pin
  pinMode(motor->direction_, OUTPUT); //Initiates Brake Channel A pin
  pinMode(motor->speed_, OUTPUT); //Initiates Brake Channel A pin
}

void loop() {
  int leftCap = digitalRead(ligneCapteurLeft);
  int rightCap = digitalRead(ligneCapteurRight);
  Serial.print("Left :");
  Serial.print(leftCap);
  Serial.print(",right :");
  Serial.println(rightCap);
  delay(200);

  avancer(vitesse);
  if(leftCap){
    tourner(0,left,turningSpeed);
    while((leftCap = digitalRead(ligneCapteurLeft))){}
  }else if (rightCap){
    tourner(0,right,turningSpeed);
    while((rightCap = digitalRead(ligneCapteurRight))){}
  }
}

void avancer(int vitesse){
  digitalWrite(motor1.brake,LOW);
  digitalWrite(motor2.brake,LOW);
  digitalWrite(motor1.direction_,HIGH);
  digitalWrite(motor2.direction_,HIGH);
  digitalWrite(motor1.speed_,vitesse);
  digitalWrite(motor2.speed_,vitesse);
}
/*
int getSoundDis(){
  int sum = 0;
  int iters = 5;
  for(int i=0;i<iters;i++){
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
   
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    int dur = pulseIn(echoPin,HIGH);
    delay(50);
    sum+=dur;
  }
  sum /= iters;
  return sum/vSon;  //vSon :valeur de temps pour parcourir 1cm en microseconde
}
*/
void arreter(){
  digitalWrite(motor1.brake,HIGH);
  digitalWrite(motor2.brake,HIGH);
}
void moveBack(int vitesse,int disRight,int disLeft){
  while(disRight <minDisInfrarouge && disLeft<minDisInfrarouge){
      digitalWrite(motor1.brake,LOW);
      digitalWrite(motor2.brake,LOW);
      digitalWrite(motor1.direction_,LOW);
      digitalWrite(motor2.direction_,LOW);
      digitalWrite(motor1.speed_,vitesse);
      digitalWrite(motor2.speed_,vitesse);
      disRight=sensorRight.getDistance();
      disLeft = sensorLeft.getDistance();
  }
  if(disRight < disLeft){
      tourner(turningDelay,left,turningSpeed);
      arreter();
  }
  else{
    tourner(turningDelay,right,turningSpeed);
    arreter();
  }
}

void tourner(int duration,int dir,int vitesse){
  digitalWrite(motor1.brake,LOW);
  digitalWrite(motor2.brake,LOW);
  digitalWrite(motor1.direction_,dir?HIGH:LOW);
  digitalWrite(motor2.direction_,!dir?HIGH:LOW);
  digitalWrite(motor1.speed_,vitesse);
  digitalWrite(motor2.speed_,vitesse);
  delay(duration);
}
