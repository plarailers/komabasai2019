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
        self.edges = {}
        self.is_terminal = False
        Node.all.append(self)

    def connect(self, point, target, index=None):
        edge = Edge(self, target, index)
        self.edges[point] = edge

class Edge:
    all = []
    def __init__(self, source, target, index=None):
        self.source = source
        self.target = target
        self.index = index
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
    print(" / ".join("{} {}".format(node.name, node.point) for node in Node.all))

# 列車
T1 = Train("①")
T2 = Train("②")
T3 = Train("③")
T4 = Train("④")

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
TOKYO.connect(0, P0, index = 0)
P0.connect(1, P1)
P1.connect(0, P3, index = 1)
P1.connect(1, P3, index = 1)
P3.connect(0, P5, index = 2)
P3.connect(1, P5, index = 2)
P5.connect(0, P6)
P5.connect(1, NAGOYA2, index = 3)
P6.connect(0, NAGOYA1, index = 3)

# 上り
NAGOYA1.connect(0, P6, index = 3)
NAGOYA2.connect(0, P4, index = 3)
P6.connect(1, P4)
P4.connect(0, P2, index = 2)
P4.connect(1, P2, index = 2)
P2.connect(0, P0, index = 1)
P2.connect(1, P0, index = 1)
P0.connect(0, TOKYO, index = 0)

def setup():
    for train in Train.all:
        train.ressya = find_ressya(train.ressyabangou, diagram.normalize_time("600"))

    T1.place = TOKYO.edges[0]
    T2.place = P1.edges[1]
    T3.place = NAGOYA2.edges[0]
    T4.place = NAGOYA1.edges[0]

    P0.point = 1
    P1.point = 0
    P2.point = 1
    P3.point = 1
    P4.point = 0
    P5.point = 0
    P6.point = 0

    log()

    # 初回信号：ポイントセット、発進
    recv = yield [
        *[channel.encode_switch(node.name, node.point) for node in [P0, P1, P2, P3, P4, P5, P6]],
        *[channel.encode_leave(train.ressya["Ressyabangou"], train.ressya["Houkou"]) for train in [T2, T3]],
    ]

    while True:
        signal_list = []
        # 信号から車両を特定
        train_id = channel.decode_passed(recv)
        train = [train for train in Train.all if train.ressya["Ressyabangou"] == train_id][0]
        # 車両の位置をポイントを考慮して更新
        passed = train.place.target
        place = passed.edges[passed.point]
        train.place = place
        target = place.target
        # 通ったポイントを更新
        next_point = get_next_point(train, passed)
        if next_point:
            passed.point = next_point
            signal_list.append(channel.encode_switch(passed.name, passed.point))
        # 終点に向かうなら
        if target.is_terminal:
            # 次の車種に切り替え
            eki_jikoku = diagram.read_eki_jikoku(train.ressya["EkiJikoku"])
            arrive, _ = eki_jikoku[-1]
            next_ressya = find_ressya(train_id, arrive)
            train.ressya = next_ressya
            # 停車、ポイント切り替え
            signal_list.append(channel.encode_stop(train_id))
        # 駅なら
        elif place.index:
            eki_jikoku = diagram.read_eki_jikoku(train.ressya["EkiJikoku"])
            arrive, leave = eki_jikoku[place.index]
            # 次の駅に止まるなら
            if arrive and leave:
                # 停車
                signal_list.append(channel.encode_stop(train_id))

        if len(set(train.place for train in Train.all)) != len(Train.all):
            print("重複")

        log()
        recv = yield signal_list


def tick():
    time = diagram.normalize_time("600")
    leave_dict = diagram.get_leave_dict(dia)
    while True:
        leave_list = leave_dict[time] if time in leave_dict else []
        signal_list = []
        for train_id in leave_list:
            ressya = find_ressya(train_id, time)
            signal_list.append(channel.encode_leave(train_id, ressya["Houkou"]))
        yield time, signal_list
        time += 5
        if time == 24 * 60:
            time = 0


def find_ressya(ressyabangou, time):
    ressya_list = []
    for ressya in dia["Kudari"][0]["Ressya"] + dia["Nobori"][0]["Ressya"]:
        if ressya["Ressyabangou"] == ressyabangou:
            ressya_list.append(ressya)
    # その時刻に走っている列車があれば返す
    for ressya in ressya_list:
        eki_jikoku = diagram.read_eki_jikoku(ressya["EkiJikoku"])
        _, leave = eki_jikoku[0]
        arrive, _ = eki_jikoku[-1]
        if leave <= time < arrive:
            return ressya
        if arrive < leave and (time < arrive or leave <= time):
            return ressya
    # なければ直近に発車するときの列車を返す
    leave_list = []
    for ressya in ressya_list:
        eki_jikoku = diagram.read_eki_jikoku(ressya["EkiJikoku"])
        _, leave = eki_jikoku[0]
        leave_list.append((duration(time, leave), ressya))
    return min(leave_list)[1]


def duration(start, end):
    return (end - start) % (24 * 60)


# ある列車があるポイントを通ったとき、そのポイントをどうするべきか
def get_next_point(train, node):
    curr_eki_jikoku = diagram.read_eki_jikoku(train.ressya["EkiJikoku"])
    if node == P0:
        if train.ressya["Houkou"] == "Kudari":
            return 0
        if train.ressya["Houkou"] == "Nobori":
            return 1
    if node in [P1, P3, P4, P2]:
        acc = []
        index = {P1: 1, P3: 2, P4: 1, P2: 2}[node]
        base = None
        for ressya in dia[train.ressya["Houkou"]][0]["Ressya"]:
            eki_jikoku = diagram.read_eki_jikoku(ressya["EkiJikoku"])
            _, time = eki_jikoku[0]
            for i in range(1, index):
                arrive, leave = eki_jikoku[i]
                if arrive and leave:
                    time += duration(arrive, leave)
            if ressya == train.ressya:
                base = time
            else:
                acc.append((time, eki_jikoku))
        _, next_eki_jikoku = min((duration(base, time), eki_jikoku) for time, eki_jikoku in acc)
        return 0 if next_eki_jikoku[index] == (None, None) else 1
    if node == P5:
        if train.ressya["Ressyamei"] == "のぞみ":
            return 1
        else:
            return 0
    if node == P6:
        if train.ressya["Houkou"] == "Kudari":
            return 1
        if train.ressya["Houkou"] == "Nobori":
            return 0


# テスト用（直接 python tokaido.py で呼び出したときのみ実行）
if __name__ == "__main__":
    g = setup()
    t = tick()
    print(list(map(channel.decode, next(g))))
    print(list(map(channel.decode, next(t)[1])))  # 6:00
    print(list(map(channel.decode, g.send(channel.encode_pass("①")))))
    print(list(map(channel.decode, g.send(channel.encode_pass("③")))))
    print(list(map(channel.decode, next(t)[1])))  # 6:05
    for i in range(20):
        print(list(map(channel.decode, g.send(channel.encode_pass("①")))))
        # print(g.send(channel.TABLE_TRAIN["①"][channel.PASS]))
