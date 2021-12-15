# This Makefile:
# - builds the xdyn wheel from xdyn python shared library
# - tests the generated wheel in a fresh docker container

help:
	echo "Run higher target to build wheel! It can not be built directly from this Makefile"

debian_10_package: DOCKER_IMAGE_BUILD=xdyn-python-deb10-build
debian_10_package: DOCKER_IMAGE_TEST=xdyn-python-deb10-test
debian_10_package: package

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

DIR := $(notdir $(shell pwd))
PYTHON=python3
DOCKER_AS_ROOT:=docker run -t --rm -w /opt/share/${DIR} -v $(shell pwd)/..:/opt/share
DOCKER_AS_USER:=$(DOCKER_AS_ROOT) -u $(shell id -u):$(shell id -g)
DOCKER_RUN_BUILD_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_BUILD) /bin/bash -c
DOCKER_RUN_TEST_BASH=$(DOCKER_AS_USER) $(DOCKER_IMAGE_TEST) /bin/bash -c

test_only:
	make ${DOCKER_IMAGE_TEST}
	make DOCKER_IMAGE_TEST=${DOCKER_IMAGE_TEST} package_test_run

package:
	make ${DOCKER_IMAGE_BUILD}
	make DOCKER_IMAGE_BUILD=${DOCKER_IMAGE_BUILD} package_build_wheel
	make ${DOCKER_IMAGE_TEST}
	make DOCKER_IMAGE_TEST=${DOCKER_IMAGE_TEST} package_test_run

package_build_wheel:
	$(eval GIT_VERSION := $(shell $(DOCKER_AS_USER) -v $(shell pwd)/../..:/all $(DOCKER_IMAGE_BUILD) \
		/bin/bash -c "cd /all && git describe --abbrev=0 --tag --always | cut -f2 -dv"))
	@echo "GIT_VERSION = ${GIT_VERSION}"
	$(DOCKER_RUN_BUILD_BASH) \
	    "GIT_VERSION=\"${GIT_VERSION}\" ${PYTHON} setup.py bdist_wheel &&\
		mv dist/*.whl ."

package_test_run:
	$(DOCKER_RUN_TEST_BASH) "${PYTHON} -m unit_tests"

package_test_run_for_a_specific_file:
	$(DOCKER_RUN_TEST_BASH) \
		"cd unit_tests/exe && ${PYTHON} test_xdyn.py"

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
	@find . -type d -name __pycache__ -exec rm -rf {} \; || true


.PHONY=xdyn-python-deb10-build
xdyn-python-deb10-build: Dockerfile_deb10_pybind
	docker build . -f $^ -t $@

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
		--build-arg PYTHON_PATCH_VERSION=1 \
		-f Dockerfile_deb11_py3yz_pybind \
		-t $@ .

.PHONY=xdyn-python-deb11-py310-test
xdyn-python-deb11-py310-test:
	docker build \
		--build-arg PYTHON_MAJOR_VERSION=3 \
		--build-arg PYTHON_MINOR_VERSION=10 \
		--build-arg PYTHON_PATCH_VERSION=1 \
		-f Dockerfile_deb11_py3yz_test \
		-t $@ .


.PHONY=xdyn-python-ubuntu2004-py308-test
xdyn-python-ubuntu2004-py308-test:
	docker build \
		-f Dockerfile_ubuntu2004_py3_test \
		-t $@ .
