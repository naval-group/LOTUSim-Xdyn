FROM debian:bullseye-slim

RUN apt-get update \
 && apt-get install --yes \
        hdf5-tools \
 && apt-get autoremove \
 && apt-get autoclean \
 && apt-get clean \
 && rm -rf /tmp/* /var/tmp/* /var/lib/apt/lists/*

ENTRYPOINT ["h5dump"]
