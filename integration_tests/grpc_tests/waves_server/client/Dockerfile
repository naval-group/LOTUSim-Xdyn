FROM python:3.10-slim
RUN apt-get update -yq \
 && apt-get install \
        --yes \
        --no-install-recommends \
        wait-for-it \
 && python -m pip install --upgrade pip \
 && python -m pip install \
        grpcio-tools \
        numpy \
 && mkdir -p /usr/python \
 && chmod a+rwx /usr/python
ENV PYTHONPATH /usr/python
ADD wave_types.proto .
ADD wave_grpc.proto .
RUN python -m grpc_tools.protoc -I . --python_out=/usr/python --grpc_python_out=/usr/python wave_types.proto
RUN python -m grpc_tools.protoc -I . --python_out=/usr/python --grpc_python_out=/usr/python wave_grpc.proto
ADD waves.py .
ENTRYPOINT ["python"]
