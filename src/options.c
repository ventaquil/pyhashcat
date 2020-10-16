#include <hashcat/types.h>

#include "options.h"

// OPTIONS

HashcatOptions *HashcatOptions_New(Hashcat *hashcat) {
    HashcatOptions *self = PyObject_New(HashcatOptions, &HashcatOptions_Type);
    if (self == NULL) {
        return NULL;
    }
    self->hashcat = hashcat;
    self->force = false;
    self->workload_profile = HashcatOptionsWorkloadProfile_DEFAULT;
    return self;
}

// PyDoc_STRVAR(HashcatOptions_DocRestore, ""); // todo

PyObject *HashcatOptions_Restore(HashcatOptions *self, PyObject *noargs) {
    const Hashcat *hashcat = self->hashcat;
    const hashcat_ctx_t *hashcat_ctx = hashcat->ctx;
    user_options_t *user_options = hashcat_ctx->user_options;
    user_options->force = self->force;
    user_options->workload_profile = self->workload_profile;
    user_options->workload_profile_chgd = true;

    Py_INCREF(Py_True);
    return Py_True;
}

// PyDoc_STRVAR(HashcatOptions_DocForce, ""); // todo

PyObject *HashcatOptions_GetForce(HashcatOptions *self) {
    PyObject *force = PyBool_FromLong(self->force);
    return force;
}

int HashcatOptions_SetForce(HashcatOptions *self, PyObject *value, void *closure) {
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete force attribute");
        return -1;
    }

    if (!PyBool_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The force attribute value must be a bool");
        return -1;
    }

    if (PyObject_IsTrue(value)) {
        self->force = true;
    } else {
        self->force = false;
    }

    return 0;
}

PyObject *HashcatOptions_GetWorkloadProfile(HashcatOptions *self) {
    PyObject *workload_profile = PyLong_FromUnsignedLong(self->workload_profile);
    return workload_profile;
}

int HashcatOptions_SetWorkloadProfile(HashcatOptions *self, PyObject *value, void *closure) {
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete workload_profile attribute");
        return -1;
    }

    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The workload_profile attribute value must be a int");
        return -1;
    }

    self->workload_profile = PyLong_AsUnsignedLong(value);

    return 0;
}

PyObject *HashcatOptions_Repr(HashcatOptions *self) {
    PyObject *force = HashcatOptions_GetForce(self);
    PyObject *workload_profile = HashcatOptions_GetWorkloadProfile(self);
    PyObject *repr = PyUnicode_FromFormat("Options(force=%R, workload_profile=%R)", force, workload_profile);
    Py_DECREF(force);
    Py_DECREF(workload_profile);
    return repr;
}

void HashcatOptions_Del(HashcatOptions *self) {
    Py_DECREF(self->hashcat);
    PyObject_Del(self);
}

PyMethodDef HashcatOptions_Methods[] = {
    {"restore", (PyCFunction) HashcatOptions_Restore, METH_NOARGS, /* HashcatOptions_DocRestore */ NULL},
    {NULL},
};

PyGetSetDef HashcatOptions_GetSet[] = {
    {"force", (getter) HashcatOptions_GetForce, (setter) HashcatOptions_SetForce, /* HashcatOptions_DocForce */ NULL, NULL},
    {"workload_profile", (getter) HashcatOptions_GetWorkloadProfile, (setter) HashcatOptions_SetWorkloadProfile, /* HashcatOptions_DocWorkloadProfile */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatOptions_DocType, ""); // todo

PyTypeObject HashcatOptions_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)     /* ob_size */
    "pyhashcat.options.Options",       /* tp_name */
    sizeof(Hashcat),                   /* tp_basicsize */
    NULL,                              /* tp_itemsize */
    (destructor) HashcatOptions_Del,   /* tp_dealloc */
    NULL,                              /* tp_print */
    NULL,                              /* tp_getattr */
    NULL,                              /* tp_setattr */
    NULL,                              /* tp_compare */
    HashcatOptions_Repr,               /* tp_repr */
    NULL,                              /* tp_as_number */
    NULL,                              /* tp_as_sequence */
    NULL,                              /* tp_as_mapping */
    NULL,                              /* tp_hash */
    NULL,                              /* tp_call */
    NULL,                              /* tp_str */
    NULL,                              /* tp_getattro */
    NULL,                              /* tp_setattro */
    NULL,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                /* tp_flags */
    /* HashcatOptions_DocType */ NULL, /* tp_doc */
    NULL,                              /* tp_traverse */
    NULL,                              /* tp_clear */
    NULL,                              /* tp_richcompare */
    NULL,                              /* tp_weaklistoffset */
    NULL,                              /* tp_iter */
    NULL,                              /* tp_iternext */
    HashcatOptions_Methods,            /* tp_methods */
    NULL,                              /* tp_members */
    HashcatOptions_GetSet,             /* tp_getset */
    NULL,                              /* tp_base */
    NULL,                              /* tp_dict */
    NULL,                              /* tp_descr_get */
    NULL,                              /* tp_descr_set */
    NULL,                              /* tp_dictoffset */
    NULL,                              /* tp_init */
    NULL,                              /* tp_alloc */
    NULL,                              /* tp_new */
};

// MODULE

// PyDoc_STRVAR(HashcatOptions_DocModule, ""); // todo

struct PyModuleDef HashcatOptions_Module = {
    PyModuleDef_HEAD_INIT,
    "pyhashcat.options",                 /* m_name */
    /* HashcatOptions_DocModule */ NULL, /* m_doc */
    -1,                                  /* m_size */
    NULL,                                /* m_methods */
    NULL,                                /* m_reload */
    NULL,                                /* m_traverse */
    NULL,                                /* m_clear */
    NULL,                                /* m_free */
};

PyMODINIT_FUNC PyInit_HashcatOptions(void) {
    PyObject *module = PyModule_Create(&HashcatOptions_Module);
    if (module == NULL) {
        return NULL;
    }

    if (PyType_Ready(&HashcatOptions_Type) < 0) {
        return NULL;
    }
    PyModule_AddObject(module, "Options", (PyObject *) &HashcatOptions_Type);

    return module;
}
