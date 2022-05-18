FROM python:3.10-slim
RUN apt-get --allow-releaseinfo-change update \
 && apt-get update -yq \
 && apt-get install \
        --yes \
        --no-install-recommends \
        wait-for-it \
 && apt-get autoremove -y
ADD requirements.txt .
RUN pip install -r requirements.txt
ADD model_exchange.proto ./
RUN python -m grpc_tools.protoc -I . --python_out=. --grpc_python_out=. model_exchange.proto
ADD tests.py ./
ENTRYPOINT wait-for-it xdyn:9002 -s -t 20 -- nose2
