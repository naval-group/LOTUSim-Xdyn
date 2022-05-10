.PHONY: all clean

DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all: run clean

run: model_exchange.proto tutorial_01_falling_ball.yml
	@$(DOCKER_COMPOSE) up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

tutorial_01_falling_ball.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_01_falling_ball.yml /xdyn_demos/"

model_exchange.proto: ../../../interfaces/model_exchange.proto
	cp $< $@

clean:
	rm -f model_exchange.proto tutorial_01_falling_ball.yml
	@$(DOCKER_COMPOSE) down --remove-orphans
