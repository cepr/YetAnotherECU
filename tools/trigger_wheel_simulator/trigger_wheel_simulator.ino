/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  Serial.begin(115200);
}

byte repeat = 100;

// the loop function runs over and over again forever
void loop() {
  if (Serial.available()) {
    byte b = Serial.read();
    if (b=='+') {
      repeat++;
    } else if (b=='-') {
      repeat--;
    } else if (b >= '0' and b <= '9') {
      repeat = (b - '0') * 10;
    }
  }
  for (int j = 0; j < repeat; j++) {
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  }
  for (int j = 0; j < repeat; j++) {
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  }
  for (int i = 0; i < 35; i++) {
    for (int j = 0; j < repeat; j++) {
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
    }
    for (int j = 0; j < repeat; j++) {
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
    }
  }
}
