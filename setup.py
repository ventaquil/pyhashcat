# Author: Rich Kelley
# License: MIT

# Build Extention: python setup.py build_ext -R /usr/local/lib
# Debug Build:     CFLAGS='-Wall -O0 -g' python setup.py build_ext -R /usr/local/lib
# NOTE: hashcat makefile will need to be changed to include -g -O0 switches if debugging is needed

import os
import sys

from setuptools import setup, Extension

hc_lib_path = "/usr/local/lib"
hc_lib = "libhashcat.so"
try:
    files = os.listdir(hc_lib_path)
    for file_ in files:
        if "hashcat" in file_:
            hc_lib = ":%s" % (file_)
except Exception as e:
    print("Unable to find libhashcat.so in %s." % (hc_lib_path))
    sys.exit(1)

pyhashcat_module = Extension(
    "pyhashcat",
    include_dirs=[
        "include",
        # "hashcat/include",
        # "hashcat/deps/OpenCL-Headers",
        "hashcat/deps/LZMA-SDK/C",
        # "hashcat/deps/zlib",
        "hashcat/OpenCL",
    ],
    library_dirs=[
        hc_lib_path,
    ],
    libraries=[
        hc_lib,
    ],
    extra_link_args=[
        "-shared",
        "-Wl,-R/usr/local/lib",
    ],
    sources=[
        "src/backend.c",
        "src/hash.c",
        "src/hashcat.c",
        "src/options.c",
        "src/pyhashcat.c",
        "src/session.c",
    ],
    extra_compile_args=[
        "-std=c99",
        "-DWITH_BRAIN",
        "-Wimplicit-function-declaration",
    ],
)

setup(
    name="pyhashcat",
    version="4.0",
    description="Python bindings for hashcat",
    author="Rich Kelley",
    author_email="rk5devmail@gmail.com",
    url="www.bytesdarkly.com",
    ext_modules=[
        pyhashcat_module,
    ],
)
