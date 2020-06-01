import bluepy.btle

 dev = Peripheral("80:7D:3A:DB:D5:A2")
 data = dev.getCharacteristics().read()

 print(data)
