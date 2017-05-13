exports.createHash = (function () {

    var Stream = require('stream').Stream,
        util = require('util');

    util.inherits(Hash, Stream);

    function Hash(hash) {
        Stream.call(this);
        this.hash = hash;
    }

    util.extend(Hash.prototype, {

        update: function (data) {
            if (typeof data === 'string') {
                data = new Buffer(data);
            }
            module.crypto.update(this.hash, data);
            return this;
        },

        digest: function (enc) {
            enc = enc || 'hex';

            var out = module.crypto.digest(this.hash);

            return Duktape.enc(enc, out);

        }
    })


    return function createHash(algo) {
        var o = new Hash(module.crypto.create(algo));
        Duktape.fin(o, function (o) {
            module.crypto.clean(o.hash)
        });
        return o;
    }


})();