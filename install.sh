#!/bin/sh

# Ensure that Python's SSL certificate file is reachable.  This environment
# variable won't be set by default in `guix shell` but might be set correctly in
# a CI environment.
if [ -z $SSL_CERT_FILE ]; then
    export SSL_CERT_FILE="$(python3 -m certifi)"
fi

# Clone the repository if it isn't already there
if [ ! -d "./emsdk" ]; then
    git clone https://github.com/emscripten-core/emsdk.git
fi

# Install the latest version of the SDK and activate it
cd emsdk
./emsdk install latest
./emsdk activate latest
