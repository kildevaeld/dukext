const fs = require('fs'),
    Path = require('path'),
    uglify = require('uglify-js');

const files = [
    "net",
    "tty",
    "stream",
    "util",
    "event-emitter",
    "prelude",
    "promise",
    "timers"
];

function fileToHexArray(file) {
    return new Promise((resolve, reject) => {


        fs.readFile(file, function (err, buf) {
            if (err) return reject(err);
            let out = [];

            buf = new Buffer(uglify.minify(buf.toString()).code);
            for (let i of buf) {
                out.push('0x' + Number(i).toString(16));
            }

            resolve({
                name: Path.basename(file).replace(/[.-]/mg, '_'),
                data: out,
                size: buf.length
            });
        });
    });
}


let promises = files.map(m => {
    let path = Path.join(process.cwd(), 'modules', m + '.js');
    return fileToHexArray(path);
});


function write_output(file) {
    let buf = `unsigned char ${file.name}[] = {\n  `


    for (let i = 0, ii = file.data.length; i < ii; i++) {
        if (i % 12 == 0 && i != 0) {
            buf += '\n  ';
        }
        buf += file.data[i];
        if (i != ii - 1) buf += ', ';
    }
    buf += `\n};\nunsigned int ${file.name}_len = ${file.data.length};\n`;



    return buf;
}

function write_header(file) {
    let buf = `unsigned char ${file.name}[];\n`

    buf += `unsigned int ${file.name}_len;\n`;

    return buf;
}

Promise.all(promises)
    .then(files => {
        let size = files.reduce((p, c) => {
            return p + c.size;
        }, 0)
        console.log('Size: %d', size);
        let sources = files.map(write_output);
        /*
        let data = "#ifndef DUKEXT_MODULES_H\n#define DUKEXT_MODULES_H\n\n"
        data += headers.join('\n') + "\n#endif"

        fs.writeFileSync('src/module_data.h', data);*/

        data = `\n${sources.join('\n')}`;
        fs.writeFileSync('src/module_data.c', data);
    }).catch(e => {
        console.log('error', e)
    })