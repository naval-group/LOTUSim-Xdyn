.PHONY: all clean issue-204 test

DOCKER_RUN:=docker run --rm -u $(shell id -u ):$(shell id -g)
DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all: test issue-204 clean

test: test.py tutorial_11.csv
	@echo "Checking heading converged for both setpoints..."
	@$(DOCKER_RUN) -v $(shell pwd):/work -w /work --entrypoint /usr/bin/python3 python:3.7 test.py
	@echo "All done!"


issue-204: ## When using gRPC interfaces with xdyn, the first state time is not set correctly according to xdyn --tstart option
	@$(DOCKER_COMPOSE) -f docker-compose-issue-204.yml up -t 0 --exit-code-from test --abort-on-container-exit --build --remove-orphans

tutorial_11_gRPC_controller.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@$(DOCKER_RUN) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_11_gRPC_controller.yml /xdyn_demos/"

tutorial_11.csv: tutorial_11_gRPC_controller.yml
	@echo "Running simulation..."
	@$(DOCKER_COMPOSE) up -t 0 --exit-code-from xdyn --abort-on-container-exit --build --remove-orphans
	rm tutorial_11_gRPC_controller.yml

clean:
	rm -rf tutorial_11.csv tutorial_11_gRPC_controller.yml
	@$(DOCKER_COMPOSE) down --remove-orphans
	@$(DOCKER_COMPOSE) -f docker-compose-issue-204.yml down --remove-orphans
