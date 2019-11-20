def read_oudia(file, encoding):
    with open(file, encoding=encoding) as f:
        stack = []
        curr = {}
        stack.append(curr)
        for line in f:
            line = line.strip()
            if line == ".":
                stack.pop()
                curr = stack[-1]
            elif line[-1] == ".":
                name = line[0:-1]
                if name not in curr:
                    curr[name] = []
                curr[name].append({})
                curr = curr[name][-1]
                stack.append(curr)
            else:
                name, *value = line.split("=")
                value = "=".join(value)
                curr[name] = value
        return stack[0]


def read_eki_jikoku(eki_jikoku):
    ret = []
    for item in eki_jikoku.split(","):
        arrive, leave = None, None
        if item == "":
            pass
        elif item[0] == "1":
            pair = item.split(";")[1]
            if "/" not in pair:
                leave = pair
            elif pair[-1] == "/":
                arrive = pair[0:-1]
            else:
                arrive, leave = pair.split("/")
        elif item[0] == "2":
            pass
        ret.append((arrive, leave))
    return ret


def get_leave_dict(dia):
    leave_dict = {}
    for ressya in dia["Kudari"][0]["Ressya"] + dia["Nobori"][0]["Ressya"]:
        for arrive, leave in read_eki_jikoku(ressya["EkiJikoku"]):
            if leave == None:
                continue
            if leave not in leave_dict:
                leave_dict[leave] = []
            leave_dict[leave].append(ressya["Ressyabangou"])
    return leave_dict
