'use strict';

import {render, createApp} from './renderer';

import createQR from './qr_code_renderer';
import getField from "./get_field";
import encodeField from "../util/encode_field";
import generate from "./pdf_generator";


window.onload = () => {
    getField().then((field) => {
        let encoded_descr = encodeField(field);
        render(field, encoded_descr);
        createQR(field, encoded_descr);

        document.getElementById("get-pdf-button").addEventListener('click', generate);

    }).catch(err => {
        console.log("ERROR: " + err);
    });
};

