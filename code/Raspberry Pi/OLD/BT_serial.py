from bluepy.btle import Scanner, Peripheral, UUID, Characteristic
import csv
import datetime
import time

temp = []

year = []
month = []
day = []
hour = []
minute = []
second = []

dev = Peripheral("80:7D:3A:DB:D5:A2","random")

char = dev.getCharacteristics(0x0100, 0x0101)

if (char[0].supportsRead()):
    for i in range(0,10):
        # taking data convert it to list of str
        val = char[0].read().decode().split(' ',1)
        # convert str list to float list
        val = [float(x.strip(' "')) for x in val]

        print('{}'.format(val))

        # date
        date = datetime.datetime.now()

        year.append(date.year)
        month.append(date.month)
        day.append(date.day)
        hour.append(date.hour)
        minute.append(date.minute)
        second.append(date.second)


        #temperature
        temp.append(val[0])

        time.sleep(1)

dev.disconnect()

# writing csv
with open('data_m5_test.csv', mode='w') as valueFile:
    valueWriter = csv.writer(valueFile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    valueWriter.writerow(year)
    valueWriter.writerow(month)
    valueWriter.writerow(day)
    valueWriter.writerow(hour)
    valueWriter.writerow(minute)
    valueWriter.writerow(second)
    valueWriter.writerow(temp)
