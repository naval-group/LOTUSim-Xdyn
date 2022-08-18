"""Integration tests for gRPC wave models."""

import logging
import math

from xdyngrpc.cosimulation import CosimulationEuler, CosimulationRequestEuler

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format="%(asctime)s,%(msecs)d ["
    + SERVICE_NAME
    + "] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s",
    datefmt="%d-%m-%Y:%H:%M:%S",
)
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def test_should_get_phases():
    cosim = CosimulationEuler("xdyn:9002")

    dt = 2
    state = {
        "t": 0,
        "x": 0,
        "y": 8,
        "z": 12,
        "u": 1,
        "v": 0,
        "w": 0,
        "p": 0,
        "q": 1,
        "r": 0,
        "phi": 0,
        "theta": 0,
        "psi": 0,
    }
    requested_output = ["phase0(TestBody)"]
    results = cosim.step(state, dt, requested_output)
    assert "phase0(TestBody)" in results["extra_observations"]
    for phase0 in results["extra_observations"]["phase0(TestBody)"]:
        assert phase0 > 0
        assert phase0 < math.pi * 2


def main():
    test_should_get_phases()


if __name__ == "__main__":
    main()
