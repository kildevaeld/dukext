var EventEmitter = require('events').EventEmitter

var e = new EventEmitter();

e.once('event', function (w) {
    console.log('Hello, ', w);
});

var i = setInterval(function () {
    console.log('interval');
    e.emit('event', 'World!');
}, 1000);

setTimeout(function () {
    clearInterval(i);
}, 3000);
/*
function fib(n) {
    if (n < 2) return n;
    return fib(n - 2) + fib(n - 1);
}

console.log(fib(35));*/

var a = uv.async(function () {
    console.log('Async Hello');
});

setTimeout(function () {
    console.log('Time Hello')

}, 1000)


console.log(process.version);

//process.stdout.write("Hello");

uv.ttyCreate(1, false);