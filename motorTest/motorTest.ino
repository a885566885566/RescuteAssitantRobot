#include <SoftwareSerial.h>
#include <Servo.h>

#define leftDir 7
#define leftPower 6
#define rightDir 4
#define rightPower 5

#define beeperPin 13
#define servoPin 9
#define baseSpeed 70
#define headAngleOffset 2

#define POWER 200
#define TURN_POWER 100
#define TURN_TIME 320
Servo headServo;
SoftwareSerial bt(10,11);  //re tx

void setup() {
    bt.begin(38400);
    pinMode(leftDir, OUTPUT);
    pinMode(leftPower, OUTPUT);
    pinMode(rightDir, OUTPUT);
    pinMode(rightPower, OUTPUT);
    pinMode(beeperPin, OUTPUT);
    headServo.attach(servoPin);
    moveHead(90);
    beep(100);
}

void loop() {
    if(bt.available()){
        switch(bt.read()){
            case 'a':
                script1();
                break;
            case 's':
                script2();
                break;
            case 'd':
                script3();
                break;
        }
    }
}
void script1(){
    moveHead(90);
    beep(100);
    for(int i=0;i<4;i++){
        travel(POWER, 1000);
        delay(1000);
        turnLeft();
        delay(1000);
        clearBT();
    }
    beep(1000);
}

void script2(){
    beep(100);
    for(int i=0;i<2;i++){
        travel(POWER, 1000);
        headTurnLeft();
        headTurnRight();
        delay(500);
        turnLeft();
        travel(POWER, 400);
        turnLeft();
        travel(POWER, 1000);
        headTurnLeft();
        headTurnRight();
        delay(500);
        turnRight();
        travel(POWER, 400);
        turnRight();
        clearBT();
    }
    stopMotor();
    beep(1000);
}

void script3(){
    beep(100);
    moveHead(90);
    delay(1500);
    turnLeft();
    travel(POWER, 1000);
    for(int i=0;i<10;i++){
        beep(100);
        delay(500);
        clearBT();
    }
}
void clearBT(){
    while(bt.available())
    bt.read();
}
void stopMotor(){
    digitalWrite(leftDir, LOW);
    digitalWrite(leftPower, LOW);
    digitalWrite(rightDir, LOW);
    digitalWrite(rightPower, LOW);
}
void moveHead(int angle){
    angle -= headAngleOffset;
    delay(100);
    angle = (angle>180)?180:angle;
    angle = (angle<0)?0:angle;
    headServo.write(angle);
    delay(100);
    //headServo.detach();
}
void headTurnLeft(){
    moveHead(15);
    delay(1000);
}
void headTurnRight(){
    moveHead(170);
    delay(1000);
}
void turnLeft(){
    move(TURN_POWER, -1*TURN_POWER);
    delay(TURN_TIME);
    stopMotor();
    delay(500);
}
void turnRight(){
    move(-1*TURN_POWER, TURN_POWER);
    delay(TURN_TIME);
    stopMotor();
    delay(500);
}
void move(int left, int right){
    digitalWrite(leftDir, left>=0);
    digitalWrite(rightDir, right>=0);
    analogWrite(leftPower, abs(left));
    analogWrite(rightPower, abs(right));
}
void travel(int avrSpeed, float dis){
    int direct = 1;
    if(avrSpeed<0){
        avrSpeed *= -1;
        direct = -1;
    }
    for(float s = 0;s < 0.3*dis; s+=10){
        float v = s*(avrSpeed-baseSpeed)/(0.3*dis) + baseSpeed;
        v *= direct;
        move(v, v);
        delay(10);
    }
    move(avrSpeed, avrSpeed);
    delay(0.4*dis);
    for(float s = 0.7*dis;s < dis; s+=10){
        float v = (dis-s)*(avrSpeed-baseSpeed)/(0.3*dis) + baseSpeed;
        v *= direct;
        move(v, v);
        delay(10);
    }
    stopMotor();
    delay(500);
}
void beep(int interval){
    digitalWrite(beeperPin, HIGH);
    delay(interval);
    digitalWrite(beeperPin, LOW);
}

