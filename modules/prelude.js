(function () {

    var tty = require('tty');

    this.process = {
        version: Duktape.version,
        stdout: new tty.WriteStream(1, false),
        stdin: new tty.ReadStream(0, true),
        stderr: new tty.WriteStream(2, false)
    };



})();