import Field from '../field';

export default function getMockField() {
    return new Field(getMockFieldDescription());
}

export function getMockFieldDescription() {
    return {
        parkingZone: [
            { x: 1150, y: 920 },
            { x: 1458, y: 765 },
            { x: 1613, y: 1073 },
            { x: 1305, y: 1228 }],
        parkingZoneDirection: { x: 230, y: -115 },
        boxes: [
            {top: 1725, left: 1955, bott: 1955, right: 2185 },
            {top: 115, left: 575, bott: 345, right: 805 },
            {top: 1840, left: 1150, bott: 2070, right: 1380 },
            {top: 460, left: 230, bott: 690, right: 460 },
            {top: 1380, left: 1840, bott: 1610, right: 2070 }],
        boxColors: [
            "Blue",
            "Green",
            "Orange",
            "Red",
            "Yellow"],
        cubeColors: [
            "Orange",
            "Blue",
            "Green",
            "Yellow",
            "Red"]
    };
}