"use strict";

const assert = require('chai').assert;
import Color from '../color.js';


describe("color", function() {
    it("Blue deep equals Blue", function () {
        assert.deepEqual(Color.Blue, Color.Blue);
    });
    it("Blue strictly equals Blue", function () {
        assert.strictEqual(Color.Blue, Color.Blue);
    });
    it("Blue does not equal Red", function () {
        assert.notEqual(Color.Blue, Color.Red);
    });
    it("Is Immutable to Adding Fields", function () {
        assert.throws(() => Color.Pink = "Whatever", TypeError);
    });
    it("Is Immutable to Reassigning Fields", function () {
        assert.throws(() => Color.Red = "Else", TypeError);
    });
    it("Is Immutable to Changing Fields", function () {
        assert.throws(() => Color.Red.r = 100, TypeError);
    });

    describe("#randomColor", function () {
        it('Generates valid color', function () {
            for (let i = 0; i < 10; i++) {
                let c = Color.randomColor();
                assert.deepEqual(c, Color[c.name], "Actual name: " + c.name);
            }
        });
    });

    describe("#randomColorExcluding", function () {
        it('Generates valid color which is not in excluded list', function () {
            for (let i = 0; i < 30; i++) {
                let c = Color.randomColorExcluding([Color.Blue, Color.Red]);
                assert.notDeepEqual(c, Color.Red);
                assert.notDeepEqual(c, Color.Blue);
                assert.deepEqual(c, Color[c.name], "Actual name: " + c.name);
            }
        });
    });
});
