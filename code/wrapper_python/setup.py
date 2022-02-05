# This setup.py creates a wheel file from the Python XDyn library built with CMake
import os
import re

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    pass


version = os.environ.get("GIT_VERSION", "0.0.0")
if not re.match(r"[0-9]+\.[0-9]+\.*[0-9]*", version):
    print(f"GIT version is invalid: {version}")
    print("Setting version to 0.0.0 for wheel generation")
    version = "0.0.0"


setup(
    name="xdyn",
    version=version,
    author="Sirehna",
    author_email="sirehna@sirehna.com",
    description="A Python wrapper for XDyn",
    long_description="",
    ext_modules=[CMakeExtension("xdyn")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={"test": ["pytest>=6.0"]},
    python_requires=">=3.6",
)
