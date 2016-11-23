from distutils.core import setup
from Cython.Build import cythonize

setup(
          name = 'Calculator',
          ext_modules = cythonize("calculator.pyx"),
)

