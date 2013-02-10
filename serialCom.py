
import sys
import serial
import time
import select

class SerialCom():
    def __init__(self):
        self.usbPort = '/dev/ttyACM0'
        self.sp = serial.Serial(self.usbPort, timeout=1)

    def close(self):
        self.sp.close()

    def sendData(self, data):
        self.sp.write(data)
        print "in arduino sendData: " + data

    def receiveData(self):
        pass

def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

if __name__ == "__main__":
    sc = SerialCom()
    sc.sp.flushInput()
    print("starting serial comm loop...")
    while True:

        # is there data coming from the app?
        if isData():
            appLine = sys.stdin.readline()
            print(appLine.replace("\n", ""))

        # is there data coming from the serial port?
        if (sc.sp.inWaiting() > 0):
            serialLine = sc.sp.readline()
            print(serialLine.replace("\n", "").replace("\r", ""))
