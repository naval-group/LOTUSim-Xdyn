.PHONY: all clean invalid_yaml_returns_non_zero_error_code tutorial_12

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
	@echo "Launching tutorial..."
	@${DOCKER_RUN} xdyn --dt 1 --tend 1 -o tsv tutorial_12_precal_r.yml
	@echo "All good, cleaning up!"
	@rm -rf ONRT_SIMMAN.raodb.ini tutorial_12_precal_r.yml test_ship.stl


cube.stl:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@${DOCKER_RUN} -v $(shell pwd):/xdyn_demos --entrypoint /bin/bash xdyn -c "cp /usr/demos/$@ /xdyn_demos/"


clean:
	rm -rf tutorial_01_falling_ball.yml cube.stl
