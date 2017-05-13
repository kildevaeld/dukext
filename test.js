/*var EventEmitter = require('events').EventEmitter

var e = new EventEmitter();


e.once('event', function (w) {
    console.log('Hello, ', w);
});*/
/*
var i = setInterval(function () {
    console.log('interval');
    e.emit('event', 'World!');
}, 1000);

setTimeout(function () {
    clearInterval(i);
}, 1200);*/
/*
function fib(n) {
    if (n < 2) return n;
    return fib(n - 2) + fib(n - 1);
}

console.log(fib(35));*/


/*
setTimeout(function () {
    console.log('Time Hello')

}, 1000)
*/

/*

var crypto = require('crypto');

var hash = crypto.createHash('sha256').update("Hello, World").digest('hex');

console.log('rapper', hash);


setTimeout(function () {
    console.log('hello');
}, 2000)
*/
/*
const crypto = require('crypto');

require('crypto');
var hash = crypto.create('sha256');

crypto.update(hash, new Buffer('Hello, World'));
var out = crypto.digest(hash);
console.log(Duktape.enc('hex', out))
crypto.clean(hash);*/
/**/
const crypto = require('crypto');
var out = crypto.createHash('sha256').update('Hello, World').digest()
console.log(out)


//process.stdin.close();

//console.log(process.stdout.write);

//process.stdout.write("Hello");
/*uv.read_start(process.stdin._h, function (err, chunk) {
    if (err) {
        throw err;
    }
    if (!chunk) {
        return uv.read_stop(utils.stdin._h);
    }
    try {
        console.log(chunk.toString());
        p(eval(chunk.toString()));
    } catch (error) {
        //uv.write(utils.stderr, utils.colorize("error", error.toString()) + "\n");
    }
    uv.write(process.stdout._h, "> ");
});
uv.write(process.stdout._h, "> ");*/

//process.stdin.read();