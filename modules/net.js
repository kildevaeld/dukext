var util = require('util'),
    Stream = require('stream').Stream;

exports.Socket = (function () {

    util.inherits(Socket, Stream);

    function Socket(handler) {

        this._handle = handler;
        Stream.call(this);
    }

    function start_reading(stream) {

        uv.read_start(stream._handle, function (err, data) {
            if (err) return stream.emit('error', err);
            data = new Buffer(data);
            stream.emit('data', data);
        });
    }

    function pause_reading(stream) {
        uv.stop_read(stream._handle);
    }

    util.extend(Socket.prototype, {


        write: function (data) {
            uv.write(this._handle, data);
        },

        on: function (event, callback, ctx, once) {
            if (event == 'data') {
                start_reading(this);
            }
            return Stream.prototype.on.call(this, event, callback, ctx, once);
        },

        close: function () {
            uv.close(this._handle);
            this.emit('close');
        }

    });

    return Socket;

})()