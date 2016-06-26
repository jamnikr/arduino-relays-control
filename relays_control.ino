int BTNPIN1 = 4;
int BTNPIN2 = 3;
int LINE1 = 5;
int LINE2 = 6;
int PIR = 12;

unsigned long timeToDisable1Line = 0;
unsigned long timeToDisable2Line = 0;

boolean line1Enable = true;
boolean line2Enable = true;

void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(BTNPIN1, INPUT);
  pinMode(BTNPIN2, INPUT);
  pinMode(LINE1, OUTPUT);
  pinMode(LINE2, OUTPUT);
  pinMode(PIR, INPUT);
  digitalWrite(PIR, LOW);

  //give the sensor some time to calibrate
  Serial.println("Sensor Calibration in Progress");
  Serial.println("------------------------------");
  for (int i = 0; i < 30; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("Sensor Calibration Completed");
  Serial.println("Sensor Reading Active");

}

void loop() {
  manageFirstLine();
  delay(100);
  manageSecondLine();
  delay(100);
}

void manageFirstLine() {
  bool line1AllTime = digitalRead(BTNPIN1) == 1; // w dol

  //do nothing
  if (line1AllTime == true && line1Enable == true) {
    return;
  }
  //turn on first line
  if (line1AllTime == true && line1Enable == false) {
    enableFirstLine();
    return;
  }

  //auto
  if (timeToDisable1Line > 0) {
    Serial.print("Time to turn off 1 line " );
    Serial.print((timeToDisable1Line - millis()) / 1000);
    Serial.println(" s");
  } else {
    Serial.println("Time to turn off 1 line - now" );
  }

  //czy już wyłączyć, był wczesniej ruch i trzeba wylaczyc
  if (timeToDisable1Line < millis() && line1Enable == true) {
    disableFirstLine();
    return;
  }
  Serial.println("Move detection");
  //jest ruch
  if (digitalRead(PIR) == HIGH) {
    Serial.println("Move detected");
    //pobierz z potencjometru, od 0 do 10minut
    unsigned long lightingTime = map(analogRead(A1), 0, 1023, 0, 600000);
    timeToDisable1Line = lightingTime + millis();
    enableFirstLine();
    return;
  }
}

void manageSecondLine() {
  bool line2AllTime = digitalRead(BTNPIN1) == 1; // w dol

  //nic nie rób
  if (line2AllTime == true && line2Enable == true) {
    return;
  }
  //włącz linię 2
  if (line2AllTime == true && line2Enable == false) {
    enableSecondLine();
    return;
  }

  //jeśli świeci światło to wyłącz
  if (((1023 - analogRead(A0)) / 10.23) > 50) {
    if (line2Enable == false) {
      return;
    }
    disableSecondLine();
    return;
  }

  //automatycznie
  if (timeToDisable2Line > 0) {
    Serial.print("Time to turn off 2 line" );
    Serial.print((timeToDisable2Line - millis()) / 1000);
    Serial.println(" s");
  } else {
    Serial.println("Time to turn off 2 line- now" );
  }

  //czy już wyłączyć, był wczesniej ruch i trzeba wylaczyc
  if (timeToDisable2Line < millis() && line2Enable == true) {
    disableSecondLine();
    return;
  }

  //jest ruch
  if (digitalRead(PIR) == HIGH) {

    //pobierz z potencjometru, od 0 do 10minut
    unsigned long lightingTime = map(analogRead(A1), 0, 1023, 0, 600000);
    timeToDisable2Line = lightingTime + millis();
    enableSecondLine();
    return;
  }
}

void enableFirstLine() {
  line1Enable = true;
  digitalWrite(LINE1, LOW);//sterowanie na low
  delay(1000);
}

void disableFirstLine() {
  line1Enable = false;
  digitalWrite(LINE1, HIGH);//sterowanie na low
  delay(1000);
}

void enableSecondLine() {
  line2Enable = true;
  digitalWrite(LINE2, LOW);//sterowanie na low
  delay(1000);
}

void disableSecondLine() {
  line2Enable = false;
  digitalWrite(LINE2, HIGH);//sterowanie na low
  delay(1000);
}


