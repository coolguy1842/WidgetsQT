#!/usr/bin/env bash

dir="$(realpath $(dirname $0))"

meson setup build --reconfigure
cd build

meson compile -j12

cd ..

# use this one for more detailed stack traces
# ulimit -n 4096; env GDK_BACKEND=wayland ASAN_OPTIONS=fast_unwind_on_malloc=0 ./build/Config/widgets ${@,2}
ulimit -n 4096; env GDK_BACKEND=wayland QT_QPA_PLATFORM=wayland ./build/Config/widgets ${@,2}