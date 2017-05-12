(function () {

    var EventEmitter = require('events').EventEmitter

    function Stream(handle) {
        //EventEmitter.call(this);
        this._h = handle;
    }

    var proto = Stream.prototype;

    proto.read = function read() {
        uv.read_start(this._h, function () {
            console.log('read')
        });
    }

    proto.write = function write() {
        uv.write_start(this._h, function () {
            console.log('write');
        });
    }

    proto.close = function close() {
        uv.close();
    }


    this.Stream = Stream;

    this.process = {
        version: Duktape.version,
        //stdout: new Stream(uv.ttyCreate(1, false))
    }

})();