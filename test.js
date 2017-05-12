var promise = new Promise(function (resolve, reject) {
    setTimeout(function () {
        resolve('World');
    }, 10000);
});

promise.then(function (what) {
    console.log('Hello');
});

var timer = setTimeout(function () {
    throw new Error('dssds')
}, 2000);