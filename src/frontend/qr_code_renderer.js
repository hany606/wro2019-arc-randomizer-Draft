'use strict';

import encodeField from '../util/encode_field';
import {randomSet} from "../util/random";
const QRCode = require('qrcode');



export default function createQR(field, code) {
    let canvas = [];
    canvas[0] = document.getElementById("qr-code-top-right");
    canvas[1] = document.getElementById("qr-code-top-left");
    canvas[2] = document.getElementById("qr-code-bottom-left");
    canvas[3] = document.getElementById("qr-code-bottom-right");

    let qr_codes_keys = ["encoded-field-top-right", "encoded-field-top-left", "encoded-field-bottom-left", "encoded-field-bottom-right"];
    let text_colors_per_fake_qr = ["#cc3300", "#006600", "#003366"];


    let random_value = randomSet(4,4);

    for(let i = 1; i <= 4; i++){
        let index = random_value[i-1] - 1;
        
        if(index == 0){
            let true_encoded_field_text = document.getElementById("true-encoded-field");
            true_encoded_field_text.appendChild(document.createTextNode(qr_codes_keys[i-1].slice(14,qr_codes_keys[i-1].length)));
        }

        let encoded_text = document.getElementById(qr_codes_keys[i-1]);
        encoded_text.appendChild(document.createTextNode(code[index]));
        if(index != 0)
            encoded_text.style.color = text_colors_per_fake_qr[index-1];


        QRCode.toCanvas(canvas[i-1], code[index],
            {
                width: 226.77,
                hight: 226.77,
                color: { dark: "#000000", light : "#ffffff" },
                errorCorrectionLevel: 'H',
                margin: 0
            },
            function (error) {
                if (error) console.error(error);
            }
        );
    
    }

}
