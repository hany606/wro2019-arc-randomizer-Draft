'use strict';

import encodeField from '../util/encode_field';
const QRCode = require('qrcode');



export default function createQR(field, code) {
    let canvas = document.getElementById("qr-code-top-right");

    let encoded_text = document.getElementById("encoded-field");
    encoded_text.appendChild(document.createTextNode(code));

    QRCode.toCanvas(canvas, code,
        {
            width: 226.77,
            hight: 226.77,
            color: { dark: "#000000", light : "#ffffff" },
            errorCorrectionLevel: 'H'
        },
        function (error) {
            if (error) console.error(error);
        }
    );

    // var ctx = canvas.getContext("2d");
    // ctx.lineWidth = "3";
    // ctx.strokeStyle = "black";
    // ctx.rect(5, 5, 227, 227);
    // ctx.stroke();
}
