var util = require('util');

exports.Stream = (function () {
    var EventEmitter = require('events').EventEmitter;


    util.inherits(Stream, EventEmitter);

    function Stream() {
        EventEmitter.call(this);
    };


    util.extend(Stream.prototype, {


    });

    return Stream;

})();
/*
exports.Readable = (function () {

    utils.inherits(ReadableStream, exports.Stream)

    function ReadableStream() {
        exports.Stream.call(this);
    }

    util.extend(ReadableStream.prototype, {

        read: function () {

        }

    });

});

exports.Writable = (function () {

    utils.inherits(WritableStream, exports.Stream)

    function WritableStream() {
        exports.Stream.call(this);
    }

    util.extend(WritableStream.prototype, {

        write: function (data) {
            uv.write.call()
        }

    });

});*/