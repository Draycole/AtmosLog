AtmosLog is a temperature and humidity monitoring system I developed using an Arduino UNO as the primary microprocessor. It measured the temperature and humoidity of the environment and logs the data onto an SD card, displays it on an LCD screen, and sends updates via Bluetooth. It also sends the data to a python script running that allows a graph of the temperature and humidity to be plotted in real-time.

## Features
- Measures temperature and humidity every 2 seconds.
- Displays real-time data on an LCD screen.
- Logs data onto an SD card for offline use.
- Sends updates to a moile Bluetooth-enabled device.
- Live graph visualization using Python.

## Hardware Used
- Arduino Uno
- DHT11 Sensor
- ST7735 1.8" TFT LCD Display
- HC05 Bluetooth Module
- SD Card Module
- SDHC 16GB Micro SD Card
- Samsung Micro SD Card Adapter

## Component Pinout
LCD Screen
 - Serial Clock - Pin 13
 - Serial Data Line (SDA) - Pin 11
 - RS/DC - Pin 8
 - Reset - Pin 9
 - Chip Select - Pin 10

SD Card Module
 - Master Out Slave In (MOSI) - Pin 11
 - Serial Clock - Pin 13
 - Master In Slave Out (MISO) - Pin 12
 - Chip Select - Pin 6

HC05 Bluetooth Module
 - RXD - Pin 3
 - TXD - Pin 5

DHT 11 Temp & Humidity Sensor
 - Data - Pin 2
  

## Circuit Schematic Diagram
![ATMOSLOG Circuit Schematic](https://github.com/user-attachments/assets/ba732f60-9805-419e-ac2b-183c2d2098c8)

## Project Flowchart Diagram
![Flowchart (1)](https://github.com/user-attachments/assets/af508ebf-18af-49c7-b62a-dce14c438639)

## Demo
- [Project Demo Video](https://youtu.be/ADbAQRFhFjg?list=TLGG9dOkwP3eu8cwMjAxMjAyNQ)

## Author
- [Henry Munachimso](https://bit.ly/henrymunachimso)

