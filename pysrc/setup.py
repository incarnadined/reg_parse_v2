from glob import glob
from setuptools import setup, Extension
import pybind11
from pybind11.setup_helpers import Pybind11Extension
    
files = sorted(glob("../../src/*.cpp")) # Sort source files for reproducibility
files.remove('../../src\\main.cpp') # Remove main.cpp (not part of module)

ext_modules = [
    Pybind11Extension(
        "reg_parse_v2",
        files,  
    ),
]

setup(
    name='reg_parse_v2',
    ext_modules=ext_modules
)

# python setup.py build_ext --inplace