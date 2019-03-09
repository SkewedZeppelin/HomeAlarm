#include <LowPower.h>

int amtSensorsDelayed = 1; //Max 6
int amtSensorsInstant = 2; //Max 9

int pinBuzzer = 13;
int pinSiren = 12;
int pinToggleArm = 11;
int pinToggleInstant = 10;

int mode = 0; //2: instant alarm, 1: delayed alarm, 0: chime

int passesDelay = 0;
int passesDelayMax = 5;
int passesQuiet = 0;
int passesQuietMax = 15;

void setup() {
  pinMode(pinBuzzer, OUTPUT); //buzzer
  pinMode(pinSiren, OUTPUT); //siren
  pinMode(pinToggleArm, INPUT_PULLUP);
  pinMode(pinToggleInstant, INPUT_PULLUP);

  for (int sp = 2; sp < amtSensorsInstant + 2; sp++) {
    pinMode(sp, INPUT_PULLUP); //init a sensor pin
  }
}

void loop() {
  int amtOpenSensorsInstant = getOpenSensorsInstant();
  int amtOpenSensorsDelayed = getOpenSensorsDelayed();

  //Allows change when all sensors are closed OR "upgrading"
  if (mode == 0 || (amtOpenSensorsInstant == 0 && amtOpenSensorsDelayed == 0)) {
    mode = getCurrentMode();
  }

  if (mode == 0) { //chime
    if (amtOpenSensorsInstant > 0 || amtOpenSensorsDelayed > 0) {
      digitalWrite(pinBuzzer, HIGH);
      LowPower.idle(SLEEP_30MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
      digitalWrite(pinBuzzer, LOW);
      LowPower.idle(SLEEP_2S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    }
  }

  if (mode == 1) { //delayed alarm
    if (amtOpenSensorsInstant > 0) {
      digitalWrite(pinSiren, HIGH);
      digitalWrite(pinBuzzer, HIGH);
    }
    if (amtOpenSensorsDelayed > 0) {
      if (passesDelay >= passesDelayMax) {
        digitalWrite(pinSiren, HIGH);
        digitalWrite(pinBuzzer, HIGH);
        passesDelay = 0;
      }
      passesDelay = passesDelay + 1;
    }
  }

  if (mode == 2) { //instant alarm
    if (amtOpenSensorsInstant > 0 || amtOpenSensorsDelayed > 0) {
      digitalWrite(pinSiren, HIGH);
      digitalWrite(pinBuzzer, HIGH);
    }
  }

  //Keep alarms on after all sensors closed
  if (amtOpenSensorsInstant == 0 && amtOpenSensorsDelayed == 0) {
    if (passesQuiet >= passesQuietMax) {
      digitalWrite(pinBuzzer, LOW);
      digitalWrite(pinSiren, LOW);
      passesQuiet = 0;
    } else {
      passesQuiet = passesQuiet + 1;
    }
  }
  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}

int getCurrentMode() {
  int mode = 2;
  if (digitalRead(pinToggleArm) == HIGH) {
    if (digitalRead(pinToggleInstant == HIGH)) {
      mode = 2;
    } else {
      mode = 1;
    }
  } else {
    mode = 0;
  }
  return mode;
}

int getOpenSensorsInstant() {
  int amtOpen = 0;
  for (int sp = 2; sp < amtSensorsInstant + 2; sp++) {
    if (digitalRead(sp) == HIGH) {
      amtOpen = amtOpen + 1;
    }
  }
  return amtOpen;
}

int getOpenSensorsDelayed() {
  int amtOpen = 0;
  for (int sp = 0; sp < amtSensorsDelayed; sp++) {
    int sv = analogRead(sp);
    if (sv != 0) {
      amtOpen = amtOpen + 1;
    }
  }
  return amtOpen;
}
