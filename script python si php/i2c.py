#Chivu Constantin Razvan
#2016
import smbus
import MySQLdb
import time

bus = smbus.SMBus(1) #for RPI B+

#address set up in arduino(for slave)
address = 0x04

#i2c constants
TEMP_IDX = 0
HUMID_IDX = 1
GAS_IDX = 2
MOTION_IDX = 3
PRESSURE_IDX = 4
PIR_MOTION_DETECTED = 5
PIR_MOTION_STOPPED = 6
GAS_DANGER = 9
GAS_NO_DANGER = 10
    

db = MySQLdb.connect("localhost","user","psswd","wordpress")
cursor = db.cursor()

def writeNumber(value):
	bus.write_byte(address,value)
	return -1

def readNumber():
	number = bus.read_byte(address)
	return number

refresh_rate = 0.1 #seconds
motion_flag = 0;

while True:
    time.sleep(refresh_rate)    
    now = time.strftime('%Y-%m-%d %H:%M:%S')

    sid = readNumber() #get sensor type first
    print "Got info from senzor ", sid
    try:
        #temp sensor
        if sid == TEMP_IDX:
            for i in range(5):
                if i==0:
                    strval = str(unichr(readNumber()))
                else:
                    strval += str(unichr(readNumber())) 
            if float(strval) > 0:
                cursor.execute("""INSERT INTO temperature VALUES(%s,%s)""",(float(strval),now))
                cursor.execute("""UPDATE latest set Value=%s, Date=%s where Name='temp'""",(float(strval),now))
            print "Got temp ",strval
        #pressure sensor        
        if sid == PRESSURE_IDX:
            for i in range(5):
                if i==0:               
                    strval = str(unichr(readNumber()))
                else:
                    strval += str(unichr(readNumber()))
            if float(strval) > 0: 
                cursor.execute("""INSERT INTO pressure VALUES(%s,%s)""",(float(strval),now))
                cursor.execute("""UPDATE latest set Value=%s, Date=%s where Name='pressure'""",(float(strval),now))
            print "Got pressure ",strval
        #motion sensor + door/window sensor
        if sid == MOTION_IDX:
            motion = readNumber()
            if(motion == PIR_MOTION_DETECTED):
                motion_str = "motion detected"
                motion_flag = 1
            elif(motion == PIR_MOTION_STOPPED):
                motion_str = "motion stopped"
                motion_flag = 1
            else:
                motion_flag = 0;
            if motion_flag == 1:
                cursor.execute("""INSERT INTO motion VALUES(%s,%s,%s)""",(sid,now,motion_str))
                cursor.execute("""UPDATE latest set Extras=%s, Date=%s where Name='motion'""",(motion_str,now))
                print "Got motion ",motion_str
        #humidity sensor
        if sid == HUMID_IDX:
            for i in range(5):
                if i==0:
                    strval = str(unichr(readNumber()))
                else:
                    strval += str(unichr(readNumber())) 
            if float(strval) > 0:
                cursor.execute("""INSERT INTO humidity VALUES(%s,%s)""",(float(strval),now))
                cursor.execute("""UPDATE latest set Value=%s, Date=%s where Name='humidity'""",(float(strval),now))
            print "Got umid ",strval
        #gas sensor
        if sid == GAS_IDX :
            gas_danger = readNumber()
            if(gas_danger == GAS_NO_DANGER):
                gas_str = "No threat"
            if(gas_danger == GAS_DANGER):
                gas_str = "Danger"
            cursor.execute("""INSERT INTO gas VALUES(%s,%s)""",(now,gas_str))
            cursor.execute("""UPDATE latest set Extras=%s, Date=%s where Name='gas'""",(gas_str,now))
            print "Got gas value ",gas_str
        db.commit()
    except:
        db.rollback()

db.close()


