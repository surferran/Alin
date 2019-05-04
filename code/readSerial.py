"""
Created on Fri May  3 00:34:42 2019

@author: RAN
        content is copied from ..
        help is in https://pyserial.readthedocs.io/en/latest/shortintro.html 
        
        get serial lib by: conda install pyserial
        from anaconda shell
"""

import serial


# this port address is for the serial tx/rx pins on the GPIO header
SERIAL_PORT = 'COM5'
# be sure to set this to the same rate used on the board
SERIAL_RATE = 115200


def main():
    ser = serial.Serial(SERIAL_PORT, SERIAL_RATE)
    readings=[]
#    while True:
    for x in range(20):
        # using ser.readline() assumes each line contains a single reading
        # sent using Serial.println() on the Arduino
        reading = ser.readline().decode('utf-8')
        # reading is a string...do whatever you want from here
        print(reading)
        readings.append(reading)
    ser.close()
    print("ser.is_open ",ser.is_open)

if __name__ == "__main__":
#    serial.tools.list_ports()
    main()