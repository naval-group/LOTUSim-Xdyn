# This Makefile:
# - builds the xdyn wheel from xdyn python shared library
# - tests the generated wheel in a fresh docker container

DIR := $(notdir $(shell pwd))
PYTHON=python3
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)
DOCKER_RUN_BUILD_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_BUILD) /bin/bash -c
DOCKER_RUN_TEST_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_TEST) /bin/bash -c

# Display the name and comment of each target followed by ' ##'
help: ## Display help and exit.
	@echo 'usage: make [target]'
	@echo
	@echo 'targets:'
	@egrep '^(.+)\:\ ##\ (.+)' ${MAKEFILE_LIST} | column -t -c 2 -s ':#'
	@echo
	@echo "Run higher target to build wheel! It can not be built directly from this Makefile"

debian_11_py309_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py309-build
debian_11_py309_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py309-test
debian_11_py309_package: package

debian_11_py307_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py307-build
debian_11_py307_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py307-test
debian_11_py307_package: package

debian_11_py308_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py308-build
debian_11_py308_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py308-test
debian_11_py308_package: package

debian_11_py310_package: DOCKER_IMAGE_BUILD=xdyn-python-deb11-py310-build
debian_11_py310_package: DOCKER_IMAGE_TEST=xdyn-python-deb11-py310-test
debian_11_py310_package: package

ubuntu2004_py308_package: DOCKER_IMAGE_TEST=xdyn-python-ubuntu2004-py308-test
ubuntu2004_py308_package: test_only

test_additional_platforms_depending_on_debian: ubuntu2004_py308_package

doc: ## Create documentation with Sphinx
doc: debian_11_py309_package
	@make -C doc

test_only: ## Install an existing wheel and run tests
	make ${DOCKER_IMAGE_TEST}
	make DOCKER_IMAGE_TEST=${DOCKER_IMAGE_TEST} package_test_run

package: ## Create the wheel package
	make ${DOCKER_IMAGE_BUILD}
	make DOCKER_IMAGE_BUILD=${DOCKER_IMAGE_BUILD} package_build_wheel
	make ${DOCKER_IMAGE_TEST}
	make DOCKER_IMAGE_TEST=${DOCKER_IMAGE_TEST} package_test_run

format: ## Use isort and black to format code
	@make xdyn-python-deb11-py309-format-lint
	$(DOCKER_AS_USER) xdyn-python-deb11-py309-format-lint /bin/bash -c \
		"isort --profile black unit_tests && \
		 black --line-length 100 unit_tests"

lint: ## Check Python unit tests code with isort, black, and pylint
	@make xdyn-python-deb11-py309-format-lint
	$(DOCKER_AS_USER) xdyn-python-deb11-py309-format-lint /bin/bash -c \
		"isort --profile black --check unit_tests && \
		 black --line-length 100 --check unit_tests && \
		 pylint --max-line-length=100 unit_tests"

package_build_wheel: ## Create xdyn wheel package
	$(eval GIT_VERSION := $(shell $(DOCKER_AS_USER) -v $(shell pwd)/../..:/all $(DOCKER_IMAGE_BUILD) \
		/bin/bash -c "cd /all && git describe --abbrev=0 --tag --always | cut -f2 -dv"))
	@echo "GIT_VERSION = ${GIT_VERSION}"
	$(DOCKER_RUN_BUILD_BASH) \
	    "GIT_VERSION=\"${GIT_VERSION}\" ${PYTHON} setup.py bdist_wheel &&\
		mv dist/*.whl ."

package_test_run: ## Test the xdyn package with a docker image containing only required dependencies
	$(DOCKER_RUN_TEST_BASH) "${PYTHON} -m unittest discover -s unit_tests"

package_test_run_for_a_specific_file:
	$(DOCKER_RUN_TEST_BASH) \
		"cd unit_tests/exe && ${PYTHON} test_xdyn.py"

clean: ## Removed all generated files
	@rm -rf build
	@rm -rf core
	@rm -rf dist
	@rm -rf *.egg-info
	@rm -rf *.whl
	@rm -f *.coverage
	@rm -f nosetests.xml
	@rm -rf unit_tests/__pycache__
	@rm -f unit_tests/*.coverage
	@rm -f unit_tests/core
	@rm -f unit_tests/nosetests.xml
	@find . -type d -name __pycache__ -exec rm -rf {} \; || true

.PHONY=xdyn-python-deb11-build
xdyn-python-deb11-build:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=9 \
		--build-arg PYTHON_PATCH_VERSION=9 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py307-build
xdyn-python-deb11-py307-build:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=7 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py307-test
xdyn-python-deb11-py307-test:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=7 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_test \
		-t $@ .

.PHONY=xdyn-python-deb11-py308-build
xdyn-python-deb11-py308-build:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=8 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py308-test
xdyn-python-deb11-py308-test:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=8 \
		--build-arg PYTHON_PATCH_VERSION=12 \
		-f Dockerfile_deb11_py3yz_test \
		-t $@ .

.PHONY=xdyn-python-deb11-py309-build
xdyn-python-deb11-py309-build:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=9 \
		--build-arg PYTHON_PATCH_VERSION=9 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py309-test
xdyn-python-deb11-py309-test:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=9 \
		--build-arg PYTHON_PATCH_VERSION=9 \
		-f Dockerfile_deb11_py3yz_test \
		-t $@ .

.PHONY=xdyn-python-deb11-py310-build
xdyn-python-deb11-py310-build:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=10 \
		--build-arg PYTHON_PATCH_VERSION=2 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py310-test
xdyn-python-deb11-py310-test:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=10 \
		--build-arg PYTHON_PATCH_VERSION=2 \
		-f Dockerfile_deb11_py3yz_test \
		-t $@ .

.PHONY=xdyn-python-deb11-py309-format-lint
xdyn-python-deb11-py309-format-lint:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=9 \
		--build-arg PYTHON_PATCH_VERSION=9 \
		-f Dockerfile_deb11_py3yz_format_lint \
		-t $@ .

.PHONY=xdyn-python-ubuntu2004-py308-test
xdyn-python-ubuntu2004-py308-test:
	docker build \
		-f Dockerfile_ubuntu2004_py3_test \
		-t $@ .
