CHANNEL1 = 0x20DFDB8C
CHANNEL2 = 0x20DFDB4C
CHANNEL3 = 0x20DFDBCC
CHANNEL4 = 0x20DFDB2C
CHANNEL5 = 0x20DFDBAC


TABLE_TRAIN = {
    #      Ahead       Back        Stop        Pass
    "①": [0x20DFDB8C, 0x20DFDB4C, 0x20DFDBCC, 0x20DFDB2C, 0x20DFDBAC],  # Train 1
    "②": [0x20DFDB80, 0x20DFDB40, 0x20DFDBC0, 0x20DFDB20, 0x20DFDBA0],  # Train 2
    "③": [0x20DFDB88, 0x20DFDB48, 0x20DFDBC8, 0x20DFDB28, 0x20DFDBA8],  # Train 3
    "④": [0x20DFDB84, 0x20DFDB44, 0x20DFDBC4, 0x20DFDB24, 0x20DFDBA4],  # Train 4
    "⑤": [0x20DFDBEC, 0x20DFDB1C, 0x20DFDB9C, 0x20DFDB5C, 0x20DFDBDC],  # Train 5
}

TABLE_SERVO = {
    "P0": [0x20DFDBE0, 0x20DFDB10],  # 東京
    "P1": [0x20DFDB90, 0x20DFDB50],  # 小田原
    "P2": [0x20DFDBD0, 0x20DFDB30],  # 小田原
    "P3": [0x20DFDBE8, 0x20DFDB18],  # 静岡
    "P4": [0x20DFDB98, 0x20DFDB58],  # 静岡
    "P5": [0x20DFDBD8, 0x20DFDB38],  # 名古屋
    "P6": [0x20DFDBE4, 0x20DFDB14],  # 名古屋
}

def encode_stop(train_id):
    return TABLE_TRAIN[train_id][2]

def encode_switch(point_id, state):
    return TABLE_SERVO[point_id][state]

def encode_leave(train_id, direction):
    if direction == "Nobori":
        return TABLE_TRAIN[train_id][0]
    if direction == "Kudari":
        return TABLE_TRAIN[train_id][1]

def decode_passed(value):
    for train_id, channel_list in TABLE_TRAIN.items():
        if channel_list[3] == value:
            return train_id
    return None
