import serial
import time

ser = serial.Serial(
    port='COM19',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=0)

# wait
time.sleep(2)

print(ser) 

ser.write('top-beam-backward\n')
print ser.readline()
time.sleep(.5)
ser.write('top-beam-stop\n')
print ser.readline()



ser.close()