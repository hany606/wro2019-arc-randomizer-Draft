#!/usr/bin/env bash

if [[ ! -e "node_modules" ]]; then
    npm install
fi

if [[ ! -e "generator/bin/generator" ]]; then
    echo 'The generator is not built; Proceed to building the generator'

    cd "scripts"
    if bash "build-generator.sh"; then
        :
    else
        echo 'Generator build failed'
        exit -1
    fi
    cd ".."
fi

if npm run build; then
    :
else
    echo 'Server build failed'
    exit -1
fi

if [[ ! -e "public/bundle.js" ]]; then
    npm run pack
fi

echo 'Running the server...'
node "src.transpiled/server/app.js"