'use strict';

import * as jsPDF from 'jspdf';

const A4_SIZE = {width: 210, height: 297};

export function generate_qualification() {
    let logo = document.getElementsByTagName("img")[0];
    let field = document.getElementById("field-canvas");

    let true_encoded_string = document.getElementById("true-encoded-field");
    
    let true_qr_code_string = String(true_encoded_string.innerText);
    true_qr_code_string = true_qr_code_string.substring(44, true_qr_code_string.length);

    let true_qr_code = document.getElementById("qr-code-" + true_qr_code_string);
    let true_encoded_qr_code_string = document.getElementById("encoded-field-" + true_qr_code_string);
    let true_encoded_string_inner_text = String(true_encoded_qr_code_string.innerText).substring(true_qr_code_string.length+1,String(true_encoded_qr_code_string.innerText).length)

    let element_coords = document.getElementById("field-descr");

    let doc = new jsPDF({ unit: 'mm', format: "a4" });

    doc.text("Qualification Round", 100, 0);


    let shift = 0;
    for (let coords of element_coords.children) {
        doc.text(coords.innerText, 100, 20 + shift);
        shift += 10;
    }

    doc.addImage(logo, 'PNG', 5, 0, 55, 10);
    doc.addImage(field, 'PNG', 0, 15, 100, 100);
    doc.setFontSize(9);
    doc.text("All the QR-codes are correct and true for the playground", 100, 80);
    doc.text("top-right: "+true_encoded_string_inner_text, 100, 85);
    doc.text("top-left: "+true_encoded_string_inner_text, 100, 90);
    doc.text("bottom-left: "+true_encoded_string_inner_text, 100, 95);
    doc.text("bottom-right: "+true_encoded_string_inner_text, 100, 100);
    

    doc.rect(15,120,150,150);
    doc.addImage(true_qr_code, 'PNG', 22.5, 127.5, 60, 60);
    doc.addImage(true_qr_code, 'PNG', 97.5, 127.5, 60, 60);
    doc.addImage(true_qr_code, 'PNG', 22.5, 202.5, 60, 60);
    doc.addImage(true_qr_code, 'PNG', 97.5, 202.5, 60, 60);

    
    doc.text("Generated by " + window.location.href, 15, A4_SIZE.height - 10);

    doc.save('a4_qualification.pdf');
}


export function generate_final() {
    let logo = document.getElementsByTagName("img")[0];
    let field = document.getElementById("field-canvas");
    let qr_code_top_right = document.getElementById("qr-code-top-right");
    let qr_code_top_left = document.getElementById("qr-code-top-left");
    let qr_code_bottom_left = document.getElementById("qr-code-bottom-left");
    let qr_code_bottom_right = document.getElementById("qr-code-bottom-right");

    let true_encoded_string = document.getElementById("true-encoded-field");
    let encoded_string_top_right = document.getElementById("encoded-field-top-right");
    let encoded_string_top_left = document.getElementById("encoded-field-top-left");
    let encoded_string_bottom_left = document.getElementById("encoded-field-bottom-left");
    let encoded_string_bottom_right = document.getElementById("encoded-field-bottom-right");
    
    let element_coords = document.getElementById("field-descr");

    let doc = new jsPDF({ unit: 'mm', format: "a4" });

    doc.text("Final Round", 100, 0);

    let shift = 0;
    for (let coords of element_coords.children) {
        doc.text(coords.innerText, 100, 20 + shift);
        shift += 10;
    }

    doc.addImage(logo, 'PNG', 5, 0, 55, 10);
    doc.addImage(field, 'PNG', 5, 15, 100, 100);
    doc.setFontSize(10);
    doc.text(true_encoded_string.innerText, 100, 80);
    doc.text(encoded_string_top_right.innerText, 100, 85);
    doc.text(encoded_string_top_left.innerText, 100, 90);
    doc.text(encoded_string_bottom_left.innerText, 100, 95);
    doc.text(encoded_string_bottom_right.innerText, 100, 100);
    

    doc.rect(15,120,150,150);
    doc.addImage(qr_code_top_left, 'PNG', 22.5, 127.5, 60, 60);
    doc.addImage(qr_code_top_right, 'PNG', 97.5, 127.5, 60, 60);
    doc.addImage(qr_code_bottom_left, 'PNG', 22.5, 202.5, 60, 60);
    doc.addImage(qr_code_bottom_right, 'PNG', 97.5, 202.5, 60, 60);

    
    doc.text("Generated by " + window.location.href, 15, A4_SIZE.height - 10);

    doc.save('a4_final.pdf');
}
