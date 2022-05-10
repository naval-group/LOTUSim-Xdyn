.PHONY: all clean run

DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all: run clean

run: tutorial_09_gRPC_wave_model.yml test_ship.hdb
	@$(DOCKER_COMPOSE) up -t 0 --exit-code-from test --abort-on-container-exit --build

tutorial_09_gRPC_wave_model.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
		-c "cp /usr/demos/tutorial_09_gRPC_wave_model.yml /xdyn_demos/" 

test_ship.hdb:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
		-c "cp /usr/demos/test_ship.hdb /xdyn_demos/" 


clean:
	rm -f tutorial_09_gRPC_wave_model.yml test_ship.hdb
	@$(DOCKER_COMPOSE) down --remove-orphans
