#!/bin/sh
rm -f .dockerignore
echo "./doc" > .dockerignore
echo "./build_deb11" >> .dockerignore
echo "./build_deb11_dbg" >> .dockerignore
echo "./.git" >> .dockerignore
echo "./validation" >> .dockerignore
docker run -it --rm \
    -u $(id -u):$(id -g) \
    -v "$(pwd)":/opt/share \
    -w /opt/share \
    sirehna/base-image-win64-gccx-posixthreads-ssc-xdyn /bin/bash -c \
       "cd build_win_posix &&\
        mkdir -p /opt/share/.wine;\
        export WINEPREFIX=/opt/share/.wine;\
        wine winecfg;\
        wine ./run_all_tests `echo $*`"
