with open('/dev/mydevice', 'r') as f:
    data = f.read().strip()
    print("Read from mydevice: ",data)

with open('/dev/mydevice', 'w') as f:
    f.write('Hello, mydevice!\n')
    print("Data succesfully saved to mydevice!")
