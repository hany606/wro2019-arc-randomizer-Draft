import * as PIXI from "pixi.js";

function drawParkingZoneFreeZone(p1, p2, p3, p4) {
    for (let i = 0; i < arguments.length; i++) {
        arguments[i].x = sm_to_px(arguments[i].x);
        arguments[i].y = sm_to_px(arguments[i].y);
    }
    let border = new PIXI.Graphics();

    border.lineStyle(1, 0xAAAA00);
    border.moveTo(p1.x, p1.y);
    border.lineTo(p2.x, p2.y);
    border.moveTo(p3.x, p3.y);
    border.lineTo(p4.x, p4.y);

    border.lineStyle(1, 0xFF0000);
    border.drawCircle(p1.x, p1.y, sm_to_px(460));
    border.drawCircle(p4.x, p4.y, sm_to_px(460));

    app.stage.addChild(border);

}


function drawPath(field) {
    let circles = new PIXI.Graphics();
    circles.lineStyle(2, 0xAA00AA);
    for(let p of field.path) {
        let destX = MARGIN + sm_to_px(p.x), destY = MARGIN + sm_to_px(p.y);
        circles.drawCircle(destX, destY, sm_to_px(230));
    }
    app.stage.addChild(circles);

    let path = new PIXI.Graphics();
    path.lineStyle(2, 0x0000AA);
    path.moveTo(MARGIN + sm_to_px(field.path[0].x), MARGIN + sm_to_px(field.path[0].y));
    for(let p of field.path) {
        let destX = MARGIN + sm_to_px(p.x), destY = MARGIN + sm_to_px(p.y);
        path.lineTo(destX, destY);
    }
    app.stage.addChild(path);

}