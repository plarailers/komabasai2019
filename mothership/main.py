import serial
import serial.tools.list_ports
import time
from queue import Queue
from pick import pick
import math
import channel
import tokaido

arduino = None
recv_queue = Queue()
send_queue = Queue()

operator_mark = None
operator_time = None

# 27.55 秒
TIME_RATIO = 90 * 60 / 30
INTERVAL = 0.1
SERIAL_SIZE = 1

clock = 0

def setup():
    global arduino
    global operator_mark
    global operator_time
    port_list = serial.tools.list_ports.comports()
    if len(port_list) == 0:
        print("ポートがありません。")
        exit()
    port, index = pick(port_list, "ポートを選択:")
    arduino = serial.Serial(port.device, baudrate=9600)
    print("通信開始")
    operator_mark = tokaido.setup()
    operator_time = tokaido.tick()
    for signal in next(operator_mark):
        send_queue.put(signal)


def loop():
    # 受信バッファにデータがあるなら
    while arduino.in_waiting >= SERIAL_SIZE:
        # データを[?]バイト読み込み、整数に変換。受信キューに入れる。
        data_bytes = arduino.read(SERIAL_SIZE)
        data_int = int.from_bytes(data_bytes, byteorder="big")
        recv_queue.put(data_int)
        print("受信　　:", "0x{:X} {}".format(data_int, channel.decode(data_int)))


    while not recv_queue.empty():
        recv = recv_queue.get()
        signal_list = operator_mark.send(recv)
        for singal in signal_list:
            send_queue.put(singal)

    border = math.floor(clock * TIME_RATIO / (5 * 60)) * (5 * 60)
    if (clock - INTERVAL) * TIME_RATIO < border:
        minutes, signal_list = next(operator_time)
        print("{}:{:02}".format(minutes // 60, minutes % 60))
        for singal in signal_list:
            send_queue.put(singal)


    # 送信キューにデータがあるなら
    while not send_queue.empty():
        time.sleep(1)  # 連続で信号を送ると動かない？
        # データを取り出し、バイト列に変換。送信する。
        data_int = send_queue.get()
        data_bytes = data_int.to_bytes(SERIAL_SIZE, byteorder="big")
        arduino.write(data_bytes)
        print("　　送信:", "0x{:X} {}".format(data_int, channel.decode(data_int)))


if __name__ == "__main__":
    setup()
    while True:
        loop()
        time.sleep(INTERVAL)
        clock += INTERVAL
