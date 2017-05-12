const EventEmitter = require('events').EventEmitter;

var e = new EventEmitter();

e.on('test', function (a) {
    console.log('Got: ', a);
});

var promise = new Promise(function (resolve, reject) {
    setTimeout(function () {
        resolve('World');
    }, 10000);
});

promise.then(function (what) {
    console.log('Hello');
});

var timer = setTimeout(function () {
    e.emit('test', 'Hello, World');
}, 1000);