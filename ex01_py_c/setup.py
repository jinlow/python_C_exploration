from distutils.core import setup, Extension

std_module = Extension('std', sources=['src/std.c'])

setup(name='std_test',
      version='0.1',
      description='Module for calculating standard deviation.',
      ext_modules=[std_module])