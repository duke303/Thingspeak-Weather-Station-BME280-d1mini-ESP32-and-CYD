#include <Wire.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <BME280I2C.h>

// Konfigurierbare I2C-Pins
const int I2C_SDA = 21;  // SDA-Pin
const int I2C_SCL = 22;  // SCL-Pin

// Deep Sleep Intervall (in Sekunden)
const int sleepInterval = 1800;  // 30Minute

// ThingSpeak
const char* ssid = "xxxxxx";
const char* password = "xxxxxx";
const char* host = "api.thingspeak.com";
const unsigned long channelNumber = "xxxxxx";
const char* apiKey = "xxxxxx";

// BME280
BME280I2C bme;

// Funktionen
void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  // Konfiguriere I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialisiere den BME280-Sensor
  if (!bme.begin()) {
    Serial.println("Fehler beim Initialisieren des BME280 Sensors!");
    while (1);
  }

  // Verbinde mit WiFi
  connectWiFi();

  // Lese Sensordaten
  float temperature, humidity, pressure;
  bme.read(pressure, temperature, humidity);

  // Sende Daten an ThingSpeak
  WiFiClient client;
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, pressure);
  ThingSpeak.writeFields(channelNumber, apiKey);

  // Ausgabe der Daten
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Gehe in den Deep Sleep-Modus
  Serial.println("Going to sleep...");
  ESP.deepSleep(sleepInterval * 1000000);
}

void loop() {
  // Der Code im Loop wird nicht verwendet, da der ESP32 in den Deep Sleep-Modus geht
}
