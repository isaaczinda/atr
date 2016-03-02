import pygame
import sys
import time
import serial
import math
import atexit

# setup the serial connection
Connection = serial.Serial(
    port='COM19',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        writeTimeout=0)

# wait for the serial connection to be setup
time.sleep(2)

print "serial connection setup"


# setup the joystick
pygame.init()
pygame.joystick.init()

print pygame.joystick.get_count(), "joystick(s) detected"

Controller = pygame.joystick.Joystick(0)
Controller.init()

print "joystick initialized with code", Controller.get_init()

NumberOfAxis = Controller.get_numaxes()
NumberOfButtons = Controller.get_numbuttons()

print NumberOfAxis, "axes,", NumberOfButtons, "buttons discovered"

# lets the event loop exit once the window has closed
Running = True

# initializes the axes
Axes = [ 0.0 ] * NumberOfAxis
# initializes the button array
Buttons = [ False ] * NumberOfButtons

def Shutdown():
	global Connection

	print "shutting down connection"
	Running = False
	Connection.close()

# run the shutdown function as the program exits
atexit.register(Shutdown)

def UpdateRobot():
    global Axes, Buttons, Connection

    if Axes[1] < -.1:
        Connection.write('top-beam-forward\n')
        print "top-beam-forward"
    elif Axes[1] > .1:
        Connection.write('top-beam-backward\n')
        print "top-beam-backward"
    else:
        Connection.write('top-beam-stop\n')

    if Axes[3] < -.1:
        Connection.write('bottom-servos-down\n')
        print "bottom-servos-down"
    elif Axes[3] > .1:
        Connection.write('bottom-servos-up\n')
        print "bottom-servos-up"

    if Buttons[4] == True:
        Connection.write('top-servo-down\n')
        print "top-servo-down"
    elif Buttons[5] == True:
        Connection.write('top-servo-up\n')
        print "top-servo-up"

    if Buttons[0] == True:
        Connection.write("allign-up\n")
    elif Buttons[1] == True:
        Connection.write("allign-down\n")

    if Buttons[2] == True:
        Connection.write("channel-valve-open\n")
        print "valve-open"
    elif Buttons[3] == True:
        Connection.write("channel-valve-close\n")
        print "valve-close"


# time that the previous robot update happened
PreviousUpdate = 0

while Running:
    # print Axes
	# gets the events and store them in arrays
    event = pygame.event.poll()

    if event.type == pygame.JOYAXISMOTION:
        e = event.dict
        Axes[e['axis']] = e['value']
    elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN ]:
        e = event.dict
        Buttons[e['button']] ^= True

    #update the robot every 100 milliseconds
    if abs(time.time() - PreviousUpdate) > .1:
        # set the previous time to the current time
        PreviousUpdate = time.time()
        # send serial commands to the robot
        UpdateRobot()

        #print Buttons

        print "sent data"