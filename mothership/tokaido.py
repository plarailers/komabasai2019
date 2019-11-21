import diagram
import channel

oudia = diagram.read_oudia("./data/東海道新幹線", encoding="utf-8")
dia = oudia["Rosen"][0]["Dia"][2]

class Train:
    all = []
    def __init__(self, ressyabangou):
        self.ressyabangou = ressyabangou
        self.ressya = None
        self.place = None
        self.direction = None
        self.type = None
        Train.all.append(self)

class Node:
    all = []
    def __init__(self, name):
        self.name = name
        self.point = 0
        self.edges = []
        self.is_terminal = False
        Node.all.append(self)

    def connect(self, target):
        edge = Edge(self, target)
        self.edges.append(edge)
        return edge

class Edge:
    all = []
    def __init__(self, source, target):
        self.source = source
        self.target = target
        self.index = None
        Edge.all.append(self)

def log():
    for train in Train.all:
        print("列車 {}  {} {:>3} {} ( {} →  {} )".format(
            train.ressyabangou,
            train.ressya["Ressyamei"],
            train.ressya["Gousuu"],
            train.ressya["Houkou"],
            train.place.source.name,
            train.place.target.name))

# 列車
T0 = Train("①")
T1 = Train("②")
T2 = Train("③")
T3 = Train("④")

# ポイント
TOKYO = Node("東京")
P0 = Node("P0")
P1 = Node("P1")
P2 = Node("P2")
P3 = Node("P3")
P4 = Node("P4")
P5 = Node("P5")
P6 = Node("P6")
NAGOYA1 = Node("名古屋1")
NAGOYA2 = Node("名古屋2")

TOKYO.is_terminal = True
NAGOYA1.is_terminal = True
NAGOYA2.is_terminal = True

# 下り
TOKYO.connect(P0).index = 0
P0.connect(P1)
P1.connect(P3).index = 1
P1.connect(P3).index = 1
P3.connect(P5).index = 2
P3.connect(P5).index = 2
P5.connect(P6)
P5.connect(NAGOYA2).index = 3
P6.connect(NAGOYA1).index = 3

# 上り
NAGOYA1.connect(P6).index = 3
NAGOYA2.connect(P4).index = 3
P6.connect(P4)
P4.connect(P2).index = 2
P4.connect(P2).index = 2
P2.connect(P0).index = 1
P2.connect(P0).index = 1
P0.connect(TOKYO).index = 0

def setup():
    for train in Train.all:
        train.ressya = find_ressya(train.ressyabangou, 600)

    T0.place = TOKYO.edges[0]
    T1.place = P1.edges[1]
    T2.place = NAGOYA2.edges[0]
    T3.place = NAGOYA1.edges[0]

    log()

    recv = yield

    while True:
        train_id = channel.decode_passed(recv)
        train = Train
        train = [T0, T1, T2, T3][recv]
        passed = train.place.target
        place = passed.edges[passed.point]
        train.place = place
        if place.target.is_terminal:
            # 停車
            eki_jikoku = diagram.read_eki_jikoku(train.ressya["EkiJikoku"])
            arrive = eki_jikoku[-1][0]
            next_ressya = find_ressya(train.ressya["Ressyabangou"], arrive)
            train.ressya = next_ressya
            recv = yield [
                channel.encode_stop(train.ressya["Ressyabangou"]),
            ]
        elif place.index:
            eki_jikoku = diagram.read_eki_jikoku(train.ressya["EkiJikoku"])
            arrive, leave = eki_jikoku[place.index]
            if arrive:
                # 停車
                place.target.point = 1
                recv = yield [
                    channel.encode_stop(train.ressya["Ressyabangou"]),
                    channel.encode_switch(place.target.name, place.target.point),
                ]
            else:
                # 通過
                place.target.point = 0
                recv = yield [
                    channel.encode_stop(train.ressya["Ressyabangou"]),
                    channel.encode_switch(place.target.name, place.target.point),
                ]

        log()

        if len(set(train.place for train in Train.all)) != len(Train.all):
            print("重複")


def tick():
    time = 600
    leave_dict = diagram.get_leave_dict(dia)
    while True:
        leave_list = leave_dict[time] if time in leave_dict else[]
        channel_list = []
        for train_id in leave_list:
            ressya = find_ressya(train_id, time)
            channel_list.append(channel.encode_leave(train_id, ressya["Houkou"]))
        yield time, channel_list
        time += 5
        if time % 100 == 60:
            time -= 60
            time += 100
            if time == 2400:
                time = 0


def find_ressya(ressyabangou, time):
    ressya_list = []
    for ressya in dia["Kudari"][0]["Ressya"] + dia["Nobori"][0]["Ressya"]:
        if ressya["Ressyabangou"] == ressyabangou:
            ressya_list.append(ressya)
    # その時刻に走っている列車があれば返す
    for ressya in ressya_list:
        eki_jikoku = diagram.read_eki_jikoku(ressya["EkiJikoku"])
        leave = eki_jikoku[0][1]
        arrive = eki_jikoku[-1][0]
        if leave <= time < arrive:
            return ressya
        if arrive < leave and (time < arrive or leave <= time):
            return ressya
    # なければ直近に発車するときの列車を返す
    leave_list = []
    for ressya in ressya_list:
        eki_jikoku = diagram.read_eki_jikoku(ressya["EkiJikoku"])
        leave = eki_jikoku[0][1]
        leave_list.append(((leave - time) % 2400, ressya))
    return min(leave_list)[1]
