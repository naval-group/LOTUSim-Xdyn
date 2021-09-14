.PHONY: all clean invalid_yaml_returns_non_zero_error_code tutorial_12 no_stl

all: invalid_yaml_returns_non_zero_error_code \
	 tutorial_12

DOCKER_RUN=docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work
WRONG_ERROR_CODE=echo "Oops, didn't get the error code we were expecting: test failed!" && return 1
EXPECTED_ERROR_CODE=echo "Got the error code we expected for an invalid YAML: success!"

invalid_yaml_returns_non_zero_error_code: tutorial_01_falling_ball.yml cube.stl
	@echo "Corrupting $<..."
	@sed -i 's/rotations/rrrotations/g' tutorial_01_falling_ball.yml
	@echo "Testing if this invalid YAML triggers a non-zero return code of xdyn..."
	@${DOCKER_RUN} xdyn --dt 1 --tend 1 tutorial_01_falling_ball.yml && ${WRONG_ERROR_CODE} || ${EXPECTED_ERROR_CODE}
	@${DOCKER_RUN} --entrypoint=xdyn-for-cs xdyn --dt 1 -p 9002 tutorial_01_falling_ball.yml && ${WRONG_ERROR_CODE} || ${EXPECTED_ERROR_CODE}
	@${DOCKER_RUN} --entrypoint=xdyn-for-me xdyn -p 9002 tutorial_01_falling_ball.yml && ${WRONG_ERROR_CODE} || ${EXPECTED_ERROR_CODE}
	@${DOCKER_RUN} --entrypoint=gz xdyn --dphi=1 --phi_max=1 -s cube.stl tutorial_01_falling_ball.yml && ${WRONG_ERROR_CODE} || ${EXPECTED_ERROR_CODE}

tutorial_01_falling_ball.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@${DOCKER_RUN} -v $(shell pwd):/xdyn_demos --entrypoint /bin/bash xdyn -c "cp /usr/demos/$@ /xdyn_demos/"

tutorial_12:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@${DOCKER_RUN} -v $(shell pwd):/xdyn_demos --entrypoint /bin/bash xdyn -c "cp /usr/demos/tutorial_12_precal_r.yml /usr/demos/ONRT_SIMMAN.raodb.ini /usr/demos/test_ship.stl /xdyn_demos/"
	@echo "Launching tutorial 12..."
	@${DOCKER_RUN} xdyn --dt 1 --tend 1 -o tsv tutorial_12_precal_r.yml
	@echo "All good, cleaning up!"
	@rm -rf ONRT_SIMMAN.raodb.ini tutorial_12_precal_r.yml test_ship.stl


cube.stl:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@${DOCKER_RUN} -v $(shell pwd):/xdyn_demos --entrypoint /bin/bash xdyn -c "cp /usr/demos/$@ /xdyn_demos/"

no_stl: test.h5
	@echo "Testing if STL file is present in HDF5 file (it shouldn't)"
	@${DOCKER_RUN} hdfgroup/hdf5lib:1.10.6 h5dump -d /inputs/meshes/TestShip/faces -c 1 test.h5; echo $$? >> .commands.result
	@if grep -q 0 .commands.result; then \
	echo "Found STL mesh in HDF5 file, despite the flag: test failed!" && exit 1; \
	else \
	echo "Did not find STL mesh in HDF5, as expected: test passed."; \
	fi
	@rm -f .commands.result

test.h5: tutorial_05_froude_krylov.yml test_ship.stl
	@${DOCKER_RUN} xdyn tutorial_05_froude_krylov.yml --dt 0.1 --tend 0.1 -o test.h5

test_ship.stl tutorial_05_froude_krylov.yml:
	@echo "Copying $@ from xdyn's Docker container..."
	@${DOCKER_RUN} -v $(shell pwd):/xdyn_demos --entrypoint /bin/bash xdyn -c "cp /usr/demos/$@ /xdyn_demos/"


clean:
	rm -rf tutorial_01_falling_ball.yml cube.stl
