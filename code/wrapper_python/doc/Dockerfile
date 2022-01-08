FROM debian:bullseye-slim
RUN apt-get update -yq \
 && apt-get install --yes --no-install-recommends \
    git \
    make \
    python3 \
    python3-numpy \
    python3-pip \
 && python3 -m pip install -U pip \
 && python3 -m pip install \
    sphinx \
    sphinx-autopackagesummary
ADD xdyn*39*-linux_x86_64.whl /
RUN python3 -m pip install --no-index --find-links / xdyn \
 && python3 -c 'import xdyn; print(dir(xdyn)); print(xdyn.__version__)'
