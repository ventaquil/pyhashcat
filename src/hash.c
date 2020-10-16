#include <stdlib.h>
#include <string.h>

#include "hash.h"

// HASH

HashcatHash *HashcatHash_New(unsigned int mode, const char *name) {
    HashcatHash *self = PyObject_New(HashcatHash, &HashcatHash_Type);
    if (self == NULL) {
        return NULL;
    }
    self->mode = mode;
    self->name = (char *) calloc(strlen(name) + 1, sizeof(char));
    if (self->name == NULL) {
        return NULL; // todo
    }
    strcpy(self->name, name);
    return self;
}

// PyDoc_STRVAR(HashcatHash_DocMode, ""); // todo

PyObject *HashcatHash_GetMode(HashcatHash *self) {
    PyObject *mode = PyLong_FromUnsignedLong(self->mode);
    return mode;
}

int HashcatHash_SetMode(HashcatHash *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify mode attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatHash_DocName, ""); // todo

PyObject *HashcatHash_GetName(HashcatHash *self) {
    PyObject *name = PyUnicode_FromString(self->name);
    return name;
}

int HashcatHash_SetName(HashcatHash *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify name attribute");
    return -1;
}

PyObject *HashcatHash_Repr(HashcatHash *self) {
    PyObject *mode = HashcatHash_GetMode(self);
    PyObject *name = HashcatHash_GetName(self);
    PyObject *repr = PyUnicode_FromFormat("Hash(mode=%R, name=%R)", mode, name);
    Py_DECREF(mode);
    Py_DECREF(name);
    return repr;
}

void HashcatHash_Del(HashcatHash *self) {
    free(self->name);
    PyObject_Del(self);
}

PyGetSetDef HashcatHash_GetSet[] = {
    {"mode", (getter) HashcatHash_GetMode, (setter) HashcatHash_SetMode, /* HashcatHash_DocMode */ NULL, NULL},
    {"name", (getter) HashcatHash_GetName, (setter) HashcatHash_SetName, /* HashcatHash_DocName */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatHash_DocType, ""); // todo

PyTypeObject HashcatHash_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)  /* ob_size */
    "pyhashcat.hash.Hash",          /* tp_name */
    sizeof(HashcatHash),            /* tp_basicsize */
    NULL,                           /* tp_itemsize */
    (destructor) HashcatHash_Del,   /* tp_dealloc */
    NULL,                           /* tp_print */
    NULL,                           /* tp_getattr */
    NULL,                           /* tp_setattr */
    NULL,                           /* tp_compare */
    HashcatHash_Repr,               /* tp_repr */
    NULL,                           /* tp_as_number */
    NULL,                           /* tp_as_sequence */
    NULL,                           /* tp_as_mapping */
    NULL,                           /* tp_hash */
    NULL,                           /* tp_call */
    NULL,                           /* tp_str */
    NULL,                           /* tp_getattro */
    NULL,                           /* tp_setattro */
    NULL,                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    /* HashcatHash_DocType */ NULL, /* tp_doc */
    NULL,                           /* tp_traverse */
    NULL,                           /* tp_clear */
    NULL,                           /* tp_richcompare */
    NULL,                           /* tp_weaklistoffset */
    NULL,                           /* tp_iter */
    NULL,                           /* tp_iternext */
    NULL,                           /* tp_methods */
    NULL,                           /* tp_members */
    HashcatHash_GetSet,             /* tp_getset */
    NULL,                           /* tp_base */
    NULL,                           /* tp_dict */
    NULL,                           /* tp_descr_get */
    NULL,                           /* tp_descr_set */
    NULL,                           /* tp_dictoffset */
    NULL,                           /* tp_init */
    NULL,                           /* tp_alloc */
    NULL,                           /* tp_new */
};

// MODULE

// PyDoc_STRVAR(HashcatHash_DocModule, ""); // todo

struct PyModuleDef HashcatHash_Module = {
    PyModuleDef_HEAD_INIT,
    "pyhashcat.hash",                 /* m_name */
    /* HashcatHash_DocModule */ NULL, /* m_doc */
    -1,                               /* m_size */
    NULL,                             /* m_methods */
    NULL,                             /* m_reload */
    NULL,                             /* m_traverse */
    NULL,                             /* m_clear */
    NULL,                             /* m_free */
};

PyMODINIT_FUNC PyInit_HashcatHash(void) {
    PyObject *module = PyModule_Create(&HashcatHash_Module);
    if (module == NULL) {
        return NULL;
    }

    if (PyType_Ready(&HashcatHash_Type) < 0) {
        return NULL;
    }
    PyModule_AddObject(module, "Hash", (PyObject *) &HashcatHash_Type);

    return module;
}
