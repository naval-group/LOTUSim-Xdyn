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
    state = {"Dt": 2,
             "states": [{"t": 0, "x": 0, "y": 8, "z": 12, "u": 1, "v": 0,
                         "w": 0, "p": 0, "q": 1, "r": 0, "qr": 1, "qi": 0,
                         "qj": 0, "qk": 0}]}

    res = run(state)
    assert len(res['t']) == 21
    assert len(res['x']) == 21
    assert len(res['y']) == 21
    assert len(res['z']) == 21
    assert len(res['u']) == 21
    assert len(res['v']) == 21
    assert len(res['w']) == 21
    assert len(res['p']) == 21
    assert len(res['q']) == 21
    assert len(res['r']) == 21
    assert len(res['qr']) == 21
    assert len(res['qi']) == 21
    assert len(res['qj']) == 21
    assert len(res['qk']) == 21
    assert len(res['phi']) == 21
    assert len(res['theta']) == 21
    assert len(res['psi']) == 21
