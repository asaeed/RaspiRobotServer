'''
robotServer 
created: 1/20/2013

@author: ahmad saeed
www.niltoid.com

'''

import SerialCom

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