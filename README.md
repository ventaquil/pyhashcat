# pyhashcat

Python bindings for Hashcat

Requirements: 
* libhashcat 6.1.1
* Python +3.6

## Install libhashcat and pyhashcat

```shellsession
user@host:~$ git clone --recurse-submodules https://github.com/ventaquil/pyhashcat.git
user@host:~$ cd pyhashcat/
user@host:~/pyhashcat$ cd hashcat/
user@host:~/pyhashcat/hashcat$ sudo make -j$(nproc) install_library install_library_dev install_modules install_kernels
user@host:~/pyhashcat/hashcat$ cd ..
user@host:~/pyhashcat$ python3 setup.py build_ext -R /usr/local/lib
user@host:~/pyhashcat$ sudo python3 setup.py install
```

## Example

```python3
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

## History

```
Rich5/pyhashcat
└── initiate6/pyhashcat
    └── f0cker/pyhashcat
        └── ventaquil/pyhashcat
```
