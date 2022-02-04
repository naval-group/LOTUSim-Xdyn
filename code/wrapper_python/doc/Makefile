DOCKER_IMAGE_BUILD=xdyn-python-sphinx
DIR := $(notdir $(shell pwd))
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)
DOCKER_RUN_BUILD_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_BUILD) /bin/bash -c

all: \
	build_docker_images \
	init \
	build

build_docker_images:
	cp ../xdyn-*-cp39-cp39-linux_x86_64.whl .
	docker build . -t $(DOCKER_IMAGE_BUILD)

init:
	@$(eval GIT_VERSION := $(shell $(DOCKER_AS_USER) -v $(shell pwd)/../../..:/all $(DOCKER_IMAGE_BUILD) \
		/bin/bash -c "cd /all && git describe --abbrev=0 --tag --always | cut -f2 -dv"))
	@echo "GIT_VERSION = ${GIT_VERSION}"
	rm -rf src
	mkdir -p src
	${DOCKER_RUN_BUILD_BASH} "sphinx-quickstart \
		--quiet \
		--project xdyn \
		--author Sirehna \
		-v ${GIT_VERSION} \
		-r ${GIT_VERSION} \
		-l en \
		--sep \
		--use-make-mode \
		--ext-autodoc \
		--ext-doctest \
		--ext-intersphinx \
		--ext-todo \
		--extensions=sphinx.ext.autosummary \
		--extensions=sphinx_autopackagesummary \
		--extensions=myst_parser \
		src"
	@echo "" >> src/source/conf.py
	@echo "autosummary_generate = True" >> src/source/conf.py
	@echo "autoclass_content = \"class\"" >> src/source/conf.py
	cp index.rst api.rst xdyn.rst src/source/.
	mkdir -p src/source/_templates/autosummary
	cp autosummary/* src/source/_templates/autosummary/.

build:
	${DOCKER_RUN_BUILD_BASH} "make -C src html"


clean:
	@rm -rf xdyn-*.whl
	@rm -rf src
