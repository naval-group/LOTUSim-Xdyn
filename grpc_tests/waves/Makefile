.PHONY: all clean

all: tutorial_09_gRPC_wave_model.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build

tutorial_09_gRPC_wave_model.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
		-c "cp /usr/demos/tutorial_09_gRPC_wave_model.yml /xdyn_demos/tutorial_09_gRPC_wave_model.yml" 

clean:
	rm -f tutorial_09_gRPC_wave_model.yml
