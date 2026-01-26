 #include <Servo.h>

Servo myservo1;
Servo myservo2;

int servoPin1 = 9;
int servoPin2 = 10;
int angle = 90;
int angleStep = 5;

#define LEFT 2
#define RIGHT 3

void setup() {
  Serial.begin(9600);
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  Serial.println("Robojax Servo 2 Buttons");
  myservo1.write(0);  // tell servo to go to position in variable 'pos'
  myservo2.write(90);
}

void loop() {
  if (digitalRead(RIGHT) == LOW) {

    myservo1.write(90);  // tell servo to go to position in variable 'pos'
    myservo2.write(0);

   //delay(400);
  }

  if (digitalRead(LEFT) == LOW) {

    myservo1.write(0);  // tell servo to go to position in variable 'pos'
    myservo2.write(90);

    //delay(1000);
  }
}