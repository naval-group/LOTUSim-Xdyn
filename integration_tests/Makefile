.PHONY: all clean invalid_yaml_returns_non_zero_error_code

all: invalid_yaml_returns_non_zero_error_code

invalid_yaml_returns_non_zero_error_code: tutorial_01_falling_ball.yml cube.stl
	@echo "Corrupting $<..."
	@sed -i 's/rotations/rrrotations/g' tutorial_01_falling_ball.yml
	@echo "Testing if this invalid YAML triggers a non-zero return code of xdyn..."
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work xdyn --dt 1 --tend 1 tutorial_01_falling_ball.yml && echo "Oops, didn't get the error code we were expecting: test failed!" && return 1 || echo "Got the error code we expected for an invalid YAML: success!"
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work --entrypoint=xdyn-for-cs xdyn --dt 1 -p 9002 tutorial_01_falling_ball.yml && echo "Oops, didn't get the error code we were expecting: test failed!" && return 1 || echo "Got the error code we expected for an invalid YAML: success!"
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work --entrypoint=xdyn-for-me xdyn -p 9002 tutorial_01_falling_ball.yml && echo "Oops, didn't get the error code we were expecting: test failed!" && return 1 || echo "Got the error code we expected for an invalid YAML: success!"
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work --entrypoint=gz xdyn --dphi=1 --phi_max=1 -s cube.stl tutorial_01_falling_ball.yml && echo "Oops, didn't get the error code we were expecting: test failed!" && return 1 || echo "Got the error code we expected for an invalid YAML: success!"
	@rm tutorial_01_falling_ball.yml cube.stl

tutorial_01_falling_ball.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/$@ /xdyn_demos/"

cube.stl:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/$@ /xdyn_demos/"

clean:
	rm -rf tutorial_01_falling_ball.yml cube.stl
