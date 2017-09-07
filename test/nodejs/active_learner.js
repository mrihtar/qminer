/**
 * Copyright (c) 2015, Jozef Stefan Institute, Quintelligence d.o.o. and contributors
 * All rights reserved.
 * 
 * This source code is licensed under the FreeBSD license found in the
 * LICENSE file in the root directory of this source tree.
 */

// JavaScript source code
var qm = require('qminer');
var la = qm.la;
var AL = qm.analytics.AL;
var assert = require("../../src/nodejs/scripts/assert.js");

describe("Active learning tests", function () {
    describe("Constructor test", function () {
        it("should return a default constructor", function () {
            var al = new AL();
            assert(al.X == null);
            assert.equal(al.y.size, 0);
            assert.equal(al.settings.SVC.c, 1);
            assert.equal(al.settings.SVC.j, 1);
            assert.equal(al.settings.SVC.algorithm, "LIBSVM");
        });

    });

    describe("Labelled and unlabelled set test", function () {
        it("should correctly return labelled and unlabelled set", function () {
            var al = new AL();

            var X = new la.Matrix([
                [-1, 1],
                [-1, 0],
                [-1, -1],
                [1, 1],
                [1, 0],
                [1, -1]
            ]).transpose(); // column examples
            al.setX(X);

            var y = new Map();
            y.set(0, -1);
            y.set(2, -1);
            y.set(3, 1);
            y.set(5, 1);
            al.sety(y);

            assert.deepEqual(al._getLabIdxArr(), [0, 2, 3, 5]);
            assert.deepEqual(al._getLabArr(), [-1, -1, 1, 1]);
            assert.deepEqual(al._getUnlabIdxArr(), [1, 4]);
        });
    });
    describe("AL loop test", function () {
        it("should query the example nearer to the hyperplane", function () {
            var al = new AL();

            var X = new la.Matrix([
                [-2, 1],
                [-1.1, 0],
                [-2, -1],
                [0, 1],
                [0, 0],
                [0, -1]
            ]).transpose(); // column examples
            al.setX(X);

            var y = new Map();
            y.set(0, -1);
            y.set(2, -1);
            y.set(3, 1);
            y.set(5, 1);
            al.sety(y);

            var qidx = al.getQueryIdx(2);
            var qidx2 = al.getQueryIdx(3);
            assert.deepEqual(qidx, [1, 4]);
            assert.deepEqual(qidx2, [1, 4]);
        });

    });
});