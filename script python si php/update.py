#Chivu Constantin Razvan
#2016
import MySQLdb
import time

db = MySQLdb.connect("localhost","user","psswd","wordpress")
cursor = db.cursor()

refresh_rate = 1 #seconds


while True:
    time.sleep(refresh_rate) 
    
    
    try:
        file = open("/var/www/latest.txt",'w')
        cursor.execute("SELECT * FROM latest")
        result = cursor.fetchall()
        db.commit()
        for row in result:
            name = row[0]
            value = row[1]
            extras = row[2]
            date = row[3]
            #print "%s | %s | %s | %Y " % (name,value,extras,date)
            file.write(name)
            file.write("\t")
            file.write(str(value))
            file.write("\t")
            if extras != None:
                file.write(extras)
            else:
                file.write("null")
            file.write("\t")
            if date != None:
                file.write(str(date))
            else:
                file.write("null")
            file.write("\n")
            
        file.close()
    except:
        print "connection error"
        
