#ifndef PYHASHCAT_HASH_H
#define PYHASHCAT_HASH_H

#include <Python.h>

#include <hashcat/types.h>

// HASH

typedef struct {
    PyObject_HEAD

    unsigned int mode;
    char *name;
} HashcatHash;

PyTypeObject HashcatHash_Type;

#define HashcatHash_Check(v) (Py_TYPE(v) == &HashcatHash_Type)

HashcatHash *HashcatHash_New(unsigned int mode, const char *name);

PyObject *HashcatHash_GetMode(HashcatHash *self);

int HashcatHash_SetMode(HashcatHash *self, PyObject *value, void *closure);

PyObject *HashcatHash_GetName(HashcatHash *self);

int HashcatHash_SetName(HashcatHash *self, PyObject *value, void *closure);

PyObject *HashcatHash_Repr(HashcatHash *self);

void HashcatHash_Del(HashcatHash *self);

// MODULE

struct PyModuleDef HashcatHash_Module;

PyMODINIT_FUNC PyInit_HashcatHash(void);

#endif // PYHASHCAT_HASH_H
