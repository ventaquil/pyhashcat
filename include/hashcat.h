#ifndef PYHASHCAT_HASHCAT_H
#define PYHASHCAT_HASHCAT_H

#include <Python.h>

#include <hashcat/types.h>

// HASHCAT

typedef struct {
    PyObject_HEAD

    hashcat_ctx_t *ctx;

    PyObject *backend;
    PyObject *hashes;
    PyObject *options;
    PyObject *session;
} Hashcat;

PyTypeObject Hashcat_Type;

#define Hashcat_Check(v) (Py_TYPE(v) == &Hashcat_Type)

Hashcat *Hashcat_New(PyTypeObject *self, PyObject *noargs, PyObject *nokwds);

PyObject *Hashcat_GetBackend(Hashcat *self);

int Hashcat_SetBackend(Hashcat *self, PyObject *value, void *closure);

PyObject *Hashcat_GetHashes(Hashcat *self);

int Hashcat_SetHashes(Hashcat *self, PyObject *value, void *closure);

PyObject *Hashcat_GetOptions(Hashcat *self);

int Hashcat_SetOptions(Hashcat *self, PyObject *value, void *closure);

PyObject *Hashcat_GetSession(Hashcat *self);

int Hashcat_SetSession(Hashcat *self, PyObject *value, void *closure);

PyObject *Hashcat_Repr(Hashcat *self);

void Hashcat_Del(Hashcat *self);

// MODULE

struct PyModuleDef Hashcat_Module;

PyMODINIT_FUNC PyInit_Hashcat(void);

#endif // PYHASHCAT_HASHCAT_H
