'use strict';

import Color from './color';



export default class Field {

    constructor(descr) {
        this.err = descr.err;

        function isPoint(p) {
            return typeof p.x === 'number' && typeof p.y === 'number';
        }

        function isRect(p) {
            return typeof p.top === 'number' && typeof p.left === 'number' &&
                typeof p.bott === 'number' && typeof p.right === 'number';
        }

        function isValidArrayOf(arr, validator_callback) {
            for(let v of arr) {
                if(!validator_callback(v)) return false;
            }
            return true;
        }

        function isValidArrayOfN(arr, num, validator_callback) {
            if(!Array.isArray(arr) || arr.length !== num) return false;
            return isValidArrayOf(arr, validator_callback);
        }

        if(typeof descr === 'undefined' || descr === null) throw Error("Field description is undefined or null");

        console.log(descr);

        if(isValidArrayOfN(descr.parkingZone, 4, isPoint)) { // all elements are points

            this.parkingZone = descr.parkingZone;

        } else {
            throw Error("'parkingZone' field of the description should be an array of four points; Got " + descr.parkingZone);
        }

        if(typeof descr.parkingZoneDirection === "object" && isPoint(descr.parkingZoneDirection)) {
            this.parkingZoneDirection = descr.parkingZoneDirection;
        } else {
            throw Error("'parkingZoneDirection' field of the description should be an point; Got " + descr.parkingZoneDirection);
        }

        if(isValidArrayOfN(descr.boxes, 5, isRect)) { // all elements are rects
            this.boxes = descr.boxes;

        } else {
            throw Error("'boxes' field of the description should be an array of five rects; Got " + descr.boxes);
        }

        if(isValidArrayOfN(descr.boxColors, 5, v => typeof Color.hasOwnProperty(v))) {
            this.boxColors = descr.boxColors;

        } else {
            throw Error("'boxColors' field of the description should be an array of five strings denoting colors; Got " + descr.boxColors);
        }

        if(isValidArrayOfN(descr.cubeColors, 5, v => typeof Color.hasOwnProperty(v))) {
            this.cubeColors = descr.cubeColors;

        } else {
            throw Error("'cubeColors' field of the description should be an array of five strings denoting colors; Got " + descr.cubeColors);
        }

        if(isValidArrayOf(descr.path, isPoint)) {
            this.path = descr.path;

        } else {
            throw Error("'path' field of the description should be an array of points; Got " + descr.path);
        }

    }


}
