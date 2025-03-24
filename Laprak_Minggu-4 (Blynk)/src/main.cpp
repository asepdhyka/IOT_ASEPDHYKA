#define BLYNK_DEVICE_NAME "LED_Control"
#define BLYNK_PRINT Serial

#define DHT_PIN 15 
#define LED_PIN 26 

#define BLYNK_AUTH_TOKEN "sOiBwnZ-_fYjZqyq71eVxb8QJUAYIkE4"
#define BLYNK_TEMPLATE_ID "TMPL6_8Q0Yf19"
#define BLYNK_TEMPLATE_NAME "LED_Control"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h> //Library untuk DHT

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST"; //nama hotspot yang digunakan
char pass[] = ""; //password hotspot yang digunakan

DHTesp dht;
BlynkTimer timer;

//function untuk pengiriman sensor
void sendSensor() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data DHT22!");
    return;
  }

  Blynk.virtualWrite(V0, temperature); //mengirimkan data temperature ke Virtual pin VO di Blynk Cloud
  Blynk.virtualWrite(V1, humidity); //mengirimkan data humidity ke Virtual pin V1 di Blynk Cloud

  //menampilkan temperature pada Serial monitor
  Serial.print("% Temperature: ");
  Serial.print(temperature);
  Serial.println("C ");
  Serial.print("% Humidity: ");
  Serial.print(humidity);
  Serial.println("% ");
}

BLYNK_WRITE(V2) {
  int ledState = param.asInt(); 
  digitalWrite(LED_PIN, ledState);
  Serial.println(ledState ? "LED ON" : "LED OFF");
  Blynk.virtualWrite(V3, ledState);
}

void setup() {
  // Debug console
  Serial.begin(115200); //serial monitor menggunakan bautrate 9600
  
  Blynk.begin(auth, ssid, pass); //memulai Blynk
  dht.setup(DHT_PIN, DHTesp::DHT22);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  timer.setInterval(1000, sendSensor); //Mengaktifkan timer untuk pengiriman data 1000ms
}

void loop() {
  Blynk.run(); //menjalankan blynk
  timer.run(); //menjalankan timer
}
