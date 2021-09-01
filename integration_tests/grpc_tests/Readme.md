This directory contains gRPC demos that run with xdyn.
These demos show how users can define their own models that
can be called by xdyn with gRPC.

They are based on docker-compose to create and organize
gRPC communications between services.

Each demo contains at least two services:

- one that runs xdyn
- a second one that runs the user-implemented service

Directory `force00` illustrates the use of docker image
`sirehna/xdyn-grpc-force:python-beta-2021-07-09`.
It is presented in detail in tutorial 10.

Directory `waves00` illustrates the use of docker image
`sirehna/xdyn-grpc-wave:python-beta-2021-07-09`.
It is presented in detail in tutorial 09.
