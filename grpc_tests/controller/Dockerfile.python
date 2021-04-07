FROM sirehna/base-image-debian10-gcc8-xdyn
RUN apt-get update -yq && \
    apt-get install \
        --yes \
        --no-install-recommends \
        python3-pip \
        python3-setuptools \
        python3-dev
RUN pip3 install wheel
ADD grpc_docker/requirements.txt .
RUN pip3 install -r requirements.txt && \
    mkdir -p /usr/python && \
    chmod a+rwx /usr/python
ENV PYTHONPATH /usr/python
ADD grpc_tests/controller/wait-for-it.sh ./
ADD code/grpc/controller.proto .
RUN python3 -m grpc_tools.protoc -I . --python_out=/usr/python --grpc_python_out=/usr/python/ controller.proto
ADD grpc_docker/controller.py /usr/python/
ADD grpc_tests/controller/pid_controller.py grpc_tests/controller/grpc_controller_client.py ./
