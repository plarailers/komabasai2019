import threading
from queue import Queue
import arduino

if __name__ == "__main__":
    recv_queue = Queue()
    send_queue = Queue()
    threading.Thread(
        target=arduino.communicate,
        kwargs={"recv_queue": recv_queue, "send_queue": send_queue}
    ).start()
