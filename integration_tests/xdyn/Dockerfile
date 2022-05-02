FROM debian:bullseye-slim

RUN apt-get update \
 && apt-get install --yes \
        hdf5-tools \
        python3 \
        python3-h5py \
        python3-numpy \
 && apt-get autoremove \
 && apt-get autoclean \
 && apt-get clean \
 && rm -rf /tmp/* /var/tmp/* /var/lib/apt/lists/*

ENTRYPOINT ["python3"]
