#!/bin/bash
# My first script

echo "Performing scan of Bluetoth devices"
sudo hcitool scan
echo "Connection to m5stack"
sudo rfcomm connect hci0 80:7D:3A:DB:D5:A2 1
echo "Success!"

exit
