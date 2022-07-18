.PHONY: all clean run

DOCKER_IMAGE=xdyn
DOCKER_WORKING_DIRECTORY=/xdyn_demos
DOCKER_RUN:=docker run --rm \
    -u $(shell id -u):$(shell id -g) \
    -v $(shell pwd):${DOCKER_WORKING_DIRECTORY} \
    -w ${DOCKER_WORKING_DIRECTORY}
DOCKER_RUN_BASH:=$(DOCKER_RUN) \
    --entrypoint /bin/bash \
    $(DOCKER_IMAGE) -c
DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all: run clean

run: tutorial_01_falling_ball.yml
	@$(DOCKER_COMPOSE) up -t 0 --exit-code-from test --abort-on-container-exit --build

tutorial_01_falling_ball.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@$(DOCKER_RUN_BASH) "cp /usr/demos/$@ /xdyn_demos/"

clean:
	rm -f tutorial_01_falling_ball.yml
	@$(DOCKER_COMPOSE) down --remove-orphans
