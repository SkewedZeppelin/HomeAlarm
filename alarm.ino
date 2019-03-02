int amtSensors = 1; //Max 12
int pinStatusLED = 13;
int pinBuzzer = 12;
//int pinSiren = 11;
int pinToggleArmed = 10;
int checkDelay = 200;
int passes = 0;
int passesMax = 15; //30 seconds

void setup() { 
  //Serial.begin(9600);
  pinMode(pinStatusLED, OUTPUT); //status LED
  pinMode(pinBuzzer, OUTPUT); //buzzer
  //pinMode(pinSiren, OUTPUT); //siren
  pinMode(pinToggleArmed, INPUT_PULLUP); //momentary switch to toggle armed state
  for(int sp = 0; sp < amtSensors; sp++) {
    pinMode(sp, INPUT_PULLUP); //init a sensor pin
  }
}

void loop() {
  boolean armed = digitalRead(pinToggleArmed) == HIGH;
  digitalWrite(pinStatusLED, HIGH);
  
  int amtSensorsOpen = 0;
  for(int sp = 0; sp < amtSensors; sp++) {
    if(digitalRead(sp) == HIGH) {
      amtSensorsOpen = amtSensorsOpen + 1;
    }
  }
  
  if(amtSensorsOpen > 0) {
    if(armed) {
      //digitalWrite(pinSiren, HIGH);
      digitalWrite(pinBuzzer, HIGH);
    } else {
      digitalWrite(pinBuzzer, HIGH);
      delay(10);
      digitalWrite(pinBuzzer, LOW);
      delay(1000);
    }
  } else {
    if(passes >= passesMax) { //Keep siren on for time after closed
      digitalWrite(pinBuzzer, LOW);
      //digitalWrite(pinSiren, LOW);
      passes = 0;
    } else {
      passes = passes + 1;
    }
  }

  digitalWrite(pinStatusLED, LOW);
  delay(checkDelay);
}
