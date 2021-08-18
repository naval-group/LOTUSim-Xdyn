#FROM xdyn
FROM sirehna/base-image-debian11-gcc10:2021-08-17
RUN apt-get update -yq && \
    apt-get install --no-install-recommends -y \
    gdb
ENV LD_LIBRARY_PATH=/build
ENTRYPOINT ["/usr/bin/gdb"]
