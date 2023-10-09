// Streambeleuchtung_by_Fellpower v0.1
// Version Oktober 2023
// 
// Youtube: https://www.youtube.com/channel/UCmRBHLJ2xIPz9j5sxnHRT5Q
//
// Discord: http://discord.bauermitbart.org
//
// Mit diesem Code, könnt ihr mit einem Wemos D1 Mini (ESP8266) im Browser die Helligkeit einer selbstgebauten Streambeleuchtung einstellen - und sie auch Ein,- und Ausschalten.
// Zur besseren Lesbarkeit im Stream, kann die Schriftgröße im Webinterface angepasst werden. 
//
// Sollte es zu Flimmern beginnen, wenn ihr eine Kamera benutzt, könnt ihr im Webinterface auch die PWM Frequenz einstellen. (von 50Hz bis 1000Hz)
//
// Die eingestellte Helligkeit wird im EEPROM gespeichert - und beim nächsten Einschalten wieder auf den zuvor genutzten Wert eingestellt.
//
// Bauvideos 
// Teil 1: https://www.youtube.com/watch?v=5xR7Eyzwvi4
// Teil 2: https://www.youtube.com/watch?v=H3wi_uYfGwo

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const int pwmPin = D5;  // Pin für das PWM-Signal
const int eepromAddress = 0;  // Adresse im EEPROM für die Helligkeit
const int eepromFontSizeAddress = 1;  // Adresse im EEPROM für die Schriftgröße

const char *ssid = "WLAN Name";  // Deine WLAN-SSID
const char *password = "WLAN PASSWORT";  // Dein WLAN-Passwort

int brightness = 0;  // Startwert der Helligkeit (0-100)
int fontSize = 24;   // Startwert für die Schriftgröße
bool lightOn = false;  // Status des Lichts (an/aus)
int pwmFrequency = 500;  // Startwert für die PWM-Frequenz

ESP8266WebServer server(80);

void setup() {
  // Setze die PWM-Frequenz
  analogWriteFreq(pwmFrequency);

  pinMode(pwmPin, OUTPUT);

  // Verbinde mit dem WLAN
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // EEPROM initialisieren
  EEPROM.begin(512);

  // Helligkeit und Schriftgröße aus dem EEPROM abrufen
  brightness = EEPROM.read(eepromAddress);
  brightness = constrain(brightness, 0, 100);
  fontSize = EEPROM.read(eepromFontSizeAddress);
  fontSize = constrain(fontSize, 10, 30);  // Begrenze die Schriftgröße

  // Routen für das Webinterface festlegen
  server.on("/", HTTP_GET, handleRoot);
  server.on("/increase", HTTP_GET, handleIncrease);
  server.on("/decrease", HTTP_GET, handleDecrease);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/increasefont", HTTP_GET, handleIncreaseFont);
  server.on("/decreasefont", HTTP_GET, handleDecreaseFont);
  server.on("/setfrequency", HTTP_POST, handleSetFrequency);

  server.begin();
}

void loop() {
  // Webserver aktualisieren
  server.handleClient();

  // Helligkeit setzen basierend auf dem aktuellen Wert und dem Status des Lichts
  if (lightOn) {
    int mappedBrightness = map(brightness, 0, 100, 0, 255);
    analogWrite(pwmPin, mappedBrightness);
  } else {
    analogWrite(pwmPin, 0);  // PWM auf 0 setzen, wenn das Licht ausgeschaltet ist
  }
}

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'></head><body style='font-size:" + String(fontSize) + "px;'>";
  html += "<h1>DIY STREAMLIGHT</h1>";
  html += "<p>Helligkeit: " + String(brightness) + "%</p>";
  html += "<p>Schriftgröße: " + String(fontSize) + "px</p>";
  html += "<p>PWM-Frequenz: " + String(pwmFrequency) + " Hz</p>";
  html += "<a href='/increase'><button style='font-size:24px;'>Heller</button></a> <a href='/decrease'><button style='font-size:24px;'>Dunkler</button></a>";
  
  if (lightOn) {
    html += " <a href='/toggle'><button style='background-color:green; color:white; font-size:24px;'>Licht ausschalten</button></a>";
  } else {
    html += " <a href='/toggle'><button style='background-color:red; color:white; font-size:24px;'>Licht einschalten</button></a>";
  }
  
  html += " <a href='/increasefont'><button style='font-size:24px;'>Schriftgröße +</button></a> <a href='/decreasefont'><button style='font-size:24px;'>Schriftgröße -</button></a>";

  // Formular zum Einstellen der PWM-Frequenz
  html += "<form action='/setfrequency' method='post'>";
  html += "PWM-Frequenz einstellen (50er Schritte): <input type='number' name='frequency' value='" + String(pwmFrequency) + "' min='50' step='50'><br>";
  html += "<input type='submit' value='PWM-Frequenz setzen'>";
  html += "</form>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleIncrease() {
  if (brightness < 100) {
    brightness += 10;
    saveSettingsToEEPROM();
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void handleDecrease() {
  if (brightness > 0) {
    brightness -= 10;
    saveSettingsToEEPROM();
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void handleToggle() {
  lightOn = !lightOn;
  saveSettingsToEEPROM();
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void handleIncreaseFont() {
  if (fontSize < 30) {
    fontSize += 2;
    saveSettingsToEEPROM();
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void handleDecreaseFont() {
  if (fontSize > 10) {
    fontSize -= 2;
    saveSettingsToEEPROM();
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void handleSetFrequency() {
  if (server.hasArg("frequency")) {
    int newFrequency = server.arg("frequency").toInt();
    if (newFrequency >= 50) {
      pwmFrequency = newFrequency;
      analogWriteFreq(pwmFrequency);
    }
  }
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "OK");
}

void saveSettingsToEEPROM() {
  EEPROM.write(eepromAddress, brightness);
  EEPROM.write(eepromFontSizeAddress, fontSize);
  EEPROM.commit();
}


