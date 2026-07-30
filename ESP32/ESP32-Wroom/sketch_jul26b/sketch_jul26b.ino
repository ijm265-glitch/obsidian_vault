// 사용할 GPIO 핀 번호 정의
const int pin1 = 15;
const int pin2 = 2;
const int pin0 = 0;

void setup() {
  // 핀들을 모두 출력(OUTPUT) 모드로 설정
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin0, OUTPUT);

  // 초기 상태: 모든 핀 OFF
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin0, LOW);
}

void loop() {
  // 1. GPIO 15번 ON
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin0, LOW);
  delay(1000); // 1초 대기

  // 2. GPIO 2번 ON
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin0, LOW);
  delay(1000); // 1초 대기

  // 3. GPIO 0번 ON
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin0, HIGH);
  delay(1000); // 1초 대기
}