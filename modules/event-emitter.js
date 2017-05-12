exports.EventEmitter = (function () {

    /**
     *
     *
     * @export
     * @param {Events[]} fn
     * @param {any[]} [args=[]]
     * @returns
     */
    function callFunc(fn, args) {
        if (args === void 0) {
            args = [];
        }
        var l = fn.length,
            i = -1,
            a1 = args[0],
            a2 = args[1],
            a3 = args[2],
            a4 = args[3];
        switch (args.length) {
            case 0:
                while (++i < l)
                    fn[i].handler.call(fn[i].ctx);
                return;
            case 1:
                while (++i < l)
                    fn[i].handler.call(fn[i].ctx, a1);
                return;
            case 2:
                while (++i < l)
                    fn[i].handler.call(fn[i].ctx, a1, a2);
                return;
            case 3:
                while (++i < l)
                    fn[i].handler.call(fn[i].ctx, a1, a2, a3);
                return;
            case 4:
                while (++i < l)
                    fn[i].handler.call(fn[i].ctx, a1, a2, a3, a4);
                return;
            default:
                while (++i < l)
                    fn[i].handler.apply(fn[i].ctx, args);
                return;
        }
    }

    function removeFromListener(listeners, fn, ctx) {
        for (var i = 0; i < listeners.length; i++) {
            var e = listeners[i];
            if ((fn == null && ctx != null && e.ctx === ctx) ||
                (fn != null && ctx == null && e.handler === fn) ||
                (fn != null && ctx != null && e.handler === fn && e.ctx === ctx)) {
                listeners.splice(i, 1);
            }
        }
        return listeners;
    }

    function EventEmitter() {

    }

    function check(e, l) {
        return (e._listeners || (e._listeners = {}))[l] || (e._listeners[l] = []);
    }

    var p = EventEmitter.prototype;
    const slice = Array.prototype.slice;

    p.on = function on(event, callback, ctx, once) {

        check(this, event).push({
            name: event,
            handler: callback,
            ctx: ctx || this,
            once: once == undefined ? false : once
        });
        return this;
    }

    p.once = function once(event, callback, ctx) {
        return this.on(event, callback, ctx, true)
    }

    p.off = function off(eventName, fn, ctx) {
        this._listeners = this._listeners || {};
        if (eventName == null && ctx == null) {
            this._listeners = {};
        } else if (this._listeners[eventName]) {
            var events = this._listeners[eventName];
            if (fn == null && ctx == null) {
                this._listeners[eventName] = [];
            } else {
                removeFromListener(events, fn, ctx);
            }
        } else {
            for (var en in this.listeners) {
                var l = this.listeners[en];
                removeFromListener(l, fn, ctx);
            }
        }
        return this;
    }

    p.emit = function emit(eventName) {
        var args = [];
        for (var _i = 1; _i < arguments.length; _i++) {
            args[_i - 1] = arguments[_i];
        }
        this._listeners = this._listeners || {};
        var events = (this._listeners[eventName] || []).concat(this._listeners["*"] || []);
        var event, a, index;
        var calls = [];
        var alls = [];
        for (var i = 0, ii = events.length; i < ii; i++) {
            event = events[i];
            a = args;
            if (events[i].name == 'all' || events[i].name == '*') {
                alls.push(events[i]);
            } else {
                calls.push(events[i]);
            }
            if (events[i].once === true) {
                index = this._listeners[events[i].name].indexOf(events[i]);
                this._listeners[events[i].name].splice(index, 1);
            }
        }
        if (alls.length) {
            var a_1 = [eventName].concat(args);
            callFunc(alls, a_1);
        }
        if (calls.length)
            callFunc(calls, args);
        return this;
    }


    return EventEmitter;

})();