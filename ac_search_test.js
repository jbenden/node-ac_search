process.env.NODE_ENV = "test";
var ac_search = require('./ac_search');

function ok(expr, msg) {
    if (!expr) throw new Error(msg);
}

describe("Aho-Corasick Pattern Matching Algorithm", function (done) {
    it("allocates a structure", function (done) {
        var root = new ac_search.ac_search();
        ok(root != 0);
        done();
    });
    it("builds a structure", function (done) {
        var root = new ac_search.ac_search();
        root.add("test");
        root.compile();
        ok(root != 0);
        done();
    });
    it("does not allow function call for initialization", function (done) {
        try {
            var root = ac_search.ac_search();
            throw Error("Can call class as function!");
        } catch(e) {
            done();
        }
    });
    it("searches a structure", function (done) {
        var root = new ac_search.ac_search();
        root.add("test");
        root.add("dogfood");
        root.compile();
        var ret = root.search("time");
        //console.log("ret=" + ret);
        ok(ret === undefined);
        var ret1 = root.search("testing");
        //console.log("ret1='" + ret1 + "'");
        ok(ret1[0] == "t");
        ok(ret1[1] == "e");
        ok(ret1[2] == "s");
        ok(ret1[3] == "t");
        ok(ret1 !== 'test');
        done();
    });
    it("catches errors thrown", function (done) {
        var root = new ac_search.ac_search();
        try {
            root.add(13);
            throw Error("Allows adding an integer value");
        } catch (e) {
        }
        try {
            root.add();
            throw Error("Allows adding an empty value.");
        } catch (e) {
        }
        root.add("search");
        root.compile();
        var res = root.search("search");
        ok(res[0] == 's');
        try {
            root.search(123);
            throw Error("Allows searching of an integer value.");
        } catch (e) {
        }
        done();
    });
});

