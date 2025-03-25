#include <TFT_eSPI.h>  // TFT_eSPI Bibliothek 
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "xxxxxxx";
const char* password = "xxxxxxx";

// ThingSpeak API settings
const char* outdoorApiKey = "xxxxxxxx";
const char* outdoorChannelID = "xxxxxxxx";
const char* serverName = "http://api.thingspeak.com/channels/";

// Initialize the display using TFT_eSPI
TFT_eSPI tft = TFT_eSPI();  

// Timing
const long thingSpeakInterval = 1800000;  // Halbstündliche Abfrage (30 Minuten)
unsigned long previousThingSpeakMillis = 0;

// ThingSpeak Daten
String outdoorTemperature = "--";
String outdoorHumidity = "--";
String outdoorPressure = "--";

void setup() {
  Serial.begin(115200);
  initializeDisplay();

  tft.fillScreen(TFT_BLACK); 
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Verbinde mit WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\Mit WiFi verbunden");
  
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(1, 1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.println("Mit WiFi verbunden");

  // Erste Datenabfrage direkt nach dem Start
  fetchAndUpdateData();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;
    fetchAndUpdateData();
  }
}

void fetchAndUpdateData() {
  if (WiFi.status() == WL_CONNECTED) {
    String outdoorRequestURL = String(serverName) + outdoorChannelID + "/feeds.json?api_key=" + outdoorApiKey + "&results=1";
    String outdoorPayload = httpGETRequest(outdoorRequestURL.c_str());
    if (outdoorPayload != "--") {
      parseThingSpeakResponse(outdoorPayload);
      updateDisplay();
    }
  } else {
    Serial.println("WiFi getrennt");
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("WiFi getrennt");
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName); 
  int httpResponseCode = http.GET();
  String payload = "--";
  
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Fehlercode: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}

void parseThingSpeakResponse(String payload) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("deserializeJson() fehlgeschlagen: ");
    Serial.println(error.f_str());
    return;
  }

  if (doc["feeds"].size() > 0) {
    JsonObject feed = doc["feeds"][0];
    outdoorTemperature = feed["field1"].isNull() ? "--" : String((int)feed["field1"].as<float>());
    outdoorHumidity = feed["field2"].isNull() ? "--" : String((int)feed["field2"].as<float>());
    outdoorPressure = feed["field3"].isNull() ? "--" : String((int)feed["field3"].as<float>());
  } else {
    Serial.println("Keine Feeds verfügbar");
  }
}

void updateDisplay() {
  tft.fillRect(40, 50, 200, 80, TFT_BLACK);
  tft.setTextSize(8);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(40, 50);
  tft.print(outdoorTemperature);
  tft.print(" `C");
  
  tft.fillRect(20, 120, 280, 40, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 140);
  tft.print("Feuchte: ");
  tft.print(outdoorHumidity);
  tft.print(" %");
  
  tft.fillRect(20, 200, 280, 40, TFT_BLACK);
  tft.setCursor(20, 200);
  tft.print("Druck: ");
  tft.print(outdoorPressure);
  tft.print(" hPa");
}

void initializeDisplay() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.invertDisplay(false);
}
