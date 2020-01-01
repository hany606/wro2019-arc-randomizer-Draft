import * as child_process from "child_process";
const generator_path = "generator/bin/generator";
const util = require('util');

// it's exported just for means of testing
// also it's worth noticing that the function does not exactly generate the field, but calls a generator, which is an executable file,
// and returns an object built from JSON representation of a field provided by stdout of the called generator,
// also saving stderr as a field of the object
export default async function generateField() {
    const exec = util.promisify(child_process.exec);

    const { stdout, stderr } = await exec(generator_path);

    let f = {};
    f = JSON.parse(stdout);
    if(stderr.toString().length > 0)
        f.err = stderr;
    else {
        f.err = null;
    }
    return f;
}
