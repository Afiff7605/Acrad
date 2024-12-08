#include <AFMotor.h>
#include <Servo.h>

//servo (pin 9) dan motor (M4 dan M3)
Servo servo; 
AF_DCMotor motor1(1);  // Motor kiri (M4)
AF_DCMotor motor4(4);  // Motor kanan (M3)

int Speed = 150; // Kecepatan motor
char value;      
int mode =0; // Manual mode

// Pin sensor IR
#define IR_LEFT1 A0  // Sensor IR kiri 1 (AO)
#define IR_RIGHT1 A1 // Sensor IR kanan 1 (A1)
#define IR_LEFT2 A2  // Sensor IR kiri 2 (A2)
#define IR_RIGHT2 A3 // Sensor IR kanan 2 (A3)

void setup() {
  Serial.begin(9600);  
  servo.attach(9);     // servo ke pin 9
  servo.write(80);     // posisi awal servo
  motor1.setSpeed(Speed);
  motor4.setSpeed(Speed-18);  // Kurangkan laju motor sebelah kiri umtuk samakan voltage 

  pinMode(IR_LEFT1, INPUT);  // Mengatur pin sensor kiri1 sebagai input
  pinMode(IR_RIGHT1, INPUT); // Mengatur pin sensor kanan1 sebagai input
  pinMode(IR_LEFT2, INPUT);  // Mengatur pin sensor kiri2 sebagai input
  pinMode(IR_RIGHT2, INPUT); // Mengatur pin sensor kanan2 sebagai input
}

// Fungsi untuk membuka gripper
void Left() {
  for (int a = 80; a > 50; a--) { 
    servo.write(a);  // Mengubah posisi servo
    delay(30);       // Memberikan delay untuk pergerakan
  }
}

// Fungsi untuk menutup gripper
void Lstart() {
  for (int a = 50; a < 80; a++) {  
    servo.write(a);  // Mengubah posisi servo
    delay(30);       // Memberikan delay untuk pergerakan
  }
}
void loop() {  // tekan button sebelah kiri dalam apps SriTuHobby untuk mengawal tutup dan buka gripper
  // Membaca data serial dari aplikasi
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }
if (value == '3') {  // tekan button sebelah kanan dalam apps SriTuHobby untuk ubah ke line tracer mode
    mode=1;}
    else if (value =='4') 
    mode=0;


if(mode==1) LineTracer(); // (line tracer mode)

if(mode==0){ // (manual mode)
  // Kontrol robot berdasarkan perintah yang diterima dari serial
  if (value == 'U') {
    forward();  // Maju
  } else if (value == 'D') {
    backward(); // Mundur
  } else if (value == 'L') {
    turnLeft(); // Belok kiri
  } else if (value == 'R') {
    turnRight(); // Belok kanan
  } else if (value == 'T') {
    turnLeft();  // Putar kiri
  } else if (value == 'F') {
    turnRight(); // Putar kanan
  } else if (value == 'H') {
    sideLeft();  // Geser kiri
  } else if (value == 'G') {
    sideRight(); // Geser kanan
  } else if (value == '1') {
    Left();      // Servo terbuka
  } else if (value == '2') {
    Lstart();    // Servo tertutup
  } else {
    Stop();      // Berhenti
  }
}}


// Fungsi untuk bergerak maju
void forward() {
  motor1.run(FORWARD);
  motor4.run(FORWARD);
}

// Fungsi untuk bergerak mundur
void backward() {
  motor1.run(BACKWARD);
  motor4.run(BACKWARD);
}

// Fungsi untuk putar kanan
void turnRight() {
  motor1.run(FORWARD);
  motor4.run(BACKWARD);
}

// Fungsi untuk putar kiri
void turnLeft() {
  motor4.run(FORWARD);
  motor1.run(BACKWARD);
}

// Fungsi untuk geser kiri
void sideLeft() {
  motor1.run(FORWARD);
  motor4.run(RELEASE);
}

// Fungsi untuk geser kanan
void sideRight() {
  motor1.run(BACKWARD);
  motor4.run(RELEASE);
}

// Fungsi untuk berhenti
void Stop() {
  motor1.run(RELEASE);
  motor4.run(RELEASE);
}

// Fungsi Line Tracing
// 0 (warna putih)
// 1 (warna hitam)
void LineTracer() {
  int l1 = digitalRead(IR_LEFT1);  // Baca sensor kiri1
  int r1 = digitalRead(IR_RIGHT1); // Baca sensor kanan1
  int l2 = digitalRead(IR_LEFT2);  // Baca sensor kiri2
  int r2 = digitalRead(IR_RIGHT2); // Baca sensor kanan2

  if (l2 ==0 && l1 ==0 && r1 ==0 && r2 ==0) {
    forward(); // Maju jika kedua sensor mendeteksi garis
  } 
  else if (l2 ==1 && l1 ==1 && r1 ==0 && r2 ==0) {
    turnLeft(); // Belok kiri
  } 
  else if (l2 ==0 && l1 ==0 && r1 ==1 && r2 ==1) {
    turnRight(); // Belok kanan
  } 
    else if (l2 ==1 && l1 ==0 && r1 ==0 && r2 ==0) {
    turnLeft(); // Belok kiri
  } 
  else if (l2 ==0 && l1 ==0 && r1 ==0 && r2 ==1) {
    turnRight(); // Belok kanan
  } 
    else if (l2 ==0 && l1 ==1 && r1 ==0 && r2 ==0) {
    sideLeft(); // Belok kanan
  } 
  else if (l2 ==0 && l1 ==0 && r1 ==1 && r2 ==0) {
    sideRight(); // Belok kanan
  } 
  
  else {
    Stop(); // Berhenti jika ada garis hitam
  }
}

 // ============================

 // COMMANDS FROM SRITUHOBBY APP
 // UP-U
 // DOWN-D
 // LEFT-L
 // RIGHT-R
 // STOP-S
 // UP+LEFT-T
 // UP+RIGHT-F
 // DOWN+LEFT-H
 // DOWN+RIGHT-G
 // CHANNEL 1 ON-1 (LEFT BUTTON ON)
 // CHANNEL 1 OFF-2 (LEFT BUTTON OFF)
 // CHANNEL 2 ON-3 (RIGHT BUTTON ON)
 // CHANNEL 2 OFF-4 (RIGHT BUTTON OFF)

 // =============================

