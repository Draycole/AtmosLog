#include <DHT.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define FLAME_SENSOR_DO A0
#define FLAME_SENSOR_AO A3
#define BUZZER_PIN A2
#define RGB_RED A5
#define RGB_GREEN A4
#define RGB_BLUE A1

// TFT Screen
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

// Bluetooth
SoftwareSerial BTSerial(3, 5);

// TFT & DHT Setup
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHTPIN, DHTTYPE);

// Thresholds
float TEMP_THRESHOLD = 35.0;
float HUMIDITY_THRESHOLD = 78.0;
int FLAME_THRESHOLD = 80;  // Digital threshold, same for analog
bool flameDetected = false;
int flameSensorValue = 0;

// Fire Probability calulation
float calculateFireProbability(int flameValue) {
  if (flameValue >= 300) return 0.0;
  if (flameValue <= 80) return 100.0;
  float probability = 100.0 * (300.0 - flameValue) / 220.0;
  if (probability < 0) return 0;
  if (probability > 100) return 100;
  return probability;
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  initializeComponents();
  displayWelcome();
  Serial.println("=== AtmosLog V2.0 ===");
}

void initializeComponents() {
  dht.begin();
  pinMode(FLAME_SENSOR_DO, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  setRGBColor(0, 255, 0);
}

void displayWelcome() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(15, 15);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2);
  tft.print("AtmosLog");
  tft.setCursor(25, 35);
  tft.setTextSize(1);
  tft.print("V2.0");
  tft.setCursor(5, 55);
  tft.setTextColor(ST7735_WHITE);
  tft.print("By Henry Munachimso");
  tft.setCursor(20, 75);
  tft.setTextColor(ST7735_GREEN);
  tft.print("System Ready");
  delay(3000);
}

void setRGBColor(int red, int green, int blue) {
  analogWrite(RGB_RED, red);
  analogWrite(RGB_GREEN, green);
  analogWrite(RGB_BLUE, blue);
}

void buzzerAlert(int alertLevel) {
  switch(alertLevel) {
    case 0: noTone(BUZZER_PIN); break;
    case 1: tone(BUZZER_PIN, 1000, 300); break;
    case 2: 
      tone(BUZZER_PIN, 1500, 500);
      delay(600);
      break;
  }
}

void checkFlameSensor() {
  flameSensorValue = analogRead(FLAME_SENSOR_AO);
  bool currentFlame = (flameSensorValue < FLAME_THRESHOLD); // fire when value drops
  
  if (currentFlame && !flameDetected) {
    // FIRE DETECTED
    flameDetected = true;
    Serial.println("🚨 FIRE DETECTED! Triggering emergency...");
    triggerFireEmergency();
  } else if (!currentFlame && flameDetected) {
    // FIRE CLEARED
    flameDetected = false;
    Serial.println("Fire cleared - returning to normal");
  }
}

void triggerFireEmergency() {
  Serial.println("=== FIRE EMERGENCY ACTIVATED ===");
  // Continuous emergency until fire is gone
  /*for (int i = 0; i < 10; i++) { // Longer emergency sequence
    setRGBColor(255, 0, 0); // Solid red
    buzzerAlert(2); // Fast emergency beep
    delay(300);
  }
  commented this out. it worked initially but i realised that it blocks the fire risk from updating on the screen if i leave it here.
  */
}

void updateSystemStatus(float temp, float humidity) {
  if (flameDetected) {
    // fire takes priority over everything
    setRGBColor(255, 0, 0); // Solid red for fire
    buzzerAlert(2); // Continuous emergency beep
    return;
  }
  
  // Normal threshold checks (only if no fire)
  bool tempExceeded = temp > TEMP_THRESHOLD;
  bool humidityExceeded = humidity > HUMIDITY_THRESHOLD;
  
  if (tempExceeded && humidityExceeded) {
    setRGBColor(255, 165, 0);
    buzzerAlert(1);
  } else if (tempExceeded) {
    setRGBColor(255, 0, 0);
    buzzerAlert(1);
  } else if (humidityExceeded) {
    setRGBColor(220, 165, 0);
    buzzerAlert(1);
  } else {
    setRGBColor(0, 255, 0);
    buzzerAlert(0);
  }
}

void updateDisplay(float temp, float humidity, bool flame, int flameRaw) {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(10, 5);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(1);
  tft.print("AtmosLog V2.0");
  
  // Temperature
  tft.setCursor(35, 25);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print("Temperature:");
  tft.setCursor(40, 40);
  tft.setTextSize(2);
  tft.setTextColor(temp > TEMP_THRESHOLD ? ST7735_RED : ST7735_WHITE);
  tft.print(temp, 1);
  tft.print("C");
  
  // Humidity
  tft.setCursor(40, 65);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print("Humidity:");
  tft.setCursor(40, 80);
  tft.setTextSize(2);
  tft.setTextColor(humidity > HUMIDITY_THRESHOLD ? ST7735_BLUE : ST7735_WHITE);
  tft.print(humidity, 1);
  tft.print("%");
  
  // Flame Status
  tft.setCursor(35, 105);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print("Flame Status:");
  tft.setCursor(50, 120);
  if (flame) {
    tft.setTextColor(ST7735_RED);
    tft.print("FIRE!");    
  } else {
    tft.setTextColor(ST7735_GREEN);
    tft.print("Clear");
  }
  
  // Fire Risk
  float fireProb = calculateFireProbability(flameRaw);
  tft.setCursor(20, 135);
  tft.setTextColor(ST7735_WHITE);
  tft.print("Fire Risk: ");
  if (fireProb > 70) tft.setTextColor(ST7735_RED);
  else if (fireProb > 30) tft.setTextColor(ST7735_YELLOW);
  else tft.setTextColor(ST7735_GREEN);
  tft.print(fireProb, 1);
  tft.print("%");
}

void broadcastData(float temp, float humidity, bool flame, int flameRaw) {
  float fireProb = calculateFireProbability(flameRaw);
  String data = "Temp:" + String(temp,1) + "C,Hum:" + String(humidity,1) + 
                "%,Flame:" + (flame ? "DETECTED" : "Clear") + 
                ",Raw:" + String(flameRaw) +
                ",Risk:" + String(fireProb, 1) + "%";
  Serial.println(data);
  BTSerial.println(data);
}

void loop() {
  checkFlameSensor(); // Check fire FIRST (highest priority)
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT read error!");
    delay(1000);
    return;
  }
  
  updateSystemStatus(temperature, humidity);
  updateDisplay(temperature, humidity, flameDetected, flameSensorValue);
  broadcastData(temperature, humidity, flameDetected, flameSensorValue);
  
  delay(1000);
}