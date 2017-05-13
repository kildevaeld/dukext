var Socket = require('net').Socket,
    util = require('util');

exports.WriteStream = (function () {

    util.inherits(WriteStream, Socket);

    function WriteStream(fd) {
        Socket.call(this, uv.ttyCreate(fd, false));

    }

    return WriteStream;

})();

exports.ReadStream = (function () {

    util.inherits(ReadStream, Socket);

    function ReadStream(fd) {
        Socket.call(this, uv.ttyCreate(fd, true));
    }

    return ReadStream;

})();