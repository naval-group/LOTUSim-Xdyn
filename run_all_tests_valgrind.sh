#!/bin/sh
# Using --security-opt seccomp=unconfined to avoid GDB error: warning: Error disabling address space randomization: Operation not permitted
# As per https://stackoverflow.com/questions/35860527/warning-error-disabling-address-space-randomization-operation-not-permitted
docker run $TERMINAL \
    --security-opt seccomp=unconfined \
    --rm \
    -u $( id -u ):$( id -g ) \
    -v $(pwd)/build_deb11_dbg:/build \
    -w /build \
    -it \
    sirehna/base-image-debian11-gcc10:2021-12-12 \
    /bin/bash -c "export LD_LIBRARY_PATH=/build; valgrind --leak-check=full --track-origins=yes --error-exitcode=1 ./run_all_tests `echo $*`"
