FROM continuumio/miniconda:4.7.12
ADD requirements.txt .
RUN conda install --yes --file requirements.txt
ADD wait-for-it.sh ./
ADD tests.py ./
ENTRYPOINT ./wait-for-it.sh xdyn:9002 -s -t 20 -- nosetests -x -v -d --nocapture --nologcapture
