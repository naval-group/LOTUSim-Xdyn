.PHONY: all clean

DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all:
	@$(DOCKER_COMPOSE) \
		up \
		-t 0 \
        --exit-code-from test \
        --abort-on-container-exit \
        --build \
        --remove-orphans

clean:
	@$(DOCKER_COMPOSE) down --remove-orphans
