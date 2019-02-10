import visa
import time
import datetime
import csv

rm = visa.ResourceManager()
res = rm.list_resources()
print("Found following resources: ")
print(res)
print("Opening " + res[-1])
'''
#Remove the ":" and blanks in the dae format as they are not permitted in the file name
date_time = str(datetime.datetime.now())
date_time = '_'.join(filter(None,date_time.split(' ')))
date_time = '_'.join(filter(None,date_time.split(':')))
'''

# Add zero-ing procedure for DMM; THis means extra connections for relays also need to be made
'''
meas = open('Logging_CapLeakage_4Ch'+date_time+'.txt', 'a')
meas.write("Logging Started at: "+str(datetime.datetime.now()) + '\n')
meas.flush()
'''

##inst = rm.open_resource(res[-1])
inst = rm.open_resource('GPIB0::12::INSTR')          # FLuke 8505A
channel_sel = rm.open_resource("COM4", baud_rate = 19200)   # open port for arduino relay card


#Fluke set to GPIB port 12

##inst.query("VR2SxFxT0?")  # Volts DC, 10V range, Sampling rate x, Filter setting x, Intant trigger, ? = process command and send response
##inst.query("OT0?") # Activate averaging and trigger

inst.query("VR0H0F1T0?")  # DC Volts, Range 0, S6 (64 samples, 267 ms), // Added H0 - 256 samples (1.1 s) F1 - Bypass filter, T0 - single reading
time.sleep(1)
inst.query("T0?")
inst.timeout = 3000  # 2 second timeout
measurement_toLog = [None] * 4

with open('Logging_4CH.csv', 'w') as csvfile:
        filewriter = csv.writer(csvfile, delimiter=',', lineterminator='\n')
        while True:
    
            for i in range(0,4):
                channel_sel.write(str(i+1) + "=ON;")  # Set the relay channel ON        
                time.sleep(1)
        
                measurement_toLog[i] = inst.query("?")  # Send "?"  to 8505A to trigger a reading
                if i==3:
                    filewriter.writerow(measurement_toLog)
                    csvfile.flush()
                    measurement_toLog.clear()
                    measurement_toLog = [None] * 4                 
                    # print("if")#  when the counter hits 4, add a \n at the end of the file and go to the next line   
                channel_sel.write(str(i+1) + "=OFF;")  # Set the relay channel OFF
                

                time.sleep(0.5)
                #print(measurement_toLog) #Debug purposes only

        #stuff needs to be withing the "with open" statement. Anything outside the loop will close the CSV file
    
##print(temp.read())  # print temperature reading

#probably add read on command function to temp measurement

