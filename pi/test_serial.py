import serial
import time

ser = serial.Serial("/dev/ttyAMA0", 115200)
def ser_test():
    send_msg = "@1,12.95,12,13#@2,21.8,22,23#@3,38.1,32#@4,41,42#@5,51,52#@10,661,62#"
    ser.write(send_msg.encode("utf-8"))

def read_ser():
    count = ser.inWaiting()
    if count != 0:
        recv_msg = ser.read(count)
        ser.flushInput()
        time.sleep(0.1)
        return recv_msg
    
if __name__ == "__main__":
    while True:
        #er_test()
        time.sleep(1)
        print(read_ser().decode("utf-8"))
        time.sleep(1)
        
