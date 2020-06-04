from bluepy.btle import Scanner, Peripheral, UUID, Characteristic
import csv
import datetime
import time
import pandas as pd
from influxdb import InfluxDBClient
import time

client = InfluxDBClient(host='localhost',port=8086)
client.switch_database('project_belleudy')
client.query('DROP SERIES FROM /.*/')
nbr_rows = 1
data = pd.read_csv("data.csv")
while(nbr_rows<2000):
    for row_index, row in data.iterrows():
        tags = row[0]
        fieldValue = row[1]
        json_body = [
                {
                    "measurement": "Person_counter_dataset",
                    "tags": {
                        "Reference": tags
                    },
                    "fields": {
                        "value": fieldValue
                    }
                }
            ]
        print(json_body)
        print("////")
        client.write_points(json_body)
        nbr_rows = nbr_rows + 1
        time.sleep(5)
