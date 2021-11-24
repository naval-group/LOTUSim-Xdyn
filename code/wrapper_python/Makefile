# This Makefile:
# - builds the xdyn wheel from xdyn python shared library
# - tests the generated wheel in a fresh docker container

DOCKER_IMAGE_BUILD=xdyn-python-build
DOCKER_IMAGE_TEST=xdyn-python-test
DIR := $(notdir $(shell pwd))
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)
DOCKER_RUN_BUILD_BASH:= $(DOCKER_AS_USER) $(DOCKER_IMAGE_BUILD) /bin/bash -c
DOCKER_RUN_TEST_BASH:= $(DOCKER_AS_USER) $(DOCKER_IMAGE_TEST) /bin/bash -c

help:
	echo "Run higher target to build wheel! It can not be built directly from this Makefile"

build_docker_build_container:
	docker build . -f Dockerfile_pybind -t ${DOCKER_IMAGE_BUILD}

demo_package: demo_package_build demo_package_test_build demo_package_test_run

demo_package_build:
	$(DOCKER_RUN_BUILD_BASH) \
	    "python3 setup.py bdist_wheel &&\
		mv dist/*.whl ."

demo_package_test_build:
	docker build . -f Dockerfile_test -t $(DOCKER_IMAGE_TEST)

demo_package_test_run:
	$(DOCKER_RUN_TEST_BASH) \
		"cd unit_tests && python3 -m nose --all-modules --with-xunit --traverse-namespace"

demo_package_test_run_for_a_specific_file:
	$(DOCKER_RUN_TEST_BASH) \
		"cd unit_tests && python3 test_wageningencontrolledforcemodel.py"

clean:
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
	@find . -type d -name __pycache__ -exec rm -rf {} \;
