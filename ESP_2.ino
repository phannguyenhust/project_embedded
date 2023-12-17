#include <ESP32Servo.h>
#include <Ticker.h>

const int trigPin1 = 25;
const int echoPin1 = 26;
const int trigPin2 = 27;
const int echoPin2 = 14;

const int trigPin3 = 32;
const int echoPin3 = 33;

const int trigPin4= 13;
const int echoPin4 = 18;


Servo servo1;  
Servo servo2;
Servo servo3;  
Servo servo4;
Servo servo5;


float distance1, distance2, distance3, distance4;
//trang thai servo
bool status_open_1 = 0;
bool status_open_2 = 0; 
bool status_open_3 = 0;
bool status_open_4 = 0; 

//bien check khoang cach
bool is_distance = true;
//bien xem nguoi dung co dua pin vao khong
bool bool_wait_check_bug1 = true;
//cua vua mo
String recent_open =""; 
int status_monitor = 1;
//wait nhan loai pin
bool wait_get_type = 1;
//receive loai pin
char name_pin = 'o';

//bien cho cho pin vao;
bool wait_user_put = true ;
//bien cho lay pin ra
bool bool_wait_user_get_pin_moi = true ;

//bien doi check rfid
bool bool_wait_check = true;
char check_rfid_true = 'c';


//bien doi nguoi dung lay pin cu ra
bool bool_wait_user_get_pin_cu = true;
//bien reset
bool is_reset = false;
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  servo1.attach(15);  
  servo2.attach(5);
  servo3.attach(4);  
  servo4.attach(2);
  servo5.attach(23);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);
}
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;  // Convert to centimeters
  return distance;
}
void setServo(Servo servo, int goc) {
  servo.write(goc);
  Serial.println(goc);
  delay(3000);
}
//doi nhan loai pin
void wait_signal(){
  while(wait_get_type){
    if (Serial2.available() > 0) {
      char receivedChar = (char)Serial2.read();
      name_pin = receivedChar;
      if(name_pin == '1' || name_pin == '2'){
        Serial.println("da nhan tin hieu 1");
        wait_get_type = 0;
      }
    }
  }
}
//ham cho nguoi dung cho pin vao
void wait_user_put_pin_cu(){
  unsigned long startTime = millis(); 
  while(wait_user_put){
    unsigned long currentTime = millis();
    if(currentTime - startTime < 10000){
      if (getDistance(trigPin1,echoPin1) < 18 && status_open_1 == 1){
      delay(3000);
      servo1.write(0);
      delay(3000);
      status_open_1 =0;
      Serial2.write('1');
      Serial.println("da gui 2 cho esp1");
      wait_user_put=false;
      } 
      else if (getDistance(trigPin2,echoPin2) < 18 && status_open_2 == 1){
        delay(1000);
        servo2.write(0);
        delay(3000);
        status_open_2 =0;
        Serial2.write('2');
        Serial.println("da gui rfid2 cho esp1");
        wait_user_put=false;
      } 
    }
    else {
      Serial.println("da qua 10 giay");
      Serial2.write('3');
      choose_servo_to_close_bug1();
      delay(5000);
      Serial.println("dang delay 10 giay");
      bool_wait_check_bug1 = false;
      status_monitor = 5;
      wait_user_put = false;
    }
    delay(100);
  }
}
void choose_servo_to_close_bug1(){
  if(recent_open == "servo_1"){
    servo1.write(0);
    status_open_1 = 0;
  }
  else if(recent_open == "servo_2"){
    servo2.write(0);
    status_open_2 = 0;
  }
}
void wait_check(){
  while(bool_wait_check){
    if (Serial2.available() > 0) {
      char receivedChar = (char)Serial2.read();
      check_rfid_true = receivedChar;
      Serial.println(check_rfid_true);
      Serial.println("da nhan duoc 1");
      if(check_rfid_true == '1'){
        Serial.println("truong hop tra pin cu");
        status_monitor = 3;
        bool_wait_check = 0;
      }  
      else if(check_rfid_true == '2'){
        Serial.println("truong hop tra pin moi");
        status_monitor = 4;
        bool_wait_check = 0;
      }
    }
  }
}
void wait_user_get_pin_cu(){
  unsigned long startTime = millis(); 
  while(bool_wait_user_get_pin_cu){
    unsigned long currentTime = millis();
    if(currentTime - startTime < 10000){
      if(getDistance(trigPin1, echoPin1) >18 && status_open_1 == 1){
      delay(1000);
      servo1.write(0);
      Serial2.write('9');
      bool_wait_user_get_pin_cu = false;
      status_monitor = 5;
    }
      else if(getDistance(trigPin2, echoPin2) >18 && status_open_2 == 1){
        delay(1000);
        servo2.write(0);
        Serial2.write('9');
        bool_wait_user_get_pin_cu = false;
        status_monitor = 5;
      }
    }
    else {
      Serial.println("da qua 10 giay");
      Serial2.write('3');
      choose_servo_to_close_bug1();
      servo5.write(90);
      delay(5000);
      Serial.println("dang delay 10 giay");
      servo5.write(0);
      status_monitor = 5;
      bool_wait_user_get_pin_cu = false;
    }
    delay(100);
  }
}
void wait_user_get_pin_moi(){
  unsigned long startTime = millis(); 
  while(bool_wait_user_get_pin_moi){
    unsigned long currentTime = millis();
    if(currentTime - startTime < 10000){
      if(getDistance(trigPin2,echoPin2) >18 && status_open_2 == 1){
      delay(3000);
      servo2.write(0);
      Serial2.write('9');
      Serial.write("da gui so 9");
      status_open_2 = 0;
      bool_wait_user_get_pin_moi = false;
      } 
      else if(getDistance(trigPin1,echoPin1) >18 && status_open_1 == 1){
        delay(3000);
        servo1.write(0);
        Serial2.write('9');
        status_open_1 = 0;
        bool_wait_user_get_pin_moi = false;
      } 
    }
    else {
      if(recent_open == "servo_1"){
        servo1.write(0);
      }
      else if(recent_open == "servo_2"){
        servo2.write(0);
      }
      Serial.println("da qua 10 giay");
      Serial2.write('3');
      servo5.write(90);
      delay(5000);
      Serial.println("dang delay 5 giay");
      servo5.write(0);
      status_monitor = 5;
      bool_wait_user_get_pin_moi = false;
    }
    delay(100);
  }
}
void choose_servo_to_open_pin_cu(){
  if(recent_open == "servo_1"){
    servo1.write(90);
    status_open_1 = 1;
  }
  else if(recent_open == "servo_2"){
    servo2.write(90);
    status_open_2 = 1;
  }
}
void choose_servo_to_open_pin_moi(){
  if(recent_open == "servo_1"){
    servo2.write(90);
    recent_open = "servo_2";
    status_open_2 = 1;
  }
  else if(recent_open == "servo_2"){
    servo1.write(90);
    recent_open = "servo_1";
    status_open_1 = 1;
  }
}
void check_type_pin_to_open(){
  if(name_pin == '1'){
    if (distance1 > 18 && distance2 < 18 && status_open_1 == 0){
      servo1.write(90);
      delay(1000);
      Serial2.write('1');
      status_open_1 = 1;
      recent_open = "servo_1";
      Serial.println("UpdateA");
      status_monitor = 2;
    }
    else if (distance1 < 18 && distance2 > 18 && status_open_2 == 0){
      servo2.write(90);
      delay(1000);
      Serial2.write('1');
      status_open_2 = 1;
      recent_open = "servo_2";
      Serial.println("UpdateA");
      status_monitor = 2;
    }
  }
  else if(name_pin == '2'){
    if (distance3 > 18 && distance4 < 18 && status_open_3 == 0){
      servo3.write(90);
      delay(1000);
      Serial2.write('1');
      status_open_3 = 1;
      recent_open = "servo_3";
      Serial.println("UpdateB");
      status_monitor = 2;
    }
    else if (distance1 < 18 && distance2 > 18 && status_open_4 == 0){
      servo4.write(90);
      delay(1000);
      Serial2.write('1');
      status_open_4 = 1;
      recent_open = "servo_4";
      Serial.println("UpdateB");
      status_monitor = 2;
    }
  }
}
int count = 1;
void loop() {
  while(is_distance){
    delay(500);
    distance1 = getDistance(trigPin1, echoPin1);
    delay(500);
    distance2 = getDistance(trigPin2, echoPin2);
    delay(500);
    distance3 = getDistance(trigPin3, echoPin3);
    delay(500);
    distance4 = getDistance(trigPin4, echoPin4);
    delay(500);
    if((distance1 > 18 && distance2 < 18 && distance2 != 0) || (distance1 < 18 && distance2 > 18 && distance1 != 0)){
      Serial.print("Distance from Sensor 1: ");
      Serial.print(distance1);
      Serial.println(" cm");
      Serial.print("Distance from Sensor 2: ");
      Serial.print(distance2);
      Serial.println(" cm");
      delay(1000);
      is_distance = false;
    }
  }
  // doi tin hieu tu esp 1, cho phep kiem tra o pin trong de mo
  if(status_monitor == 1){
    if (count == 2){
      Serial.println(" Nhay vao lan 2");
    }
    wait_signal();
    check_type_pin_to_open();
    count = 2;
  }
  // doi nguoi dung dua pin vao 
  if(status_monitor == 2){
    Serial.println("da vao monitor 2");
    // dong servo, servo ma chua pin vua moi duoc dua vao
    wait_user_put_pin_cu();
    if(bool_wait_check_bug1){
      wait_check();
      bool_wait_check_bug1 == false;
    }
  }
  if(status_monitor == 3 ){
    choose_servo_to_open_pin_cu();
    wait_user_get_pin_cu();
  }
  Serial.println("done");
  if(status_monitor == 4 ){
    choose_servo_to_open_pin_moi();
    wait_user_get_pin_moi();
    Serial.println("het so 4");
    status_monitor = 5;
  }
  if(status_monitor == 5 ){
    is_reset = true;
  }
  if(is_reset){
    status_open_1 = 0;
    status_open_2 = 0; 
    status_open_3 = 0;
    status_open_4 = 0; 
    recent_open =""; 
    status_monitor = 1;
    wait_get_type = 1;
    name_pin = 'o';
    wait_user_put = true ;
    bool_wait_user_get_pin_moi = true ;
    bool_wait_check = true;
    check_rfid_true = 'c';
    bool_wait_user_get_pin_cu = true;
    is_reset = false;
    bool_wait_check_bug1 = true;
    is_distance = true;
  }
}