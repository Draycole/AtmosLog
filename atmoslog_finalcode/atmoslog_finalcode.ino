#include <DHT.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2      //DHT data pin
#define DHTTYPE DHT11
#define TFT_CS 10     // Chip Select for LCD Screen
#define TFT_RST 9     
#define TFT_DC 8      
#define SD_CS 6       // Chip Select for SDCard Module

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHTPIN, DHTTYPE); 
SoftwareSerial BTSerial(5,3);   //HC04 RXD pin, HC04 TXD Pin
File dataFile;

void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600); 
  dht.begin();        
  BTSerial.println("DHT11 sensor initialized.");

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    BTSerial.println("SD card initialization failed!");
    Serial.println("SD card initialization failed!");
    while (true);
  }
  BTSerial.println("SD card initialized.");

  
  // Open file to append header
  dataFile = SD.open("finedata.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time (s), Temperature (C), Humidity (%)");
    dataFile.close();
  } else {
    BTSerial.println("Failed to open file.");
    Serial.println("Failed to open file.");
  }

  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);

}

void loop() {
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    BTSerial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(", Humidity: ");
    Serial.println(humidity);

    BTSerial.print("Temp: ");
    BTSerial.print(temperature);
    BTSerial.print(", Humidity: ");
    BTSerial.println(humidity);
  } 

  // Display readings on TFT
  tft.fillRect(0, 0, 128, 160, ST7735_BLACK); // Clear screen
  tft.setCursor(10, 40);
  tft.print("Temp: ");
  tft.setCursor(10, 60);
  tft.print(temperature);
  tft.println(" C");

  tft.setCursor(10, 100);
  tft.print("Humidity: ");
  tft.setCursor(10, 120);
  tft.print(humidity);
  tft.println(" %");

  // Log data to SD card
  dataFile = SD.open("finedata.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(millis() / 1000);
    dataFile.print(", ");
    dataFile.print(temperature);
    dataFile.print(", ");
    dataFile.println(humidity);
    dataFile.close();
  } else {
    Serial.println("Failed to open file for writing.");
  }


  delay(2000);
}
