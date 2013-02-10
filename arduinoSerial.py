import serial
import time
import multiprocessing

class Arduino(multiprocessing.Process):
    def __init__(self, taskQ, resultQ):
        multiprocessing.Process.__init__(self)
        self.taskQ = taskQ
        self.resultQ = resultQ
        self.usbPort = '/dev/ttyACM0'
        self.sp = serial.Serial(self.usbPort, timeout=1)

    def closeArduino(self):
        self.sp.close()

    def sendData(self, data):
        print "sendData start..."
        self.sp.write(data)
        time.sleep(3)
        print "sendData done: " + data

    def run(self):
        while True:
            #print "START RUN"
            if not self.taskQ.empty():
                task = self.taskQ.get()
                print "arduino received from tornado: " + task
                result = task
                self.resultQ.put(result)
            #time.sleep(2)
            #print "END RUN"

            #here also look at incoming serial data..
            #