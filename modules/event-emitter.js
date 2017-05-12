exports.EventEmitter = (function () {

    function EventEmitter() {

    }

    function check(e, l) {
        if (!e._listeners) e._listeners = {};
        return e._listeners[l] || (e._listeners[l] = []);
    }

    var p = EventEmitter.prototype;
    const slice = Array.prototype.slice;

    p.on = function on(event, callback, ctx, once) {
        var list = check(this, event);
        list.push({
            event: event,
            callback: callback,
            ctx: ctx || this,
            once: once == undefined ? false : once
        });
        return this;
    }

    p.emit = function emit(event) {
        if (typeof event !== 'string') {
            throw TypeError("event name must be a string");
        }
        var args = slice.call(arguments, 1);
        var events = check(this, event);
        for (var i = 0, ii = events.length; i < ii; i++) {
            var e = events[i]
            e.callback.apply(e.ctx, args);
        }
    }


    return EventEmitter;

})();