#include <Servo.h>
#include <HX711.h>
#include <AccelStepper.h>

AccelStepper stepper1(1, 20, 21);
Servo servos[12];
#define relay1 34
#define relay2 35
#define relay3 36
#define relay4 37
#define relay5 38
#define relay6 39
#define relay7 40
#define relay8 41
#define Enable 44
int A = 50;  //power on delay
int B = 500; //cycle delay
int candyCount[6] = { 0, 0, 0, 0, 0, 0 };
const int servoPins[12] = { 14, 15, 16, 17, 18, 19, 8, 9, 10, 11, 12, 7 };
const int sensorPins[6] = { 2, 3, 4, 5, 6, 13 };
const int DT[6] = { 22, 24, 26, 28, 30, 32 };
const int SCKPin[6] = { 23, 25, 27, 29, 31, 33 };  // Renamed to avoid conflict
const float CALIBRATION_FACTORM[6] = { -1790, -1790, -1790, -1790, -1790, -1790 };
int P = 0;
int D = -900;
HX711 scales[6];
const float CALIBRATION_FACTOR = -476.592;
int command = 0;
String a;
String T;
int prev = 0;

//
void setup() {
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  pinMode(Enable, OUTPUT);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
  digitalWrite(Enable, HIGH);
  Serial.begin(9600);

  for (int i = 0; i < 12; i++) {
    servos[i].attach(servoPins[i]);
  }
  digitalWrite(relay3, LOW);
  delay(A);
  servos[0].write(170);
  servos[6].write(180);
  delay(B);
  servos[0].write(180);
  servos[6].write(90);
  delay(B);
  digitalWrite(relay3, HIGH);
  delay(A);
  digitalWrite(relay4, LOW);
  delay(A);
  servos[1].write(170);
  servos[7].write(180);
  delay(B);
  servos[1].write(180);
  servos[7].write(90);
  delay(B);
  digitalWrite(relay4, HIGH);
  delay(A);
  digitalWrite(relay5, LOW);
  delay(A);
  servos[2].write(170);
  servos[8].write(180);
  delay(B);
  servos[2].write(180);
  servos[8].write(90);
  delay(B);
  digitalWrite(relay5, HIGH);
  delay(A);
  digitalWrite(relay6, LOW);
  delay(A);
  servos[3].write(170);
  servos[9].write(180);
  delay(B);
  servos[3].write(180);
  servos[9].write(90);
  delay(B);
  digitalWrite(relay6, HIGH);
  delay(A);
  digitalWrite(relay7, LOW);
  delay(A);
  servos[4].write(170);
  servos[10].write(180);
  delay(B);
  servos[4].write(180);
  servos[10].write(90);
  delay(B);
  digitalWrite(relay7, HIGH);
  delay(A);
  digitalWrite(relay8, LOW);
  delay(A);
  servos[5].write(170);
  servos[11].write(180);
  delay(B);
  servos[5].write(180);
  servos[11].write(90);
  delay(B);
  digitalWrite(relay8, HIGH);
  delay(A);


  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  stepper1.setMaxSpeed(1000);
  delay(500);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  for (int i = 0; i < 6; i++) {

    /*
        remove this
        1 - bad reading
        2 - reading ok
        3 - not working
        4 - reading ok
        5 - not working
        6 - not working
    */

    //
    if (i == 0 || i == 2 || i == 4 || i == 5)
    {
      continue;
    }

    //
    scales[i].begin(DT[i], SCKPin[i]);
    scales[i].set_scale(CALIBRATION_FACTORM[i]); //This value is obtained by using the SparkFun_HX711_Calibration sketch
    scales[i].tare();
  }
}

//
void loop()
{
  //digitalWrite(relay3, LOW);  if(digitalRead(sensorPins[0]) == 0) { Serial.println("ON"); } if(digitalRead(sensorPins[0]) == 1) { Serial.println("OFF");} // sensor test
  while (Serial.available())
  {
    String a = Serial.readStringUntil('\n');

    //
    if (a.indexOf("count") >= 0)
    {
      Serial.println("a");
      // count
      String str = a.substring(6);
      char strArray[16]; // Make sure the array is large enough to hold the string
      str.toCharArray(strArray, sizeof(strArray));

      // Prepare an array to store the split integers
      int i = 0; // Index for the numbers array

      // Tokenize the string using strtok and convert to integers
      char* token = strtok(strArray, ",");
      while (token != NULL) {
        candyCount[i++] = atoi(token);
        token = strtok(NULL, ",");
      }
    }

    //
    else
    {
      //
      command = a.toInt();
      Serial.println(a);
      Serial.println(D);

      // candy 1 [not working]
      if (command == 1)
      {
        D = -900;
        digitalWrite(relay3, LOW);
        digitalWrite(Enable, LOW);
        prev = 1;

        while (digitalRead(sensorPins[0]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        delay(1000);

        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }

        servos[0].write(90);
        delay(5000);
        servos[0].write(150);
        delay(5000);

        for (int i = 90; i <= 130; i++) {
          servos[6].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {
          servos[6].write(i);
          delay(10);
        }
        P = 1;
        delay(1000);
        digitalWrite(relay3, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      // candy 2 [working]
      if (command == 2)
      {
        if (P == 0) {
          D = -1800;
        }
        if (P == 1) {
          D = -900;
        }
        digitalWrite(relay4, LOW);
        digitalWrite(Enable, LOW);
        prev = 2;

        Serial.println("WARN1");

        // sensor
        while (digitalRead(sensorPins[1]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();

          Serial.println("WARN2");
        }


        Serial.println("tanga");
        delay(1000);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();

          Serial.println("WARN3");
        }

        Serial.println("WARN4");

        // takep? old
        //servos[1].write(90);
        //delay(2000);
        //servos[1].write(170);
        //delay(2000);

        // takep? new
        Serial.println("WARN5");
        // open
        servos[1].write(90);
        delay(1000);

        //
        bool isweight = false;
        while (!isweight)
        {
          if (scales[1].get_units() >= candyCount[1])
          {
            Serial.println("WARN6");
            // close
            servos[1].write(170);
            delay(1000);
            isweight = true;
          }
          else
          {

          }

          Serial.println(scales[1].get_units());
        }


        // tapon?
        for (int i = 90; i <= 130; i++) {
          servos[7].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {

          servos[7].write(i);
          delay(10);
        }
        P = 2;
        delay(1000);
        digitalWrite(relay4, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      // candy 3 [not working]
      if (command == 3)
      {
        if (P == 0) {
          D = -2700;
        }
        if (P == 1) {
          D = -1800;
        }
        if (P == 2) {
          D = -900;
        }
        digitalWrite(relay5, LOW);
        digitalWrite(Enable, LOW);
        prev = 3;
        while (digitalRead(sensorPins[2]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        delay(1000);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        servos[2].write(90);
        delay(2000);
        servos[2].write(170);
        delay(2000);
        for (int i = 90; i <= 130; i++) {
          servos[8].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {

          servos[8].write(i);
          delay(10);
        }
        P = 3;
        delay(1000);
        digitalWrite(relay5, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      // candy 4 [working]
      if (command == 4)
      {
        if (P == 0) {
          D = -3600;
        }
        if (P == 1) {
          D = -2700;
        }
        if (P == 2) {
          D = -1800;
        }
        if (P == 3) {
          D = -900;
        }
        digitalWrite(relay6, LOW);
        digitalWrite(Enable, LOW);
        prev = 4;

        // sensor?
        while (digitalRead(sensorPins[3]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        delay(1000);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }

        // takep? old
        //servos[3].write(90);
        //delay(2000);
        //servos[3].write(170);
        //delay(2000);

        // takep? new
        Serial.println("WARN5");
        // open
        servos[3].write(90);
        delay(1000);

        //
        bool isweight = false;
        while (!isweight)
        {
          if (scales[3].get_units() >= candyCount[3])
          {
            Serial.println("WARN6");
            // close
            servos[3].write(170);
            delay(1000);
            isweight = true;
          }
          else
          {

          }

          Serial.println(scales[3].get_units());
        }

        // tapon?
        for (int i = 90; i <= 130; i++) {
          servos[9].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {

          servos[9].write(i);
          delay(10);
        }
        P = 4;
        delay(1000);
        digitalWrite(relay6, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      // candy 5 [not working]
      if (command == 5)
      {
        if (P == 0) {
          D = -4500;
        }
        if (P == 1) {
          D = -3600;
        }
        if (P == 2) {
          D = -2700;
        }
        if (P == 3) {
          D = -1800;
        }
        if (P == 4) {
          D = -900;
        }
        digitalWrite(relay7, LOW);
        digitalWrite(Enable, LOW);
        prev = 5;
        while (digitalRead(sensorPins[4]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        delay(1000);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        servos[4].write(90);
        delay(2000);
        servos[4].write(170);
        delay(2000);
        for (int i = 90; i <= 130; i++) {
          servos[10].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {

          servos[10].write(i);
          delay(10);
        }
        P = 5;
        delay(1000);
        digitalWrite(relay7, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      // candy 6 [not working]
      if (command == 6)
      {

        if (P == 0) {
          D = -5400;
        }
        if (P == 1) {
          D = -4500;
        }
        if (P == 2) {
          D = -3600;
        }
        if (P == 3) {
          D = -2700;
        }
        if (P == 4) {
          D = -1800;
        }
        if (P == 5) {
          D = -900;
        }
        digitalWrite(relay8, LOW);
        digitalWrite(Enable, LOW);
        prev = 6;
        while (digitalRead(sensorPins[5]) == 0) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        delay(1000);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != D) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
        servos[5].write(90);
        delay(2000);
        servos[5].write(170);
        delay(2000);
        for (int i = 90; i <= 130; i++) {
          servos[11].write(i);
          delay(10);
        }
        delay(1000);
        for (int i = 130; i >= 90; i--) {

          servos[11].write(i);
          delay(10);
        }
        delay(1000);
        digitalWrite(relay8, HIGH);
        digitalWrite(Enable, HIGH);
      }  //end

      if (command == 7)
      {
        //
        candyCount[0] = 0;
        candyCount[1] = 0;
        candyCount[2] = 0;
        candyCount[3] = 0;
        candyCount[4] = 0;
        candyCount[5] = 0;

        //
        digitalWrite(Enable, LOW);
        if (prev == 1) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -5800) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }

        if (prev == 2) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -4900) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }

        if (prev == 3) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -4000) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }

        if (prev == 4) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -3100) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }

        if (prev == 5) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -2200) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }

        if (prev == 6) {
          stepper1.setCurrentPosition(0);
          while (stepper1.currentPosition() != -1300) {
            stepper1.setSpeed(-400);
            stepper1.runSpeed();
          }
        }
      }

      if (command == 8) {
        delay(3000);
        digitalWrite(Enable, LOW);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != 6500) {
          stepper1.setSpeed(400);
          stepper1.runSpeed();
        }
      }

      if (command == 9) {
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, LOW);

        delay(5000);

        digitalWrite(relay1, LOW);
        digitalWrite(relay2, HIGH);


        delay(5000);

        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(Enable, LOW);
        stepper1.setCurrentPosition(0);
        while (stepper1.currentPosition() != -300) {
          stepper1.setSpeed(-400);
          stepper1.runSpeed();
        }
      }
    }
  }
}
