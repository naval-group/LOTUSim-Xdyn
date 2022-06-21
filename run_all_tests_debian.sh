#!/bin/sh
# Using --security-opt seccomp=unconfined to avoid GDB error: warning: Error disabling address space randomization: Operation not permitted
# As per https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted
rm -f .dockerignore
echo "./doc" > .dockerignore
echo "./build_win_posix" >> .dockerignore
echo "./build_deb11_dbg" >> .dockerignore
echo "./.git" >> .dockerignore
echo "./validation" >> .dockerignore
echo "./.wine" >> .dockerignore
docker run $TERMINAL \
    --security-opt seccomp=unconfined \
    --rm \
    -u $( id -u $USER ):$( id -g $USER ) \
    -v $(pwd)/build_deb11:/build \
    -w /build \
    -it \
    sirehna/base-image-debian11-gcc10:2021-12-12 \
    /bin/bash -c "export LD_LIBRARY_PATH=/build; ./run_all_tests `echo $*`"
rm -f .dockerignore
