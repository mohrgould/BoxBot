#define pinSteering 10
#define pinThrottle 11
#define pinToggle 12

#define pinStandby 7
#define pinRightPower 6
#define pinRightDir1 8
#define pinRightDir2 9
#define pinLeftPower 3
#define pinLeftDir1 4
#define pinLeftDir2 5

void setup() {
  pinMode(pinSteering, INPUT);
  pinMode(pinThrottle, INPUT);
  pinMode(pinToggle, INPUT);
  
  pinMode(pinRightPower, OUTPUT);
  pinMode(pinRightDir1, OUTPUT);
  pinMode(pinRightDir2, OUTPUT);
  pinMode(pinLeftPower, OUTPUT);
  pinMode(pinLeftDir1, OUTPUT);
  pinMode(pinLeftDir2, OUTPUT);

  pinMode(pinStandby, OUTPUT);
  digitalWrite(pinStandby, HIGH);  

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

int rawThrottle;
int rawSteering;
int rawToggle;

int inThrottle;
int inSteering;
int inToggle;

int newLeft;
int newRight;
int left;
int right;
int maxStep = 1;

int ledState = 0;

unsigned long currentTime;
unsigned long startTime;
unsigned long dt;

void loop() {

  /*
  digitalWrite(pinStandby, HIGH);
  
  currentTime = millis();
  dt = currentTime - startTime;
  
  if (dt > 10000) {
    startTime = currentTime;
    ledState = ledState == 0 ? 1 : 0;
    digitalWrite(LED_BUILTIN, ledState);
  } else if (dt > 8000) {
    // stop
    analogWrite(pinRightPower, 0);
    analogWrite(pinLeftPower, 0);
    digitalWrite(pinRightDir1, HIGH);
    digitalWrite(pinRightDir2, LOW);
    digitalWrite(pinLeftDir1, HIGH);
    digitalWrite(pinLeftDir2, LOW);
  } else if (dt > 6000) {
    // backward
    analogWrite(pinRightPower, 128);
    analogWrite(pinLeftPower, 128);
    digitalWrite(pinRightDir1, LOW);
    digitalWrite(pinRightDir2, HIGH);
    digitalWrite(pinLeftDir1, LOW);
    digitalWrite(pinLeftDir2, HIGH);
  } else if (dt > 4000) {
    // stop
    analogWrite(pinRightPower, 0);
    analogWrite(pinLeftPower, 0);
    digitalWrite(pinRightDir1, LOW);
    digitalWrite(pinRightDir2, HIGH);
    digitalWrite(pinLeftDir1, LOW);
    digitalWrite(pinLeftDir2, HIGH);
  } else {
    // forward
    analogWrite(pinRightPower, 200);
    analogWrite(pinLeftPower, 200);
    digitalWrite(pinRightDir1, HIGH);
    digitalWrite(pinRightDir2, LOW);
    digitalWrite(pinLeftDir1, HIGH);
    digitalWrite(pinLeftDir2, LOW);
  }
  return;
  */

  rawSteering = pulseIn(pinSteering, HIGH, 100000);
  rawThrottle = pulseIn(pinThrottle, HIGH, 100000);
  rawToggle = pulseIn(pinToggle, HIGH, 100000);

  Serial.print("ch1:");
  Serial.print(rawSteering);
  Serial.print("\tch2:");
  Serial.print(rawThrottle);
  Serial.print("\tch3:");
  Serial.print(rawToggle);

  
  if (rawSteering < 900 || rawThrottle < 900 || rawToggle < 900) {
    Serial.println("\tINPUT ERROR **************");
    return;
  }
  
  inSteering = rawSteering - 1500;
  inThrottle = rawThrottle - 1500;
  inToggle = rawToggle - 1500;
  

  left = inThrottle - inSteering;
  right = inThrottle + inSteering;

  newLeft = inThrottle - inSteering;
  newRight = inThrottle + inSteering;

  left = left + constrain(newLeft-left, -maxStep, maxStep);
  right = right + constrain(newRight-right, -maxStep, maxStep);

  Serial.print("\tleft: ");
  Serial.print(left);
  Serial.print("\tright: ");
  Serial.print(right);
  Serial.println("");
  
  digitalWrite(pinLeftDir1, left > 0 ? HIGH : LOW);
  digitalWrite(pinLeftDir2, left > 0 ? LOW : HIGH);
  analogWrite(pinLeftPower, constrain(abs(left) / 2, 0, 200));
  
  digitalWrite(pinRightDir1, right > 0 ? HIGH : LOW);
  digitalWrite(pinRightDir2, right > 0 ? LOW : HIGH);
  analogWrite(pinRightPower, constrain(abs(right) / 2, 0, 200));
  
  //digitalWrite(pinStandby, inToggle<0 ?  LOW : HIGH);
}
