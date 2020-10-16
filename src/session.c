#include <stdlib.h>

#include <hashcat/types.h>
#include <hashcat/hashcat.h>
#include <hashcat/memory.h>
#include <hashcat/user_options.h>

#include "hashcat.h"
#include "options.h"
#include "session.h"

// SESSION

HashcatSession *HashcatSession_New(Hashcat *hashcat) {
    HashcatSession *self = PyObject_New(HashcatSession, &HashcatSession_Type);
    if (self == NULL) {
        return NULL;
    }
    self->hashcat = hashcat;
    return self;
}

// PyDoc_STRVAR(HashcatSession_DocInitialize, ""); // todo

PyObject *HashcatSession_Initialize(HashcatSession *self, PyObject *noargs) {
    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;

    size_t hc_argc = 1;
    char **hc_argv = (char **) calloc(hc_argc, sizeof(char *));
    hc_argv[0] = NULL;
    user_options_t *user_options = hashcat_ctx->user_options;
    user_options->hc_argc = hc_argc;
    user_options->hc_argv = hc_argv;

    HashcatOptions *options = hashcat->options;
    Py_DECREF(HashcatOptions_Restore(options, NULL));

    char *py_path = "/usr/bin/";                // fixme determine program directory in runtime
    char *hc_path = "/usr/local/share/hashcat"; // todo can load from hashcat header files?
    if (hashcat_session_init(hashcat_ctx, py_path, hc_path, 0, NULL, 0) != 0) {
        // todo
    }

    Py_INCREF(Py_True);
    return Py_True;
}

// PyDoc_STRVAR(HashcatSession_DocExecute, ""); // todo

PyObject *HashcatSession_Execute(HashcatSession *self, PyObject *noargs) {
    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;

    if (hashcat_session_execute(hashcat_ctx) != 0) {
        // todo
    }

    Py_INCREF(Py_True);
    return Py_True;
}

// PyDoc_STRVAR(HashcatSession_DocDestroy, ""); // todo

PyObject *HashcatSession_Destroy(HashcatSession *self, PyObject *noargs) {
    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;

    user_options_t *user_options = hashcat_ctx->user_options;
    user_options->hc_argc = 0;
    free(user_options->hc_argv);
    user_options->hc_argv = NULL;

    if (hashcat_session_destroy(hashcat_ctx) != 0) {
        // todo
    }

    user_options->rp_files = (char **) hccalloc(256, sizeof(char *)); // recreate freed memory

    Py_INCREF(Py_True);
    return Py_True;
}

PyObject *HashcatSession_Repr(HashcatSession *self) {
    PyObject *repr = PyUnicode_FromString("Session()");
    return repr;
}

void HashcatSession_Del(HashcatSession *self) {
    Py_DECREF(self->hashcat);
    PyObject_Del(self);
}

PyMethodDef HashcatSession_Methods[] = {
    {"initialize", (PyCFunction) HashcatSession_Initialize, METH_NOARGS, /* HashcatSession_DocInitialize */ NULL},
    {"execute", (PyCFunction) HashcatSession_Execute, METH_NOARGS, /* HashcatSession_DocExecute */ NULL},
    {"destroy", (PyCFunction) HashcatSession_Destroy, METH_NOARGS, /* HashcatSession_DocDestroy */ NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatSession_DocType, ""); // todo

PyTypeObject HashcatSession_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)     /* ob_size */
    "pyhashcat.session.Session",       /* tp_name */
    sizeof(Hashcat),                   /* tp_basicsize */
    NULL,                              /* tp_itemsize */
    (destructor) HashcatSession_Del,   /* tp_dealloc */
    NULL,                              /* tp_print */
    NULL,                              /* tp_getattr */
    NULL,                              /* tp_setattr */
    NULL,                              /* tp_compare */
    HashcatSession_Repr,               /* tp_repr */
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
    /* HashcatSession_DocType */ NULL, /* tp_doc */
    NULL,                              /* tp_traverse */
    NULL,                              /* tp_clear */
    NULL,                              /* tp_richcompare */
    NULL,                              /* tp_weaklistoffset */
    NULL,                              /* tp_iter */
    NULL,                              /* tp_iternext */
    HashcatSession_Methods,            /* tp_methods */
    NULL,                              /* tp_members */
    NULL,                              /* tp_getset */
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

// PyDoc_STRVAR(HashcatSession_DocModule, ""); // todo

struct PyModuleDef HashcatSession_Module = {
    PyModuleDef_HEAD_INIT,
    "pyhashcat.session",                 /* m_name */
    /* HashcatSession_DocModule */ NULL, /* m_doc */
    -1,                                  /* m_size */
    NULL,                                /* m_methods */
    NULL,                                /* m_reload */
    NULL,                                /* m_traverse */
    NULL,                                /* m_clear */
    NULL,                                /* m_free */
};

PyMODINIT_FUNC PyInit_HashcatSession(void) {
    PyObject *module = PyModule_Create(&HashcatSession_Module);
    if (module == NULL) {
        return NULL;
    }

    if (PyType_Ready(&HashcatSession_Type) < 0) {
        return NULL;
    }
    PyModule_AddObject(module, "Session", (PyObject *) &HashcatSession_Type);

    return module;
}
