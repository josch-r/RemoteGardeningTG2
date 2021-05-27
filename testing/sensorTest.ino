#define sensorPin A0
int sensorVal = 0;
int moisturePercentage = 0;
int dryVal = 400;
int wetVal = 150;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorVal = analogRead(sensorPin);
  //Serial.println(sensorVal);
  moisturePercentage = map(sensorVal, dryVal, wetVal, 0, 100);
  if(moisturePercentage > 100){
    moisturePercentage = 100;
  }
  Serial.print(moisturePercentage);
  Serial.println(" %");
  delay(1000);
}
