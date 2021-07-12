% Remote models in xdyn
% Charles-Edouard CADY
% July 13, 2021

# Pre-requisite: gRPC

## What is gRPC?

- Remote Procedure Call (RPC) allows calling functions from a
  different address space (or computer) as if they were local,
  without coding the details of the remote interaction
- gRPC is a RPC framework developped by Google & based on protocol buffers.
- It has pluggable support for load balancing, tracing, health checking and
  authentication.
- As it uses Protocol Buffers, it has a schema and encoders/decoders are
  generated for a variety of languages (Java, C++, Python...)

## Why are we using gRPC?

Using gRPC allows us to __extend xdyn in our language of choice__:

- We don't have to code everything in C++ anymore
- Some models can be kept private and still be used with xdyn
- We might want to run a given model in a very specific setting
  (hardware, software) for performance reasons
- gRPC supports all major platforms
- Using authentication, we can share models
- The gRPC interface forces us to specify the interface in a non-ambiguous way,
  which facilitates communication.


## How is gRPC used in xdyn?

gRPC is used to call remote models from xdyn:

- Force models
- Wave models
- Controllers

# Pre-requisite: `docker-compose`

## What is `docker-compose`?

> _"Compose is a tool for defining and running multi-container Docker
> applications. With Compose, you use a YAML file to configure your
> application’s services. Then, with a single command, you create and start all
> the services from your configuration."_ (Docker Compose documentation)


## Why are we using `docker-compose`

Although we could run each service "by hand", using `docker-compose` makes it
much easier: once our simulation is described it can be launched in a single
command and `docker-compose` provides facilities for logs, volumes, etc.

## How is `docker-compose` used in xdyn?

Any time we use remote models in xdyn we can use `docker-compose`.

`docker-compose` apps are described using a YAML file:

```yaml
version: '3'
services:
  force-model:
    image: sirehna/xdyn-force-grpc:python3
    command: harmonic_oscillator.py
    working_dir: /work
    volumes:
    - .:/work
  xdyn:
    image: sirehna/xdyn
    working_dir: /data
    entrypoint: xdyn model.yml --dt 0.1 --tend 0.2 -o out.csv
    volumes:
    - .:/data
    depends_on:
    - force-model
```

Launching the app is done with:

```
docker-compose up
```

# Remote force models

## Force interface

Three methods must be implemented for xdyn to be able to use a gRPC service:

```protobuf
service Force
{
    rpc set_parameters(SetForceParameterRequest)                  returns (SetForceParameterResponse);
    rpc force(ForceRequest)                                       returns (ForceResponse);
    rpc required_wave_information(RequiredWaveInformationRequest) returns (RequiredWaveInformationResponse);
}
```

Implementing the gRPC interface directly can be a hassle, which is why we
created the "interfaces" module
(https://gitlab.com/sirehna_naval_group/sirehna/interfaces). in this module,
there is a helper `force.py` module in Python: you just need to derive from the
`force.Model` class.


## Force interface - `set_parameters`

This method is called using the force model's intialization (before the first
time step). It is used to supply information both to xdyn and to the force model.

- Information given from xdyn to the force model:
  - Remote model's YAML inputs read from xdyn's input file
  - Name of the "body" frame
  - Instance name (if the same model is used everal times but with different parameters)
- Information given from the force model to xdyn:
  - Reference frame in which the forces and torques are expressed
  - Whether or not the force model needs to be supplied with wave data
  - If the model needs the state history, how far back should it go?
  - The commands needed by this model, so xdyn can check they are supplied

```python
def __init__(self, parameters, body_name):
    param = yaml.safe_load(parameters)
    self.c = param['c']
    self.body_name = body_name
```

## Force interface - `force`

- This method is where the actual force and torques are calculated.
- The force model can output extra observations (which can then be outputted
  by xdyn).

```python
def force(self, states, commands, __):
    k = commands['k']
    return {'Fx': -k*states.x[0] - self.c*states.u[0],
            'Fy': 0,
            'Fz': 0,
            'Mx': 0,
            'My': 0,
            'Mz': 0,
            'extra_observations': {'k': k, 'harmonic_oscillator_time': states.t[0]}}
```

## Example

```bash
git clone git@gitlab.com:CharlesEdouardCady_SIREHNA/xdyn-remote-models-examples.git
```

# Remote wave models

## Interface

```protobuf
service Waves
{
    rpc set_parameters(SetParameterRequest)                    returns (SetParameterResponse);
    rpc elevations(XYTGrid)                                    returns (XYZTGrid);
    rpc dynamic_pressures(XYZTGrid)                            returns (DynamicPressuresResponse);
    rpc orbital_velocities(XYZTGrid)                           returns (OrbitalVelocitiesResponse);
    rpc spectrum(SpectrumRequest)                              returns (SpectrumResponse);
    rpc angular_frequencies_for_rao(AngularFrequenciesRequest) returns (AngularFrequenciesResponse);
    rpc directions_for_rao(DirectionsRequest)                  returns (DirectionsResponse);
}
```

## Example

```bash
git clone git@gitlab.com:CharlesEdouardCady_SIREHNA/xdyn-remote-models-examples.git
```

## Other wave models

Depending on what a given wave model implements, some force models may not be available.

|--------------------|-------------------------|
| Computed quantity  |  Force model using it   |
|--------------------|-------------------------|
| Wave elevations    | Non-linear hydrostatics |
| Dynamic pressure   | Froude-Krylov           |
| Orbital velocities | Rudder                  |
| Linear spectrum    | Diffraction             |
|--------------------|-------------------------|

For example, for non-linear wave models (e.g. Choppy, HOS...) the wave model
needs to be linearized to retrieve a linear spectrum. Other approaches might
be possible (uncharted territory!)

# Remote controllers

## Architecture

## Interface

## Example
