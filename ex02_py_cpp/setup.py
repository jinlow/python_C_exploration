from distutils.core import setup, Extension

std_module = Extension('sumC', sources=['src/sumC.cpp'],
                       extra_compile_args=['-std=c++11'])

setup(name='pysumC',
      version='0.1',
      description='Python module for creating basic summary stats using C.',
      ext_modules=[std_module])