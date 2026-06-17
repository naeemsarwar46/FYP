#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display




// Motor pins
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Ultrasonic pins
#define trigFront 3
#define echoFront 2

#define trigLeft 11
#define echoLeft 12

#define trigRight 35
#define echoRight 33

#define LE 9  //  ENABLE LIFT
#define RE 10 //  ENABLE RIGHT

int pwml = 145; //  SPEED145
int pwmr = 120; //  SPEED120

int buzer = 24;
int fan = 27;// fan

int mop_up = 51;// mop up
int mop_dn = 53;// mop dn

char command;
char command2;
int ff = 0;
int fire = 23;
int FIRE;
bool FIRE_SENSOR = false;
bool start = false;
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // in cm
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void moveback() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void moveLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopMoving() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}


void left_turn()
{

  stopMoving(); delay(500); moveLeft(); delay(3000);  stopMoving(); delay(500);
  moveForward(); delay(3000); stopMoving();  delay(500); moveLeft(); delay(3000);




}
void right_turn()

{

  stopMoving(); delay(500);
  moveRight();  delay(3000); stopMoving();  delay(500);
  moveForward(); delay(3200); stopMoving();  delay(500);
  moveRight(); delay(3000);



}
void setup() {
  Wire.begin();

  Wire.beginTransmission(0x27);
  lcd.setBacklight(255);
  lcd.begin(16, 2);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LE, OUTPUT);
  pinMode(RE, OUTPUT);
  pinMode(trigFront, OUTPUT); pinMode(echoFront, INPUT);
  pinMode(trigLeft, OUTPUT); pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT); pinMode(echoRight, INPUT);
  pinMode(fire, INPUT_PULLUP);
  pinMode(buzer, OUTPUT); digitalWrite(buzer, LOW);
  pinMode(mop_up, OUTPUT);
  pinMode(mop_dn, OUTPUT);

  pinMode(fan, OUTPUT);
  digitalWrite(mop_up, HIGH );
  digitalWrite(mop_dn, LOW);
  digitalWrite(fan, HIGH);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Floor Cleaning ");

  lcd.setCursor(0, 1);
  lcd.print("   REBOOT ");

  delay(3000);
  lcd.clear();

}


void loop() {
  BT();
  if (start == true)
  { ff = 1;
    analogWrite(LE, pwml);    analogWrite(RE, pwmr);
    long front = getDistance(trigFront, echoFront);
    long left = getDistance(trigLeft, echoLeft);
    long right = getDistance(trigRight, echoRight);
    /* Serial.print("fir: "); Serial.print(FIRE);
      Serial.print("Front: "); Serial.print(front);
       Serial.print(" | Left: "); Serial.print(left);
       Serial.print(" | Right: "); Serial.println(right);
    */
    if (front < 40) {
      stopMoving(); delay(200);
      if (left > right) {
        // moveLeft(); delay(4000);
        left_turn();
      } else {
        // moveRight(); delay(4000);
        right_turn();
      }
    } else {
      moveForward();
    }

    delay(100);
  }
  FIRE = digitalRead(fire);
  if ((FIRE == 0) && (ff == 1))
  { FIRE_SENSOR = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIRE DETECT");
  }

  if (FIRE_SENSOR == true) {delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("FIRE DETECT");
    start = false;  stopMoving();

    for (int i = 1000; i <= 2000; i += 10) {
      tone(buzer, i);
      delay(2);
    }

    // Descending tone
    for (int i = 2000; i >= 1000; i -= 10) {
      tone(buzer, i);
      delay(2);
    }

    delay(100); // small pause before repeating



  }



}

void BT() {
  if (Serial2.available()) {
    command2 = Serial2.read();
    Serial.println(command2);

    switch (command2) {

      case 'f': start = false; stopMoving(); ff = 0; break; // off

      case 'F': moveForward();
        break;
      case 'r': moveback();
        break;

      case 'L': moveLeft();

        break;

      case 'R': moveRight();
        break;
      case 'S': stopMoving();

        break;
    }
  }
  /*****************************************************************/
  if (Serial1.available()) {
    command = Serial1.read();
    Serial.println(command);

    switch (command) {

      case 'f': start = false; stopMoving(); ff = 0; break; // off
      case 'o': start = true; break;  //on


      case 'X': digitalWrite(fan, LOW); lcd.setCursor(0, 0);
        lcd.print("FAN ON "); break;  // Fan ON
      case 'Y': digitalWrite(fan, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("FAN OFF"); break; // Fan OFF


      case 'U':  // Mop UP
        digitalWrite(mop_up, HIGH);
        digitalWrite(mop_dn, LOW);
        lcd.setCursor(0, 1);
        lcd.print("UP");
        break;
      case 'D':  // Mop DOWN
        digitalWrite(mop_up, LOW);
        digitalWrite(mop_dn, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("DW");
        break;
        /*************************************************************************/


    }
  }
}
