#!/bin/sh

# Ensure the output path exists
mkdir -p out

# For some reason, `guix shell` isn't setting `LD_LIBRARY_PATH` correctly.
# Thus, dependencies installed in the container don't get picked up correctly by
# pre-compiled applications.  Fix that by using the value of `LIBRARY_PATH`.
export LD_LIBRARY_PATH=$LIBRARY_PATH

# Since Guix installs Python 3 as `python3`, set `EMSDK_PYTHON` to reflect the
# correct command name.
export EMSDK_PYTHON=python3

# Pull in the emsdk environment variables by running `emsdk_env.sh`.  We enter
# the directory so that the script can be sourced correctly in CI servers with
# weird shell configurations (I'm looking at you, Ubuntu).
cd emsdk
. ./emsdk_env.sh
cd ..

# Set EMSDK_PYTHON again because `emsdk_env.sh` clears it for some reason...
export EMSDK_PYTHON=python3

# Activate the latest version of the SDK (might be unnecessary after initial setup)
emsdk activate latest

# The `C_INCLUDE_PATH` will cause header files installed by Guix to get picked
# up by `clang`.  For now we don't want that, so clear the variable so that
# Emscripten's own headers will get used.
unset C_INCLUDE_PATH

# Use `emmake` to wrap a call to `pkg-config` so that Emscripten's package
# definitions are used to find its own ported GLFW library
export LIBS="$(emmake pkg-config --libs glfw3)"

# Build main.c with `emcc`, specifying the WebGL version and including the
# `assets` folder
emcc main.c -o out/index.html \
     $LIBS \
     -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 \
     --preload-file assets

# If not running in CI, host the browser page with a local server for testing
if [ -z $CI ]; then
  emrun --no_browser out/index.html
fi
