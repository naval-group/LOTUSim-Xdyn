"""Integration tests for gRPC wave models."""

import logging

from websocket import create_connection

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def run(state):
    """Run a cosimulation step."""
    import json
    ws = create_connection("ws://xdyn:9002")
    ws.send(json.dumps(state))
    result = ws.recv()
    ws.close()
    return json.loads(result)

def test_should_get_the_right_number_of_results_from_xdyn_in_cosim_mode():
    state = {"Dt": 0.1,
             "states": [{"t": 0, "x": 0, "y": 8, "z": 12, "u": 1, "v": 0,
                         "w": 0, "p": 0, "q": 1, "r": 0, "qr": 1, "qi": 0,
                         "qj": 0, "qk": 0}]}

    res = run(state)
    assert len(res['t']) == 2
    assert len(res['x']) == 2
    assert len(res['y']) == 2
    assert len(res['z']) == 2
    assert len(res['u']) == 2
    assert len(res['v']) == 2
    assert len(res['w']) == 2
    assert len(res['p']) == 2
    assert len(res['q']) == 2
    assert len(res['r']) == 2
    assert len(res['qr']) == 2
    assert len(res['qi']) == 2
    assert len(res['qj']) == 2
    assert len(res['qk']) == 2
    assert len(res['phi']) == 2
    assert len(res['theta']) == 2
    assert len(res['psi']) == 2
