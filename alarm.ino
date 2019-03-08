#include <LowPower.h>


int amtSensors = 1;
int pinBuzzer = 13;
int pinSiren = 12;
int pinToggleArmed = 10;

int passes = 0;
int passesMax = 5;

boolean armed = true;

void setup() {
  pinMode(pinBuzzer, OUTPUT); //buzzer
  pinMode(pinSiren, OUTPUT); //siren
  pinMode(pinToggleArmed, INPUT_PULLUP); //switch to toggle armed state
  for(int sp = 0; sp < amtSensors; sp++) {
    pinMode(sp, INPUT_PULLUP); //init a sensor pin
  }
}

void loop() {
  int amtSensorsOpen = 0;
  for(int sp = 0; sp < amtSensors; sp++) {
    if(digitalRead(sp) == HIGH) {
      amtSensorsOpen = amtSensorsOpen + 1;
    }
  }

  if(amtSensorsOpen > 0) {
    if(armed) {
      digitalWrite(pinSiren, HIGH);
      digitalWrite(pinBuzzer, HIGH);
    } else {
      digitalWrite(pinBuzzer, HIGH);
      LowPower.idle(SLEEP_30MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
      digitalWrite(pinBuzzer, LOW);
      LowPower.idle(SLEEP_2S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    }
  } else {
    if(passes >= passesMax) { //Keep siren on for time after closed
      digitalWrite(pinBuzzer, LOW);
      digitalWrite(pinSiren, LOW);
      passes = 0;
    } else {
      passes = passes + 1;
    }

    //Only allow changing armed state when all sensors are closed
    armed = digitalRead(pinToggleArmed) == HIGH;
  }

  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}
