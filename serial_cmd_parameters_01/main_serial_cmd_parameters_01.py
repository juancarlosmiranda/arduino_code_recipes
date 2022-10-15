"""
Project: Reading and parsing data from PC
Author: Juan Carlos Miranda
Date: October 2022
Description:
  A Python script used to send command data using a serial port.
  The code is based on: Demo of PC-Arduino comms using Python.
  * https://forum.arduino.cc/t/serial-input-basics/278284/2
  * https://forum.arduino.cc/t/demo-of-pc-arduino-comms-using-python/219184/4

Use:
sketch_serial_01.ino -> Load sketch_serial_01.ino into Arduino board.
main_serial_arduino_01.py -> simple send and receive using serial port.


"""
import serial
import time

global START_CMD, END_CMD


def recv_from_arduino():
    str_read = ""
    x = "."  # any value that is not an end- or startMarker
    byte_count = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    while x != START_CMD:
        x = serial_handler.read()
        print("DEBUG->", x, ord(x))
    str_read = x.decode()
    # save data until the end marker is found
    while x != END_CMD:
        x = serial_handler.read()
        print("DEBUG->", x, ord(x), x.decode())
        if ord(x) != START_CMD:
            str_read = str_read + x.decode()
            byte_count += 1

    return str_read


def main_arduino():
    cmd_str_01_ON = '<CMD_01,ON>'
    cmd_str_01_OFF = '<CMD_01,OFF>'
    cmd_str_02 = '<CMD_02>'
    cmd_str_03 = '<CMD_03>'

    print("Manage commands from Arduino ->")
    # receive data from Arduino <STARTING_ARDUINO>
    print("Receiving data from Arduino CMD ->")
    data_recvd = recv_from_arduino()
    print("Reply Received  " + data_recvd)

    # Start to send commands and receive and echo message
    print("Send Arduino CMD ->"+cmd_str_01_ON)
    serial_handler.write(cmd_str_01_ON.encode())
    # waiting cycle
    while serial_handler.inWaiting() == 0:
        pass

    data_recvd = recv_from_arduino()
    print("Reply Received  " + data_recvd)
    print("Sleeping -->>> Before to turn off")
    time.sleep(5)
    print("Send Arduino CMD ->"+cmd_str_01_OFF)
    serial_handler.write(cmd_str_01_OFF.encode())
    # waiting cycle
    while serial_handler.inWaiting() == 0:
        pass

    data_recvd = recv_from_arduino()
    print("Reply Received  " + data_recvd)


# ---------------
# MAIN
# ---------------
serial_port = 'COM3'  # "/dev/ttyS80"
baud_rate = 9600
serial_handler = serial.Serial(serial_port, baud_rate)

# Characters must be codified as byte type, or character number must be initialised
# If prefer definition as byte, use ord(START_CMD) to convert data from serial port in receive function
# START_CMD = 60
# END_CMD = 62
START_CMD = b'<'
END_CMD = b'>'

if __name__ == '__main__':
    print(f'Serial port={serial_port} baud_rate={baud_rate}')
    main_arduino()

# send and receive data
# make using parallel process, to insert into a client for network.
