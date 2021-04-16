.PHONY: all

all:
	cp ../code/interfaces/force.proto .
	cp ../code/interfaces/wave_grpc.proto .
	cp ../code/interfaces/wave_types.proto .
	docker build . -t xdyn-force-grpc:python3
	rm -f force.proto wave_grpc.proto wave_types.proto
