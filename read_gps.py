
#!/usr/bin/env python
import time
import serial
import sys

ser = serial.Serial(
        port='/dev/serial0',
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

i=0

while (i==0):

        line=str(ser.readline())
        #print (line)
        #for line in x:
        if line.startswith("b'$GPRMC"):
                line = line.strip()
                #print (line)
                dados = line.split(',')
                latitude = dados[3]
                graus_lat = latitude[0]+latitude[1]
                longitude = dados[5]
                graus_long = longitude[0]+longitude[1]+longitude[2]
                coordenadas = graus_lat+'º'+latitude[2:]+"'"+ dados[4]+','+graus_long+'º'+longitude[3:]+"'"+dados[6]
                print(coordenadas)

                arq = open("file.txt","a")
                arq.write(coordenadas)
                arq.close()

                ser.close()
                i=i+1
