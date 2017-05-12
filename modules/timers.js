/*
 *  C eventloop example (c_eventloop.c).
 *
 *  Ecmascript code to initialize the exposed API (setTimeout() etc) when
 *  using the C eventloop.
 *
 *  https://developer.mozilla.org/en-US/docs/Web/JavaScript/Timers
 */

/*
 *  Timer API
 */

(function () {

    function startTimer(func, delay, repeat) {
        var cb_func;
        var bind_args;
        var timer_id;

        if (typeof delay !== 'number') {
            throw new TypeError('delay is not a number');
        }

        if (typeof func === 'string') {
            // Legacy case: callback is a string.
            cb_func = eval.bind(this, func);
        } else if (typeof func !== 'function') {
            throw new TypeError('callback is not a function/string');
        } else if (arguments.length > 2) {
            // Special case: callback arguments are provided.
            bind_args = Array.prototype.slice.call(arguments, 2); // [ arg1, arg2, ... ]
            bind_args.unshift(this); // [ global(this), arg1, arg2, ... ]
            cb_func = func.bind.apply(func, bind_args);
        } else {
            // Normal case: callback given as a function without arguments.
            cb_func = func;
        }

        timer_id = uv.timerCreate();
        uv.timerStart(timer_id, delay, repeat, func);
        return timer_id;
    }

    this.setTimeout = function setTimeout(func, delay) {
        return startTimer(func, delay, 0);
    }

    this.clearInterval = this.clearTimeout = function clearTimeout(timer_id) {
        var success = uv.close(timer_id); /* retval ignored */
    }

    this.setInterval = function setInterval(func, delay) {
        return startTimer(func, delay, delay);
    }

})();