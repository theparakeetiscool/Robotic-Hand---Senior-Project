// Motor Constants
// gearReductionRatio = the gear ratio between the shaft and the motor
// motor_ppr = the pulses per rotation of the motor
const int gearReductionRatio = 150;
const int motor_ppr = 7;
int shaft_ppr = gearReductionRatio * motor_ppr;

// Motor Driver Direction Pins
// Driver 1

// Motor 1
int md1_in1Pin = 11;
int md1_in2Pin = 12;

// Motor 2
int md1_in3Pin = 8;
int md1_in4Pin = 9;

// Driver 2

// Motor 1
int md2_in1Pin = 5;
int md2_in2Pin = 6;

// Motor 2
int md2_in3Pin = 2;
int md2_in4Pin = 3;


// Encoder Pins
int pulsePin1 = 0;
int pulsePin2 = 0;
int pulsePin3 = 0;
int pulsePin4 = 0;


// Thumb control pin
int thumbPin = A0;

// Arrays to store current finger data
// fingerAngles[] = an array of the 4 finger angles
// fingerForward[] = an array of the directions of the 4 fingers (0 = not moving, 1 = forward, 2 = backwards)
float fingerAngles[] = {0, 0, 0, 0};

int fingerDirection[] = {0, 0, 0, 0};

// Interrupt functions that are called whenever a low to high interrupt is detected
// on each of the finger's encoder pins
// Uses the direction values for each finger to add or remove angular displacement

void finger1_pulse() {
  if(digitalRead(pulsePin1)) {
    if (fingerDirection[0] == 1) {
      fingerAngles[0] += 1/ shaft_ppr;
    }
    else if (fingerDirection[0] == 2) {
      fingerAngles[0] -= 1/ shaft_ppr;
    }
  }
}

void finger2_pulse() {
  if(digitalRead(pulsePin2)) {
    if (fingerDirection[1] == 1) {
      fingerAngles[1] += 1/ shaft_ppr;
    }
    else if (fingerDirection[1] == 2) {
      fingerAngles[1] -= 1/ shaft_ppr;
    }
  }
}

void finger3_pulse() {
  if(digitalRead(pulsePin3)) {
    if (fingerDirection[2] == 1) {
      fingerAngles[2] += 1/ shaft_ppr;
    }
    else if (fingerDirection[2] == 2) {
      fingerAngles[2] -= 1/ shaft_ppr;
    }
  }
}

void finger4_pulse() {
  if(digitalRead(pulsePin4)) {
    if (fingerDirection[3] == 1) {
      fingerAngles[3] += 1/ shaft_ppr;
    }
    else if (fingerDirection[3] == 2) {
      fingerAngles[3] -= 1/ shaft_ppr;
    }
  }
}


// Setup function
// Declares the mode of each pin
void setup() {

  // Motor control pins are declared as ouptut
  pinMode(md1_in1Pin, OUTPUT);
  pinMode(md1_in2Pin, OUTPUT);
  pinMode(md1_in3Pin, OUTPUT);
  pinMode(md1_in4Pin, OUTPUT);

  pinMode(md2_in1Pin, OUTPUT);
  pinMode(md2_in2Pin, OUTPUT);
  pinMode(md2_in3Pin, OUTPUT);
  pinMode(md2_in4Pin, OUTPUT);

  pinMode(thumbPin, OUTPUT);

  // Encoder pins are set to input pullup pins
  // pullup pins default to high (and help avoid floating point errors)
  pinMode(pulsePin1, INPUT_PULLUP);
  pinMode(pulsePin2, INPUT_PULLUP);
  pinMode(pulsePin3, INPUT_PULLUP);
  pinMode(pulsePin4, INPUT_PULLUP);

  // Attaches an interrupt to each encoder pin that is triggered on a rising event
  // Calls the respective pulse function
  attachInterrupt(digitalPinToInterrupt(pulsePin1), finger1_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePin2), finger2_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePin3), finger3_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePin4), finger4_pulse, RISING);

  // Writes all motor pins to low (not moving)
  digitalWrite(md1_in1Pin, LOW);
  digitalWrite(md1_in2Pin, LOW);
  digitalWrite(md1_in3Pin, LOW);
  digitalWrite(md1_in4Pin, LOW);

  digitalWrite(md2_in1Pin, LOW);
  digitalWrite(md2_in2Pin, LOW);
  digitalWrite(md2_in3Pin, LOW);
  digitalWrite(md2_in4Pin, LOW);

  digitalWrite(thumbPin, LOW);

  // Begins serial communication and waits for it to start
  Serial.begin(9600);
  while(!Serial){};
}

// Continously running code
void loop() {
  // put your main code here, to run repeatedly:
  /*
  int size = sizeof(fingerAngles) / sizeof(fingerAngles[0]);
  String anglesAsString = "|";
  for(int i = 0; i < size; i++) {
    anglesAsString += i + " |";
  }
  
  Serial.print(anglesAsString);
  */

  // Checks that serial is available
  
  while (Serial.available()) {
    // Reads the most recent character in serial
    char c = Serial.read();

    // If the character is 'b', stops all motors
    if (c == 'b') {
      digitalWrite(md1_in1Pin, LOW);
      digitalWrite(md1_in2Pin, LOW);
      digitalWrite(md1_in3Pin, LOW);
      digitalWrite(md1_in4Pin, LOW);

      digitalWrite(md2_in1Pin, LOW);
      digitalWrite(md2_in2Pin, LOW);
      digitalWrite(md2_in3Pin, LOW);
      digitalWrite(md2_in4Pin, LOW);

      digitalWrite(thumbPin, LOW);
      continue;
    }
    else if (c == 'p') {
      digitalWrite(thumbPin, HIGH);
      continue;
    }
    

    // Categorizes character inputs as up or down inputs

    char upInputs[] = {'y', 'u', 'i', 'o'};
    char downInputs[] = {'h', 'j', 'k', 'l'};

    // 0 = unchecked, 1 = up, 2 = down
    int isUp = 0;
    int index = 5;

    // Checks if a character is up or down, and then stores the index of the character
    // The index corresponds to which finger is being controlled
    for(int i = 0; i < 4; i++) {
      if(c == upInputs[i]) {
        isUp = 1;
        index = i;
        break;
      }
    }

    if(isUp == 0) {
      for(int i = 0; i < 4; i++) {
        if(c == downInputs[i]) {
          isUp = 2;
          index = i;
          break;
        }
      }
    }

    // Creates variables storing the relevant motor pins for the finger being controlled
    int firstPin;
    int secondPin;
    int currentFinger;

    // Assigns above variables using the index
    if(index == 0) {
      firstPin = md1_in1Pin;
      secondPin = md1_in2Pin;
      currentFinger = 1;
    }
    else if(index == 1) {
      firstPin = md1_in3Pin;
      secondPin = md1_in4Pin;
      currentFinger = 2;
    }
    else if(index == 2) {
      firstPin = md2_in1Pin;
      secondPin = md2_in2Pin;
      currentFinger = 3;
    }
    else if(index == 3) {
      firstPin = md2_in3Pin;
      secondPin = md2_in4Pin;
      currentFinger = 4;
    }



    // Goes through and triggers the pins for each finger 
    // Uses isUp to determine direction
    if(isUp == 0) {
      digitalWrite(firstPin, LOW);
      digitalWrite(secondPin, LOW);
      fingerDirection[currentFinger - 1] = 0;
    }
    else if(isUp == 1) {
      digitalWrite(firstPin, LOW);
      digitalWrite(secondPin, HIGH);
      fingerDirection[currentFinger - 1] = 1;
    }
    else if (isUp == 2) {
      digitalWrite(firstPin, HIGH);
      digitalWrite(secondPin, LOW);
      fingerDirection[currentFinger - 1] = 2;
    }

    // Writes b to serial so that the motors will stop if the key is not continuously pressed down
    Serial.write('b');

    delay(100);
  }
  
}
