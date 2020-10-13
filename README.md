# pyhashcat

Python bindings for hashcat
------
Python C API binding to libhashcat, originally written by Rich5. Updated to use the latest Hashcat version, with additional functionality added. 


Pulled from here: https://github.com/Rich5/pyhashcat/tree/master/pyhashcat
and ported to Python v3 here: https://github.com/initiate6/pyhashcat/tree/master/pyhashcat

pyhashcat has been completely rewritten as a Python C extension to interface directly with libhashcat. The pyhashcat module now acts as direct bindings to hashcat.

VERSION: 3.0


Requirements: 
* libhashcat 5.1.0
* Python 3.6/7

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

### Simple Test:

```
user@host:~/pyhashcat$ python3 pyhashcat/simple_mask.py
-------------------------------
---- Simple pyhashcat Test ----
-------------------------------
[+] Running hashcat
STATUS:  Cracked
8743b52063cd84097a65d1633f5c74f5  -->  hashcat
```

### Help:

```
import pyhashcat
help(pyhashcat)
```
