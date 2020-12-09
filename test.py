#!/usr/bin/python3
from time import sleep
from numpy import pi

def test():
    spin = 0
    while True:
        mode = spin%4 + 1
        motor = open("/dev/mystep0", "w")
        motor.write(str(mode))
        motor.close()
        spin += 1
        sleep(0.05)
        print("回転角度: {}".format(spin*1.8), end="\r")

def test1():
    spin = 0
    while True:
        mode = spin%4 + 5
        motor = open("/dev/mystep0", "w")
        motor.write(str(mode))
        motor.close()
        spin += 1
        sleep(0.01)
        print("回転角度: {}".format(spin*1.8), end="\r")

if "main" in __name__:
    mode = int(input("enter mode: "))
    try:
        if (mode == 0):
            test()
        elif (mode == 1):
            test1()
    except KeyboardInterrupt:
        with open("/dev/mystep0", "w") as motor:
            motor.write("0")
        print("Goodbye")
