# This setup.py creates a wheel file from the Python XDyn library built with CMake
import os
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    pass


setup(
    name="pyxdyn",
    version="0.0.1",
    author="Sirehna",
    author_email="sirehna@sirehna.com",
    description="A Python wrapper for XDyn",
    long_description="",
    ext_modules=[CMakeExtension("pyxdyn")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={"test": ["pytest>=6.0"]},
    python_requires=">=3.6",
)
