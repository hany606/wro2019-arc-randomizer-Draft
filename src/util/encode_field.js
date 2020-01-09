import {nextIntIn} from "./random";
import {randomSet} from "./random";

export function encodePoint(p) {
    return String.fromCharCode('A'.charCodeAt(0) + Math.round(p.x/115)) + "," + String.fromCharCode('A'.charCodeAt(0) + Math.round(p.y/115));
}

// 0 - top-left, bottom-right, 1 - top-right, bottom-left
function encodeBox(box, key = nextIntIn(0, 1)) {
    let p1 = {x: box.right, y: box.top};
    let p2 = {x: box.left, y: box.bott};
    if(key === 0) {
        p1 = {x: box.left, y: box.top};
        p2 = {x: box.right, y: box.bott};
    }
    return "(" + encodePoint(p1) + "," + encodePoint(p2) +")";
}

export default function encodeField(field) {
    let fields = [];
    let res = "";

    // parking zone top left point
    res += "(" + encodePoint(field.parkingZone[0]) + ",";

    // parking zone direction point
    let dir = {x: field.parkingZone[0].x + field.parkingZoneDirection.x,
               y: field.parkingZone[0].y + field.parkingZoneDirection.y};
    res += encodePoint(dir) + ")";

    let parkingZoneEncoding = res;

    let randomIdxSet = randomSet(5,5);
    for(let i = 1; i <= 5; i++){
        let box = encodeBox(field.boxes[randomIdxSet[i-1]-1], nextIntIn(0, 2));
        res += box
    }
    fields[0] = res;

    res = parkingZoneEncoding;
    randomIdxSet = randomSet(5,5);
    for(let i = 1; i <= 5; i++){
        let box = encodeBox(field.fakeBoxes1[randomIdxSet[i-1]-1], nextIntIn(0, 2));
        res += box
    }
    fields[1] = res;



    res = parkingZoneEncoding;
    randomIdxSet = randomSet(5,5);
    for(let i = 1; i <= 5; i++){
        let box = encodeBox(field.fakeBoxes2[randomIdxSet[i-1]-1], nextIntIn(0, 2));
        res += box
    }
    fields[2] = res;


    res = parkingZoneEncoding;
    randomIdxSet = randomSet(5,5);
    for(let i = 1; i <= 5; i++){
        let box = encodeBox(field.fakeBoxes3[randomIdxSet[i-1]-1], nextIntIn(0, 2));
        res += box
    }
    fields[3] = res;

    return fields;
}