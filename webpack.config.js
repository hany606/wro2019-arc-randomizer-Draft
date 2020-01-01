const path = require('path');

module.exports = {
    mode: "production",
    context: path.resolve(__dirname),
    entry: './src/frontend/main.js',
    output: {
        filename: "bundle.js",
        path:  path.resolve(__dirname, "public")
    },
    devtool: "source-map"
};
