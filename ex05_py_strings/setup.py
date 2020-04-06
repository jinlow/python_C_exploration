from distutils.core import setup, Extension

std_module = Extension(
    "stringC", sources=["src/concat_str.cpp"], extra_compile_args=["-std=c++11"]
)

setup(
    name="pystringC",
    version="0.1",
    description="Working with strings with C.",
    ext_modules=[std_module],
)
