
'use strict';

import Color from '../util/color';
import {encodePoint} from '../util/encode_field';
import {nextIntIn} from "../util/random";
import * as PIXI from 'pixi.js';

const SM_TO_PX_COEFF = 4;
const WIDTH = sm_to_px(2300)+1;
const HEIGHT = sm_to_px(2300)+1;

const MARGIN = 30;

let app = null;


// field - an object of type Field, containing the description of the field to render
// encoded_descr - a string with some important data about the field
export function render(field, encoded_descr) {
    let canvasFields = ["final", "qualification"];
    for(let i in canvasFields){
        createApp(canvasFields[i]);

        // draw letters on field border (as in the sea battle game)
        for (let i = 1; i <= 20; i++) {
            drawText(i * sm_to_px(115), 10, String.fromCharCode('A'.charCodeAt(0) + i - 1));
            drawText(10, i * sm_to_px(115), String.fromCharCode('A'.charCodeAt(0) + i - 1));
        }

        // draw crosses on the field
        for (let i = 0; i <= sm_to_px(2300); i += sm_to_px(115)) {
            for (let j = 0; j <= sm_to_px(2300); j += sm_to_px(115)) {
                drawCross(MARGIN + i, MARGIN + j, 5);
            }
        }

        drawParkingZone(...field.parkingZone);
        for(let i = 0; i < 5; i++) {
            drawBox(field.boxes[i], Color[field.boxColors[i]].value);
            for(let p of [point(115 - 30, 0), point(0, 115 - 30), point(230 - 60, 115 - 30), point(115 - 30, 230 - 60)]) {
                let b = field.boxes[i];
                drawBox({top: b.top + p.y, left: b.left + p.x, bott: b.top + p.y + 60, right: b.left + p.x + 60},
                    Color[field.cubeColors[i]].value);
            }
        }

        if(canvasFields[i] == "final"){
            //0xcc3300 -> Red
            for(let i = 0; i < 5; i++)
                drawFakeBox(field.fakeBoxes1[i],0xcc3300);

            //0x006600 -> Green
            for(let i = 0; i < 5; i++)
                drawFakeBox(field.fakeBoxes2[i],0x006600);

            //0x003366 -> Blue
            for(let i = 0; i < 5; i++)
                drawFakeBox(field.fakeBoxes3[i],0x003366);
        }

        // draw strings with field element coordinates
        let descr = document.getElementById("field-descr");
        if(canvasFields[i] == "final"){
            let parkingZoneDescr = document.createElement("p");
            parkingZoneDescr.setAttribute("class", "descr-paragraph");
            let dir = {x: field.parkingZone[0].x + field.parkingZoneDirection.x,
                    y: field.parkingZone[0].y + field.parkingZoneDirection.y};
            parkingZoneDescr.appendChild(document.createTextNode(
                "Parking Zone: (" + encodePoint(field.parkingZone[0]) + " " + encodePoint(dir) + ")"));
            descr.appendChild(parkingZoneDescr);        
        
            for(let i = 0; i < 5; i++) {
                let boxDescr = document.createElement("p");
                let p1;
                let p2;
                p1 = {x: field.boxes[i].right, y: field.boxes[i].top};
                p2 = {x: field.boxes[i].left, y: field.boxes[i].bott};
                p1 = encodePoint(p1);
                p2 = encodePoint(p2);    
                boxDescr.appendChild(document.createTextNode(field.boxColors[i] + ": (" + p1 + " " + p2 + ")"));
                boxDescr.setAttribute("class", "descr-paragraph");
                descr.appendChild(boxDescr);
            }
        }

        drawBorder();
    }
}



function point(x, y) {
    return {x: x, y: y};
}



function createApp(type) {
    initPixi();
    let canvas = document.getElementById("field-canvas-"+type);
    app = new PIXI.Application({width: MARGIN + WIDTH + 5, height: MARGIN + HEIGHT + 5, view: canvas, preserveDrawingBuffer: true});
    app.renderer.backgroundColor = 0xFFFFFF;
}



function initPixi() {
    let type = "WebGL";

    if(!PIXI.utils.isWebGLSupported()){
        type = "canvas";
    }

    PIXI.utils.sayHello(type);
}



function sm_to_px(sm) {
    return sm / SM_TO_PX_COEFF;
}



function drawBorder() {
    let border = new PIXI.Graphics();

    border.lineStyle(5, 0x000000);

    border.moveTo(MARGIN, MARGIN);
    border.lineTo(MARGIN + WIDTH, MARGIN);
    border.lineTo(MARGIN + WIDTH, MARGIN + HEIGHT);
    border.lineTo(MARGIN, MARGIN + HEIGHT);
    border.lineTo(MARGIN, MARGIN);

    app.stage.addChild(border);
}



function drawCross(x, y, size) {
    let cross = new PIXI.Graphics();

    cross.lineStyle(1, 0x888888);

    cross.moveTo(x - size / 2, y);
    cross.lineTo(x + size / 2, y);
    cross.moveTo(x, y - size / 2);
    cross.lineTo(x, y + size / 2);

    app.stage.addChild(cross);
}


function drawParkingZone(p1, p2, p3, p4) {
    let p = [];
    for (let i = 0; i < arguments.length; i++) {
        p.push({ x: MARGIN + sm_to_px(arguments[i].x),
                 y: MARGIN + sm_to_px(arguments[i].y) });
    }
    let contour = new PIXI.Graphics();

    contour.lineStyle(3, 0x00AA00);
    contour.moveTo(0, 0);
    contour.lineTo(p[1].x - p[0].x, p[1].y - p[0].y);

    contour.lineStyle(3, 0x000000);
    contour.lineTo(p[2].x - p[0].x, p[2].y - p[0].y);
    contour.lineTo(p[3].x - p[0].x, p[3].y - p[0].y);

    contour.pivot.x = (p[2].x - p[0].x) / 2;
    contour.pivot.y = (p[2].y - p[0].y) / 2;

    contour.position.x = (p[0].x + p[2].x) / 2;
    contour.position.y = (p[0].y + p[2].y) / 2;

    app.stage.addChild(contour);
}



function drawBox(rect, color=0x0000FF, fill=true) {
    let box = new PIXI.Graphics();

    box.lineStyle(1, 0x000000);
    if(fill) box.beginFill(color);
    box.drawRect(MARGIN + sm_to_px(rect.left), MARGIN + sm_to_px(rect.top),
                    sm_to_px(rect.right - rect.left), sm_to_px(rect.bott - rect.top));
    if(fill) box.endFill();

    app.stage.addChild(box);
}

function drawFakeBox(rect, color=0x000000) {
    let box = new PIXI.Graphics();

    box.lineStyle(1, color, 2);
    box.drawRect(MARGIN + sm_to_px(rect.left), MARGIN + sm_to_px(rect.top),
                    sm_to_px(rect.right - rect.left), sm_to_px(rect.bott - rect.top));

    app.stage.addChild(box);
}


function drawText(x, y, text) {
    let textObj = new PIXI.Text(text, {fontFamily : 'Arial', fontSize: 9, fill : 0xff1010, align : 'center'});
    textObj.x = x;
    textObj.y = y;

    app.stage.addChild(textObj);

    return textObj;
}
