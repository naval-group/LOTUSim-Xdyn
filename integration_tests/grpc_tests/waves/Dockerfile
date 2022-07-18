FROM xdyngrpc-python
ADD requirements.txt .
RUN pip3 install -r requirements.txt
ENTRYPOINT wait-for-it xdyn:9002 -s -t 20 -- nose2 -v
