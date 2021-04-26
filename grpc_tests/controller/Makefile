.PHONY: all

all: tutorial_11_gRPC_controller.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

tutorial_11_gRPC_controller.yml:
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_11_gRPC_controller.yml /xdyn_demos/"
