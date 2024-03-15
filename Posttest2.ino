#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "punya gw";
const char* password = "slebew123";

// Define pins
const int BUZZER_PIN = D2;
const int LED_PIN = D1;
const int BUTTON_ENABLE = D5;
const int BUTTON_PLUS = D6;
const int BUTTON_MINUS = D7;

// Define variables
String enableState = "Mati";
int brightness = 0;
int Tone = 100;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_ENABLE, INPUT_PULLUP);
  pinMode(BUTTON_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_MINUS, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // Halaman utama
  server.begin(); // Memulai web server
}

void loop() {
  if (digitalRead(BUTTON_ENABLE) == 0){
    if (enableState == "Mati"){
      enableState = "Nyala";
    } else {
      enableState = "Mati";
    }
    delay(200);
  }

  if (digitalRead(BUTTON_MINUS) == 0){
    if (enableState == "Nyala") {
      if (brightness >= 3){
        brightness -= 3;
      }
      if (Tone >= 110){
        Tone -= 10;
      }
    }
    delay(200);
  }
  
  if (digitalRead(BUTTON_PLUS) == 0){
    if (enableState == "Nyala"){
      if (brightness <= 246){
        brightness += 3;
      }
      if (Tone <= 2490){
        Tone += 10;
      }
    }
    delay(200);
  }

  if (enableState == "Nyala"){
    analogWrite(LED_PIN, brightness);
    tone(BUZZER_PIN, Tone);
  } else {
    analogWrite(LED_PIN, 0);
    noTone(BUZZER_PIN);
  }
  server.handleClient();
}

void handleRoot() {
  String html = "<html>";
  html += "<head><title>POSTTEST 2 IoT</title>";
  html += "<script> window.setTimeout( function() {window.location.reload(); }, 500);";
  html += "setInterval(function() {document.getElementById('enable').textContent ='" + enableState +"';";
  html += "document.getElementById('brightness').textContent ="+ String(brightness) +";";
  html += "document.getElementById('Tone').textContent ="+ String(Tone)+";}, 0);";
  html += "</script></head><body>";
  html += "<h1>Monitoring Rangkaian LED dan Piezo Buzzer</h1>";
  html += "<p>Status Rangkaian: <span id='enable'></span></p>";
  html += "<p>Kecerahan LED: <span id='brightness'></span></p>";
  html += "<p>Tone Buzzer: <span id='Tone'></span></p>";
  html += "</body></html>";
  server.send(200, "text/html", html); // Menampilkan halaman utama
}