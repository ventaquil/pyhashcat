# pyhashcat

Python bindings for hashcat
------
Python C API binding to libhashcat, originally written by Rich5. Updated to use the latest Hashcat version, with additional functionality added. 


Pulled from here: https://github.com/Rich5/pyhashcat/tree/master/pyhashcat and ported to Python v3 here: https://github.com/initiate6/pyhashcat/tree/master/pyhashcat

pyhashcat has been completely rewritten as a Python C extension to interface directly with libhashcat. The pyhashcat module now acts as direct bindings to Hashcat.

VERSION: 4.0-alpha

Requirements: 
* libhashcat 6.1.1
* Python +3.6

### Install libhashcat and pyhashcat:

```
user@host:~$ git clone --recurse-submodules https://github.com/ventaquil/pyhashcat.git
user@host:~$ cd pyhashcat/
user@host:~/pyhashcat$ cd hashcat/
user@host:~/pyhashcat/hashcat$ sudo make -j$(nproc) install_library install_library_dev install_modules install_kernels
user@host:~/pyhashcat/hashcat$ cd ..
user@host:~/pyhashcat$ python3 setup.py build_ext -R /usr/local/lib
user@host:~/pyhashcat$ sudo python3 setup.py install
```

### Help:

```
import pyhashcat
help(pyhashcat)
```

### Example

```
import pyhashcat
hashcat = pyhashcat.hashcat.Hashcat()    # create new Hashcat object
print(hashcat.backend.platforms)         # return backend platforms
print(hashcat.backend.devices)           # return backend devices
print(hashcat.hashes)                    # return available hashes
hashcat.options.force = True
hashcat.options.workload_profile = 4     # change workload profile
platform = hashcat.backend.platforms[0]  # get first platform
device = hashcat.backend.devices[0]      # get first device
hash = hashcat.hashes[0]                 # get first hash
platform.benchmark(hash)                 # perform platform benchmark
device.benchmark(hash)                   # perform device benchmark
hashes = [
    hashcat.hashes[ 0],
    hashcat.hashes[-1],
]
device.benchmark(hashes)                 # can pass list
```
