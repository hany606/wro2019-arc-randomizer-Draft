"use strict";

import {default as encodeField, encodePoint} from '../encode_field.js';
import getMockField from './mock_field';
import {expect} from 'chai';



describe('encodeField', function () {
    it('Successfully encodes a field to a valid string', function () {
        let field = getMockField();
        expect(encodeField(field)).to.be.a('string');
    });
    it('Successfully encodes a field with correct symbols', function () {
        let field = getMockField();
        let s = encodeField(field);
        expect(s).to.be.a('string');
        // TODO
    });
});

describe('encodePoint', function () {
    it('Successfully encodes a point to a valid string', function () {
        let p = {x: 115*7, y: 115*10};
        expect(encodePoint(p)).to.be.a('string');
    });
    it('Successfully encodes a point with correct symbols', function () {
        let p = {x: 115*7, y: 115*10};
        expect(encodePoint(p)).to.be.equal('HK');
    });
});
