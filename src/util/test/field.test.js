"use strict";

import Field from '../field.js';
import {expect} from 'chai';
import getMockField, {getMockFieldDescription} from './mock_field';

describe("Mock Field", function () {
   it("Is an object", function () {
       expect(getMockFieldDescription()).to.be.an("object");
   });
});

describe("Field", function () {
    describe("#constructor", function () {
        it("Creates a field from description", function () {
            expect(new Field(getMockFieldDescription())).to.be.an('object');
        });
        it("Fails with empty description", function () {
            expect(() => new Field()).to.throw();
        });
    })
});
