.PHONY: all

DOCKER_CHANGELOG:=docker run -it --rm -v $(shell pwd)/../:/work -w /work changelog

all:
	@docker build -t changelog .
	@rm -f CHANGELOG.md
	@${DOCKER_CHANGELOG} v6.0.3 v6.1.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v6.0.2 v6.0.3 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v6.0.1 v6.0.2 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v6.0.0 v6.0.1 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v5.1.0 v6.0.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v5.0.1 v5.1.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v5.0.0 v5.0.1 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.3.0 v5.0.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.2.0 v4.3.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.1.0 v4.2.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.0.4 v4.1.0 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.0.3 v4.0.4 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.0.2 v4.0.3 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.0.1 v4.0.2 >> CHANGELOG.md
	@${DOCKER_CHANGELOG} v4.0.0 v4.0.1 >> CHANGELOG.md

# The following tags won't work as they predate the use of Gitlab issues & merge requests
#@${DOCKER_CHANGELOG} v3.1.1 v4.0.0 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v3.1.0 v3.1.1 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v3.0.3 v3.1.0 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v3.0.2 v3.0.3 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v3.0.1 v3.0.2 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v3.0.0 v3.0.1 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v2.0.0 v3.0.0 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} v1.0.0 v1.0.0 >> CHANGELOG.md
#@${DOCKER_CHANGELOG} 8fac6c21 v1.0.0 >> CHANGELOG.md

clean:
	@rm -f CHANGELOG.md
