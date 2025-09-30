#import serial
#import matplotlib
#matplotlib.use('TkAgg')
#import matplotlib.pyplot as plt
#from time import sleep

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import serial
import serial.tools.list_ports
from time import sleep
import numpy as np


ser = serial.Serial('COM6', 9600) 
sleep(2)  

temp_data = []
humidity_data = []
plt.ion()
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))  # Two subplots, one on top of the other

while True:
    try:
        line = ser.readline().decode().strip()  # Read data from serial
        print(f"Raw data: {line}")  # Debug print to see incoming string
        
        if "Temp" in line and "Humidity" in line:
            parts = line.split(", ")  # Split string by ", "
            temp = float(parts[0].split(":")[1].strip().replace("°C", ""))  # Extract temperature
            humidity = float(parts[1].split(":")[1].strip().replace("%", ""))  # Extract humidity
            
            temp_data.append(temp)
            humidity_data.append(humidity)
            
            # Update Temperature Plot
            ax1.clear()
            ax1.plot(temp_data, label='Temperature (°C)', color='red')
            ax1.set_title('Temperature')
            ax1.set_ylabel('°C')
            ax1.legend(loc='upper left')
            ax1.grid(True)
            
            # Update Humidity Plot
            ax2.clear()
            ax2.plot(humidity_data, label='Humidity (%)', color='blue')
            ax2.set_title('Humidity')
            ax2.set_ylabel('%')
            ax2.legend(loc='upper left')
            ax2.grid(True)
            
            plt.pause(0.1)
            
    except ValueError as e:
        print(f"ValueError: {e}, Raw data: {line}")
    except KeyboardInterrupt:
        break

ser.close()
