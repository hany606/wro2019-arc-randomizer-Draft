

export function nextInt(max) {
    return Math.floor(Math.random() * max);
}



export function nextIntIn(min, max) {
    return min + Math.floor(Math.random() * (max - min));
}

