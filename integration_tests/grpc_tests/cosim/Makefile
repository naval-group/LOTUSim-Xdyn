.PHONY: all clean run

all: run clean

run: cosimulation.proto tutorial_01_falling_ball.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build

cosimulation.proto: ../../../interfaces/cosimulation.proto
	cp $< $@

tutorial_01_falling_ball.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_01_falling_ball.yml /xdyn_demos/"

clean:
	rm -f tutorial_01_falling_ball.yml cosimulation.proto
