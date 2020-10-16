#ifndef PYHASHCAT_SESSION_H
#define PYHASHCAT_SESSION_H

#include <Python.h>

#include "hashcat.h"

// SESSION

typedef struct {
    PyObject_HEAD

    Hashcat *hashcat;
} HashcatSession;

PyTypeObject HashcatSession_Type;

#define HashcatSession_Check(v) (Py_TYPE(v) == &HashcatSession_Type)

HashcatSession *HashcatSession_New(Hashcat *hashcat);

PyObject *HashcatSession_Initialize(HashcatSession *self, PyObject *noargs);

PyObject *HashcatSession_Execute(HashcatSession *self, PyObject *noargs);

PyObject *HashcatSession_Destroy(HashcatSession *self, PyObject *noargs);

PyObject *HashcatSession_Repr(HashcatSession *self);

void HashcatSession_Del(HashcatSession *self);

// MODULE

struct PyModuleDef HashcatSession_Module;

PyMODINIT_FUNC PyInit_HashcatSession(void);

#endif // PYHASHCAT_SESSION_H
