#ifndef PYHASHCAT_OPTIONS_H
#define PYHASHCAT_OPTIONS_H

#include <stdbool.h>

#include <Python.h>

#include "hashcat.h"

// WORKLOAD PROFILE

typedef enum {
    HashcatOptionsWorkloadProfile_LOW = 1,
    HashcatOptionsWorkloadProfile_DEFAULT = 2,
    HashcatOptionsWorkloadProfile_HIGH = 3,
    HashcatOptionsWorkloadProfile_NIGHTMARE = 4,
} HashcatOptionsWorkloadProfile;

// OPTIONS

typedef struct {
    PyObject_HEAD

    Hashcat *hashcat;

    bool force;
    HashcatOptionsWorkloadProfile workload_profile;
} HashcatOptions;

PyTypeObject HashcatOptions_Type;

#define HashcatOptions_Check(v) (Py_TYPE(v) == &HashcatOptions_Type)

HashcatOptions *HashcatOptions_New(Hashcat *hashcat);

PyObject *HashcatOptions_Restore(HashcatOptions *self, PyObject *noargs);

PyObject *HashcatOptions_GetForce(HashcatOptions *self);

int HashcatOptions_SetForce(HashcatOptions *self, PyObject *value, void *closure);

PyObject *HashcatOptions_GetWorkloadProfile(HashcatOptions *self);

int HashcatOptions_SetWorkloadProfile(HashcatOptions *self, PyObject *value, void *closure);

PyObject *HashcatOptions_Repr(HashcatOptions *self);

void HashcatOptions_Del(HashcatOptions *self);

// MODULE

struct PyModuleDef HashcatOptions_Module;

PyMODINIT_FUNC PyInit_HashcatOptions(void);

#endif // PYHASHCAT_OPTIONS_H
