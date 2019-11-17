import serial
import serial.tools.list_ports
import time
from queue import Queue
from pick import pick

arduino = None
recv_queue = Queue()
send_queue = Queue()

def setup():
    global arduino
    port_list = serial.tools.list_ports.comports()
    if len(port_list) == 0:
        print("ポートがありません。")
        exit()
    port, index = pick(port_list, "ポートを選択:")
    arduino = serial.Serial(port.device, baudrate=9600)
    print("通信開始")


def loop():
    # 受信バッファにデータがあるなら
    while arduino.in_waiting >= 4:
        # データを4バイト読み込み、整数に変換。受信キューに入れる。
        data_bytes = arduino.read(4)
        data_int = int.from_bytes(data_bytes, byteorder="big")
        recv_queue.put(data_int)
        print("受信:", "0x{:08X}".format(data_int))

    send_queue.put(0x20DFDB8C)

    # 送信キューにデータがあるなら
    while not send_queue.empty():
        # データを取り出し、バイト列に変換。送信する。
        data_int = send_queue.get()
        data_bytes = data_int.to_bytes(4, byteorder="big")
        arduino.write(data_bytes)
        print("送信:", "0x{:08X}".format(data_int))


if __name__ == "__main__":
    setup()
    while True:
        loop()
        time.sleep(1)
