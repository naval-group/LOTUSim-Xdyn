all: wave_grpc.proto wave_types.proto waves.py

DOCKER_IMAGE=xdyn
DOCKER_WORKING_DIRECTORY=/xdyn_demos
DOCKER_RUN := docker run --rm \
    -u $(shell id -u):$(shell id -g) \
    -v $(shell pwd):${DOCKER_WORKING_DIRECTORY} \
    -w ${DOCKER_WORKING_DIRECTORY}
DOCKER_RUN_BASH:= ${DOCKER_RUN} \
    --entrypoint /bin/bash \
    ${DOCKER_IMAGE} -c

wave_grpc.proto:
	@cp ../../../../interfaces/$@ .

wave_types.proto:
	@cp ../../../../interfaces/$@ .

waves.py:
	@cp ../../../../interfaces/waves/python/client/$@ .

clean:
	@rm -f wave_types.proto wave_grpc.proto
	@rm -f waves.py
	@rm -rf __pycache__
