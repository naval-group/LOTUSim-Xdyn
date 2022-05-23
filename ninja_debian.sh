#!/bin/sh
rm -f .dockerignore
echo "./doc" > .dockerignore
echo "./build_win_posix" >> .dockerignore
echo "./build_deb11_dbg" >> .dockerignore
echo "./.git" >> .dockerignore
echo "./validation" >> .dockerignore
echo "./.wine" >> .dockerignore

make headers
docker run \
    --name xdyn-ninja-debian \
    --rm \
    -u $( id -u ):$( id -g ) \
    -v "$(pwd)/build_deb11":/build \
    -w /build \
    -v "$(pwd)":/opt/share \
    sirehna/base-image-debian11-gcc10:2021-12-12 \
    /bin/sh -c "cd /build && ninja `echo $*`"
rm -f .dockerignore
