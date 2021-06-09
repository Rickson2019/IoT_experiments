void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
    int i = 10000000000;
    while( i >= 0){
      i--;
      move_one_sec();
    }
//  move_one_sec();
}

void move_one_sec() {
  digitalWrite(3, LOW);
  digitalWrite(3, HIGH);
  delayMicroseconds(30);
}
