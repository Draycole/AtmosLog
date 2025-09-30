import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import serial
import serial.tools.list_ports
from time import sleep
import numpy as np

def find_arduino_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'Arduino' in port.description or 'CH340' in port.description:
            return port.device
    return 'COM3'

# Setup serial
ser = serial.Serial(find_arduino_port(), 9600, timeout=1)
sleep(2)

# Setup plotting - ONLY TEMP AND HUMIDITY
plt.ion()
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
fig.suptitle('AtmosLog V2.0 - Temperature & Humidity Monitoring')

# Data storage with proper initial values
data_points = 50
temps = [25.0] * data_points  # Start at reasonable temp
humids = [50.0] * data_points  # Start at reasonable humidity
x_data = list(range(data_points))

# Initial plots
line_temp, = ax1.plot(x_data, temps, 'r-', linewidth=2, label='Temperature (°C)')
line_humid, = ax2.plot(x_data, humids, 'b-', linewidth=2, label='Humidity (%)')

# Configure axes with proper ranges
ax1.set_title('Temperature')
ax1.set_ylabel('°C')
ax1.set_ylim(15, 40)  # Reasonable temp range
ax1.legend()
ax1.grid(True)

ax2.set_title('Humidity') 
ax2.set_ylabel('%')
ax2.set_ylim(20, 90)  # Reasonable humidity range
ax2.set_xlabel('Time (updates)')
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.show(block=False)

print("Temperature & Humidity monitoring started. Close window to stop.")

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode().strip()
            print(f"Data: {line}")
            
            if "Temp" in line and "Hum:" in line:
                try:
                    parts = line.split(",")
                    temp = float(parts[0].split(":")[1].replace("C", "").strip())
                    humid = float(parts[1].split(":")[1].replace("%", "").strip())
                    
                    # Update data arrays
                    temps.pop(0)
                    temps.append(temp)
                    humids.pop(0)
                    humids.append(humid)
                    
                    # Auto-adjust y-axis if needed (but keep reasonable bounds)
                    current_min_temp, current_max_temp = min(temps), max(temps)
                    ax1.set_ylim(max(10, current_min_temp - 2), min(50, current_max_temp + 2))
                    
                    current_min_humid, current_max_humid = min(humids), max(humids) 
                    ax2.set_ylim(max(10, current_min_humid - 5), min(100, current_max_humid + 5))
                    
                    # Update plots
                    line_temp.set_ydata(temps)
                    line_humid.set_ydata(humids)
                    
                    # Redraw
                    fig.canvas.draw()
                    fig.canvas.flush_events()
                    
                except Exception as e:
                    print(f"Parse error: {e}")
        
        plt.pause(0.01)
        
except KeyboardInterrupt:
    print("Stopping...")
finally:
    ser.close()
    plt.close('all')