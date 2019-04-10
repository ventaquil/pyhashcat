# pyhashcat

Python bindings for hashcat
------
Python C API binding to libhashcat, originally written by Rich5. Updated to use the latest Hashcat version, with additional functionality added. 


Pulled from here: https://github.com/Rich5/pyhashcat/tree/master/pyhashcat
and ported to Python v3 here: https://github.com/initiate6/pyhashcat/tree/master/pyhashcat

pyhashcat has been completely rewritten as a Python C extension to interface directly with libhashcat. The pyhashcat module now acts as direct bindings to hashcat.

VERSION: 2.2


Requirements: 
* libhashcat 5.1.0
* Python 3.6/7

### Install libhashcat and pyhashcat:

```
git clone https://github.com/Rich5/pyHashcat.git
cd pyhashcat/pyhashcat
git clone https://github.com/hashcat/hashcat.git
cd hashcat/
sudo make install_library
sudo make install
cd ..
python setup.py build_ext -R /usr/local/lib
sudo python setup.py install
```

### Simple Test:

```
user@host:~/pyHashcat/pyhashcat$ python simple_mask.py
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
