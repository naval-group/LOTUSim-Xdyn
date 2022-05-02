"""Script to check size of xdyn output result files.

Get started with

```bash
python check_output_size.py --help
python check_output_size.py csv my_xdyn_result_file.csv
python check_output_size.py hdf5 my_xdyn_result_file.hdf5
```
"""
import argparse
import json
from typing import List

import h5py
import numpy as np

_EXPECTED_TIME_VECTOR = np.arange(0, 1.1, 0.1)
_EXPECTED_NUMBER_TIME_STEPS = len(_EXPECTED_TIME_VECTOR)


def _message_size(format: str, time_vector: np.ndarray) -> str:
    length_time_vector = len(time_vector)
    msg = f"Time vector contains {length_time_vector} elements, "
    msg += f"instead of {_EXPECTED_NUMBER_TIME_STEPS}, "
    msg += f"for {format} export: t={time_vector}"
    return msg


def _message_vector(format: str, time_vector: np.ndarray) -> str:
    return f"Error checking size for {format}: {time_vector}"


def check_time_vector_size_in_output_hdf5(filename: str = "res.hdf5"):
    res = h5py.File(filename, "r")
    time_vector = res["outputs"]["t"]
    length_time_vector = len(time_vector)
    assert length_time_vector == _EXPECTED_NUMBER_TIME_STEPS, _message_size("HDF5", time_vector)
    assert np.allclose(_EXPECTED_TIME_VECTOR, time_vector), _message_vector("HDF5", time_vector)


def check_time_vector_size_in_output_csv(filename: str = "res.csv"):
    res = np.genfromtxt(filename, names=True, delimiter=",")
    time_vector = res["t"]
    length_time_vector = len(time_vector)
    assert length_time_vector == _EXPECTED_NUMBER_TIME_STEPS, _message_size("CSV", time_vector)
    assert np.allclose(_EXPECTED_TIME_VECTOR, time_vector), _message_vector("CSV", time_vector)


def check_time_vector_size_in_output_tsv(filename: str = "res.tsv"):
    res = np.genfromtxt(filename, names=True, delimiter="\t")
    time_vector = res["t"]
    length_time_vector = len(time_vector)
    assert length_time_vector == _EXPECTED_NUMBER_TIME_STEPS, _message_size("TSV", time_vector)
    assert np.allclose(_EXPECTED_TIME_VECTOR, time_vector), _message_vector("TSV", time_vector)


def check_time_vector_size_in_output_json(filename: str = "res.json"):
    lines = open(filename, "r").readlines()
    data = [json.loads(line) for line in lines]
    time_vector = [d["t"] for d in data]
    length_time_vector = len(time_vector)
    assert length_time_vector == _EXPECTED_NUMBER_TIME_STEPS, _message_size("JSON", time_vector)
    assert np.allclose(_EXPECTED_TIME_VECTOR, time_vector), _message_vector("JSON", time_vector)


def create_parser() -> argparse.ArgumentParser:
    """
    Create the parser
    """
    parser = argparse.ArgumentParser(
        description="Main entry point to check output size wrt format\n",
        add_help=True,
    )
    parser.add_argument("format", choices=["csv", "hdf5", "json", "tsv"], help="Result format")
    parser.add_argument("filename", type=str, help="Filename to check")
    return parser


def main(cli: List[str] = None) -> None:
    """
    Main entry point used when calling from command line interface
    """
    parser = create_parser()
    args = parser.parse_args(cli)
    format2checker = {
        "csv": check_time_vector_size_in_output_csv,
        "tsv": check_time_vector_size_in_output_tsv,
        "json": check_time_vector_size_in_output_json,
        "hdf5": check_time_vector_size_in_output_hdf5,
    }
    format2checker[args.format](args.filename)


if __name__ == "__main__":  # pragma: no cover
    main()
