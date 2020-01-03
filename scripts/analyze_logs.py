import json
import subprocess
from functools import reduce

subprocess.call(["rm", "../log.json"])
for i in range(100):
    subprocess.call("../generator/bin/generator >> ../log.json", shell=True)
    subprocess.call("printf '\n' >> ../log.json", shell=True)

f = open("../log.json", "r")

fields = []

currS = "{}"

for x in f:
    if x == "{\n":
        fields.append(json.loads(currS))
        currS = ""
    currS += x

del fields[0]

class Point:
    def __init__(self, data):
        self.x = data["x"]
        self.y = data["y"]

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y;

    def __hash__(self):
        return self.x * 10000 + self.y

class ParkingZone:
    def __init__(self, data):
        self.points = []
        for i in range(3):
            self.points.append(Point(data[i]))
        self.points = tuple(self.points)

    def __eq__(self, other):
        for i in range(3):
            if self.points[i] != other.points[i]:
                return False
        return True

    def __hash__(self):
        return hash(self.points)

boxes = reduce(lambda bs1, bs2: bs1 + bs2, list(map(lambda f: list(str(b) for b in f["boxes"]), fields)))
print(len(boxes))
boxes = set(boxes)
print(len(boxes))

fake_boxes1 = reduce(lambda bs1, bs2: bs1 + bs2, list(map(lambda f: list(str(b) for b in f["fakeBoxes1"]), fields)))
print(len(fake_boxes1))
fake_boxes1 = set(fake_boxes1)
print(len(fake_boxes1))

fake_boxes2 = reduce(lambda bs1, bs2: bs1 + bs2, list(map(lambda f: list(str(b) for b in f["fakeBoxes2"]), fields)))
print(len(fake_boxes2))
fake_boxes2 = set(fake_boxes2)
print(len(fake_boxes2))

fake_boxes3 = reduce(lambda bs1, bs2: bs1 + bs2, list(map(lambda f: list(str(b) for b in f["fakeBoxes3"]), fields)))
print(len(fake_boxes3))
fake_boxes3 = set(fake_boxes3)
print(len(fake_boxes3))


parking_zones = list(map(lambda f: ParkingZone(f["parkingZone"]), fields))
print(len(parking_zones))
parking_zones = set(parking_zones)
print(len(parking_zones))