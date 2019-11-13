"""
Arduinoとの通信を行う。
"""

import serial
import serial.tools.list_ports
import time

def communicate(recv_queue, send_queue):
    # ポート選択
    port_list = serial.tools.list_ports.comports()
    if len(port_list) == 0:
        print("ポートがありません。")
        return
    print("ポートを選択:", port_list)
    while True:
        port = input()
        if port in port_list:
            break
        print("無効なポート名です。")

    # 通信開始
    with serial.Serial(port, baudrate=9600) as ser:
        while True:
            # 受信バッファにデータがあるなら
            while ser.in_waiting >= 4:
                # データを4バイト読み込み、整数に変換。受信キューに入れる。
                data_bytes = ser.read(4)
                data_int = int.from_bytes(data_bytes, byteorder="big")
                recv_queue.put(data_int)
            # 送信キューにデータがあるなら
            while not send_queue.empty():
                # データを取り出し、バイト列に変換。送信する。
                data_int = send_queue.get()
                data_bytes = data_int.to_bytes(4, byteorder="big")
                ser.write(data_bytes)
            # 1秒待つ
            time.sleep(1)
