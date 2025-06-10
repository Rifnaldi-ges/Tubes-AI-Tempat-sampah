#include <Arduino.h>
#include <ESP32Servo.h>

// --- Pengaturan ---
// Deklarasi tiga objek servo untuk setiap jenis sampah
Servo servoMetal;
Servo servoKertas;  // Paper
Servo servoPlastik;

// Tentukan pin GPIO untuk setiap servo
const int PIN_SERVO_METAL = 13;
const int PIN_SERVO_KERTAS = 12;
const int PIN_SERVO_PLASTIK = 14; // Ganti ke pin GPIO yang tersedia

// Tentukan posisi sudut untuk setiap servo
const int POSISI_AWAL = 0;   // Posisi netral/awal
const int POSISI_BUANG = 90; // Posisi saat membuang sampah, sesuaikan jika perlu

// --- Deklarasi Fungsi (Function Prototype) ---
// Memberitahu compiler bahwa fungsi ini ada sebelum digunakan di loop()
void gerakkanServo(Servo &servoObj);

void setup() {
  // Mulai komunikasi serial
  Serial.begin(115200);
  
  // Sambungkan objek servo ke pin yang sudah ditentukan
  servoMetal.attach(PIN_SERVO_METAL);
  servoKertas.attach(PIN_SERVO_KERTAS);
  servoPlastik.attach(PIN_SERVO_PLASTIK);

  // Atur semua servo ke posisi awal saat pertama kali dinyalakan
  servoMetal.write(POSISI_AWAL);
  servoKertas.write(POSISI_AWAL);
  servoPlastik.write(POSISI_AWAL);

  Serial.println("Sistem pemilah sampah siap.");
  Serial.println("Kirim perintah melalui Serial Monitor:");
  Serial.println(" 'M' untuk Metal");
  Serial.println(" 'K' untuk Kertas (Paper)");
  Serial.println(" 'L' untuk Plastik");
}

void loop() {
  // Cek apakah ada data yang masuk dari port serial
  if (Serial.available() > 0) {
    // Baca perintah yang masuk (satu karakter)
    char perintah = Serial.read();

    // Cetak perintah yang diterima untuk debugging
    Serial.print("Perintah diterima: ");
    Serial.println(perintah);

    // Logika untuk menggerakkan servo yang sesuai berdasarkan perintah
    if (perintah == 'R' || perintah == 'r') {
      Serial.println("-> Memilah METAL");
      gerakkanServo(servoMetal);
    } 
    else if (perintah == 'L' || perintah == 'l') {
      Serial.println("-> Memilah KERTAS");
      gerakkanServo(servoKertas);
    } 
    else if (perintah == 'P' || perintah == 'p') {
      Serial.println("-> Memilah PLASTIK");
      gerakkanServo(servoPlastik);
    }
    else {
      // Abaikan karakter yang tidak dikenali seperti newline atau spasi
      if (perintah != '\n' && perintah != '\r') {
        Serial.println("-> Perintah tidak dikenali.");
      }
    }
  }
}

/**
 * @brief Menggerakkan servo yang diberikan ke posisi buang, lalu kembali ke posisi awal.
 * @param servoObj Objek servo yang akan digerakkan.
 */
void gerakkanServo(Servo &servoObj) {
  servoObj.write(POSISI_BUANG); // Gerakkan servo ke posisi buang
  delay(2000);                   // Beri jeda waktu agar sampah jatuh
  servoObj.write(POSISI_AWAL);  // Kembalikan servo ke posisi awal
  delay(300);                    // Jeda singkat sebelum siap menerima perintah baru
  Serial.println("Sistem siap untuk item berikutnya.");
}
