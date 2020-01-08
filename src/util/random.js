

export function nextInt(max) {
    return Math.floor(Math.random() * max);
}

export function nextIntIn(min, max) {
    return min + Math.floor(Math.random() * (max - min));
}

export function randomSet(size, max) {
    var arr = [];
    while(arr.length < size){
        var r = nextIntIn(0,max)+1;
        if(arr.indexOf(r) === -1) arr.push(r);
    }
    return arr;
}