.PHONY: all clean test

all: test clean

test: test.py tutorial_11.csv
	@echo "Checking heading converged for both setpoints..."
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work --entrypoint /usr/bin/python3 python:3.7 test.py
	@echo "All done!"

tutorial_11_gRPC_controller.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_11_gRPC_controller.yml /xdyn_demos/"

tutorial_11.csv: tutorial_11_gRPC_controller.yml
	@echo "Running simulation..."
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans
	rm tutorial_11_gRPC_controller.yml

clean:
	rm -rf tutorial_11.csv tutorial_11_gRPC_controller.yml
