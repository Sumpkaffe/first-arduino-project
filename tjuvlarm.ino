int ledPin = 13;
int pirPin = 2;
int knappPin = 7;
int buzz = 9;
int pirStat = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(knappPin, INPUT);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool meas = digitalRead(pirPin);
  bool knapp = digitalRead(knappPin);
  Serial.println(String(meas) + "      " + String(int(millis()/500)%2) );

  if (meas == HIGH) {
    pirStat = HIGH;
  }
  
  if (pirStat == HIGH) {
    if (int(millis()/500)%2){
      digitalWrite(ledPin, HIGH);
      tone(buzz, 1550);
    } else {
      digitalWrite(ledPin, LOW);
      noTone(buzz);
    }
  }

  if (knapp == HIGH) {
    digitalWrite(ledPin, LOW);
    noTone(buzz);
    pirStat = LOW;
  }
}
