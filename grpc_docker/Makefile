.PHONY: all

all:
	cp ../interfaces/force.proto .
	cp ../interfaces/wave_grpc.proto .
	cp ../interfaces/wave_types.proto .
	docker build . -t xdyn-force-grpc:python3
	rm -f force.proto wave_grpc.proto wave_types.proto
