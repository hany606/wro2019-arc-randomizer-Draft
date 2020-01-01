"use strict";

import {expect} from 'chai';
import generateField from "../generate_field";



describe('generateField', function () {
    it('Returns a field object', function (done) {
        generateField().then((res) => {
            expect(res).to.be.an("object");
            done();
        }).catch(done);
    });
});