void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //  pinMode(A0, INPUT);
  //  pinMode(A1, INPUT);
  //  pinMode(A2, INPUT);
  //  pinMode(A3, INPUT);
  pinMode(1, OUTPUT);
}

void loop() {


  digitalWrite(1, HIGH);


  Serial.println("Left-X");
  Serial.println(analogRead(A0));

  Serial.println("Left-Y");
  Serial.println(analogRead(A1));


  Serial.println("Left-X");
  Serial.println(analogRead(A2));

  Serial.println("Left-Y");
  Serial.println(analogRead(A3));

  Serial.println("DIGITAL LEFT");
  Serial.println(digitalRead(2));


  Serial.println("DIGITAL RIGHT");
  Serial.println(digitalRead(4));
  delay(1000);
}
