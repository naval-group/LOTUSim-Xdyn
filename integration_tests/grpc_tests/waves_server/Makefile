.PHONY: all clean run

DASH:=-
UNDERSCORE:=_
SLUG:=$(subst $(DASH),$(UNDERSCORE),$(CI_PROJECT_PATH_SLUG))
COMPOSE_PROJECT_NAME:=$(shell basename `pwd`)${SLUG}${CI_PIPELINE_ID}
DOCKER_COMPOSE:=COMPOSE_PROJECT_NAME=$(COMPOSE_PROJECT_NAME) CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all:
	make -C client
	make test

test:
	@${DOCKER_COMPOSE} up \
		-t 0 \
		--exit-code-from client \
		--abort-on-container-exit \
		--build \
		--remove-orphans

clean:
	@make --silent -C client clean
	@$(DOCKER_COMPOSE) down --remove-orphans
