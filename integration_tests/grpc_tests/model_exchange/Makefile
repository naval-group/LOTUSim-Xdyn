.PHONY: all clean

all: run clean

run: model_exchange.proto tutorial_01_falling_ball.yml
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

tutorial_01_falling_ball.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_01_falling_ball.yml /xdyn_demos/"

model_exchange.proto: ../../../interfaces/model_exchange.proto
	cp $< $@

clean:
	rm -f model_exchange.proto tutorial_01_falling_ball.yml
