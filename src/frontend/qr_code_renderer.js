'use strict';

import encodeField from '../util/encode_field';
const QRCode = require('qrcode');



export default function createQR(field, code) {
    let canvas = document.getElementById("qr-code-top-left");

    let encoded_text = document.getElementById("encoded-field");
    encoded_text.appendChild(document.createTextNode(code));

    QRCode.toCanvas(canvas, code,
        {
            width: 227,
            height: 227,
            color: { dark: "#000000", light : "#ffffff" },
            errorCorrectionLevel: 'H'
        },
        function (error) {
            if (error) console.error(error);
        }
    );

    var ctx = canvas.getContext("2d");
    ctx.lineWidth = "3";
    ctx.strokeStyle = "black";
    ctx.rect(5, 5, 227, 227);
    ctx.stroke();
}
