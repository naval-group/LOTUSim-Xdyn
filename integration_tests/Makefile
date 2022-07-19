.PHONY: all clean

all:
	@echo "Building pre-requisite images..."
	@make -C ../interfaces build
	@make -C xdyn
	@make -C grpc_tests
	@make -C json_tests

clean:
	@make -C grpc_tests $@
	@make -C json_tests $@
	@make -C xdyn $@
