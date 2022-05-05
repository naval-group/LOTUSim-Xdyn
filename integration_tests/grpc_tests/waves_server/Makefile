.PHONY: all clean run

DOCKER_COMPOSE:=CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose

all:
	make -C client
	make test

test:
	@${DOCKER_COMPOSE} rm -f
	@${DOCKER_COMPOSE} up \
		-t 0 \
		--exit-code-from client \
		--abort-on-container-exit \
		--build \
		--remove-orphans

clean:
	@make --silent -C client clean
