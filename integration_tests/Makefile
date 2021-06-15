.PHONY: all clean test

all: invalid_yaml_returns_non_zero_error_code

invalid_yaml_returns_non_zero_error_code: tutorial_01_falling_ball.yml
	@echo "Corrupting $<..."
	@sed -i 's/rotations/rrrotations/g' tutorial_01_falling_ball.yml
	@docker run -t --rm -u $(shell id -u):$(shell id -g) -v $(shell pwd):/work -w /work xdyn --dt 1 --tend 1 tutorial_01_falling_ball.yml && return 1 || echo "Success!"
	#@rm tutorial_01_falling_ball.yml
	@echo "All done!"

tutorial_01_falling_ball.yml:
	@echo "Copying tutorial files from xdyn's Docker container..."
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/$@ /xdyn_demos/"

clean:
	rm -rf tutorial_01_falling_ball.yml
