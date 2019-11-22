CHANNEL1 = 0x8C
CHANNEL2 = 0x4C
CHANNEL3 = 0xCC
CHANNEL4 = 0x2C
CHANNEL5 = 0xAC


TABLE_TRAIN = {
    #      Ahead Back  Stop  Pass  Battery
    "①": [0x8C, 0x4C, 0xCC, 0x2C, 0xAC],  # Train 1
    "②": [0x80, 0x40, 0xC0, 0x20, 0xA0],  # Train 2
    "③": [0x88, 0x48, 0xC8, 0x28, 0xA8],  # Train 3
    "④": [0x84, 0x44, 0xC4, 0x24, 0xA4],  # Train 4
    "⑤": [0xEC, 0x1C, 0x9C, 0x5C, 0xDC],  # Train 5
}

AHEAD = 0
BACK = 1
STOP = 2
PASS = 3
BATTERY = 4

TABLE_SERVO = {
    #     Straight     Siding
    "P0": [0xE0, 0x10],  # 東京
    "P1": [0x90, 0x50],  # 小田原
    "P2": [0xD0, 0x30],  # 小田原
    "P3": [0xE8, 0x18],  # 静岡
    "P4": [0x98, 0x58],  # 静岡
    "P5": [0xD8, 0x38],  # 名古屋
    "P6": [0xE4, 0x14],  # 名古屋
}

STRAIGHT = 0
SIDING = 1

def encode_stop(train_id):
    return TABLE_TRAIN[train_id][STOP]

def encode_switch(point_id, state):
    return TABLE_SERVO[point_id][state]

def encode_leave(train_id, direction):
    if direction == "Nobori":
        return TABLE_TRAIN[train_id][AHEAD]
    if direction == "Kudari":
        return TABLE_TRAIN[train_id][BACK]

def decode_passed(value):
    for train_id, channel_list in TABLE_TRAIN.items():
        if channel_list[PASS] == value:
            return train_id
    return None

# 以下テスト用

def encode_pass(train_id):
    return TABLE_TRAIN[train_id][PASS]

def decode(value):
    TABLE_TRAIN_COLUMNS = ["Ahead", "Back", "Stop", "Pass", "BatteryShortage"]
    for train_id, channel_list in TABLE_TRAIN.items():
        if value in channel_list:
            return (train_id, TABLE_TRAIN_COLUMNS[channel_list.index(value)])
    TABLE_SERVO_COLUMNS = ["Straight", "Siding"]
    for node_id, channel_list in TABLE_SERVO.items():
        if value in channel_list:
            return (node_id, TABLE_SERVO_COLUMNS[channel_list.index(value)])
    return None
