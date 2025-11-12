unsigned long prevTimeLED1 = 0, LED1BlinkDelay = 1000;
unsigned long prevTimeLED2 = 0, LED2BlinkDelay = 1000;
byte LED1State = LOW, LED2State = LOW;

int chosenLED = 0, chosenDelay = 0;


void taskLED1();
void taskLED2();
void taskSerialInput();


typedef void (*TaskPtr)();
TaskPtr taskTable[] = { taskSerialInput, taskLED1, taskLED2 };
const int NUM_TASKS = sizeof(taskTable) / sizeof(TaskPtr);


void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  prevTimeLED1 = millis();
  prevTimeLED2 = millis();
}


void loop() {
  // Round-robin task scheduling
  for (int i = 0; i < NUM_TASKS; i++) {
    taskTable[i]();  // Call the next task
  }
}


void taskSerialInput() {
  if (Serial.available() > 0) {
    chosenLED = getNumber("What LED? (1 or 2)");
    chosenDelay = getNumber("What interval (in msec)?");

    if (chosenLED == 1)
      LED1BlinkDelay = chosenDelay;
    else if (chosenLED == 2)
      LED2BlinkDelay = chosenDelay;
  }
}


void taskLED1() {
  unsigned long now = millis();
  if (now - prevTimeLED1 >= LED1BlinkDelay) {
    prevTimeLED1 = now;
    LED1State = !LED1State;
    digitalWrite(8, LED1State);
  }
}


void taskLED2() {
  unsigned long now = millis();
  if (now - prevTimeLED2 >= LED2BlinkDelay) {
    prevTimeLED2 = now;
    LED2State = !LED2State;
    digitalWrite(9, LED2State);
  }
}


int getNumber(const char* prompt) {
  Serial.println(prompt);
  while (Serial.available() == 0) { } // Wait
  int number = Serial.parseInt();
  while (Serial.available() > 0) Serial.read();
  return number;
}
