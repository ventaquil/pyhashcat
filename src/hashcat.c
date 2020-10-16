#include <stdlib.h>

#include <hashcat/types.h>
#include <hashcat/folder.h>
#include <hashcat/hashcat.h>
#include <hashcat/interface.h>
#include <hashcat/memory.h>
#include <hashcat/user_options.h>

#include "backend.h"
#include "hash.h"
#include "hashcat.h"
#include "options.h"
#include "session.h"

// HASHCAT

static PyObject *_Hashcat_GetHashes(Hashcat *self) {
    char *modulefile = (char *) hcmalloc(HCBUFSIZ_TINY);
    if (modulefile == NULL) {
        // todo
    }

    HashcatSession *session = self->session;
    Py_DECREF(HashcatSession_Initialize(session, NULL));

    PyObject *hashes = PyList_New(0);
    hashcat_ctx_t *hashcat_ctx = self->ctx;
    user_options_t *user_options = hashcat_ctx->user_options;
    const folder_config_t *folder_config = hashcat_ctx->folder_config;
    const hashconfig_t *hashconfig = hashcat_ctx->hashconfig;
    for (unsigned int i = 0; i < MODULE_HASH_MODES_MAXIMUM; i++) {
        user_options->hash_mode = i;

        module_filename(folder_config, i, modulefile, HCBUFSIZ_TINY);

        if (hc_path_exist(modulefile) == false) {
            continue;
        }

        if (hashconfig_init(hashcat_ctx) == 0) {
            const HashcatHash *hash = HashcatHash_New(hashconfig->hash_mode, hashconfig->hash_name);
            PyList_Append(hashes, (PyObject *) hash);
        }

        hashconfig_destroy(hashcat_ctx);
    }

    Py_DECREF(HashcatSession_Destroy(session, NULL));

    hcfree(modulefile);

    return hashes;
}

Hashcat *Hashcat_New(PyTypeObject *self, PyObject *noargs, PyObject *nokwds) {
    if (!PyArg_ParseTuple(noargs, ":new")) {
        return NULL;
    }

    Hashcat *hashcat = PyObject_New(Hashcat, &Hashcat_Type);
    if (hashcat == NULL) {
        return NULL;
    }

    hashcat->ctx = (hashcat_ctx_t *) calloc(1, sizeof(hashcat_ctx_t));
    if (hashcat->ctx == NULL) {
        // todo
    }
    if (hashcat_init(hashcat->ctx, NULL) != 0) {
        // todo
    }
    if (user_options_init(hashcat->ctx) != 0) {
        // todo
    }

    Py_INCREF(hashcat);
    HashcatBackend *backend = HashcatBackend_New(hashcat);
    hashcat->backend = (PyObject *) backend;

    Py_INCREF(hashcat);
    HashcatOptions *options = HashcatOptions_New(hashcat);
    hashcat->options = (PyObject *) options;

    Py_INCREF(hashcat);
    HashcatSession *session = HashcatSession_New(hashcat);
    hashcat->session = (PyObject *) session;

    PyObject *hashes = _Hashcat_GetHashes(hashcat);
    hashcat->hashes = hashes;

    return hashcat;
}

// PyDoc_STRVAR(Hashcat_DocBackend, ""); // todo

PyObject *Hashcat_GetBackend(Hashcat *self) {
    HashcatBackend *backend = (HashcatBackend *) self->backend;
    Py_INCREF(backend);
    return (PyObject *) backend;
}

int Hashcat_SetBackend(Hashcat *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify backend attribute");
    return -1;
}

// PyDoc_STRVAR(Hashcat_DocHashes, ""); // todo

PyObject *Hashcat_GetHashes(Hashcat *self) {
    const unsigned int hashes_count = PyList_Size(self->hashes);
    PyObject *hashes = PyList_New(hashes_count);
    for (unsigned int hash_id = 0; hash_id < hashes_count; hash_id++) {
        PyObject *hash = PyList_GetItem(self->hashes, hash_id);
        Py_INCREF(hash);
        PyList_SetItem(hashes, hash_id, hash);
    }
    return hashes;
}

int Hashcat_SetHashes(Hashcat *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify hashes attribute");
    return -1;
}

// PyDoc_STRVAR(Hashcat_DocOptions, ""); // todo

PyObject *Hashcat_GetOptions(Hashcat *self) {
    HashcatOptions *options = (HashcatOptions *) self->options;
    Py_INCREF(options);
    return (PyObject *) options;
}

int Hashcat_SetOptions(Hashcat *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify options attribute");
    return -1;
}

// PyDoc_STRVAR(Hashcat_DocSession, ""); // todo

PyObject *Hashcat_GetSession(Hashcat *self) {
    HashcatSession *session = (HashcatSession *) self->session;
    Py_INCREF(session);
    return (PyObject *) session;
}

int Hashcat_SetSession(Hashcat *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify session attribute");
    return -1;
}

PyObject *Hashcat_Repr(Hashcat *self) {
    PyObject *options = Hashcat_GetOptions(self);
    PyObject *repr = PyUnicode_FromFormat("Hashcat(options=%R)", options);
    Py_DECREF(options);
    return repr;
}

void Hashcat_Del(Hashcat *self) {
    hashcat_destroy(self->ctx);
    free(self->ctx);
    Py_DECREF(self->backend);
    Py_DECREF(self->hashes);
    Py_DECREF(self->options);
    Py_DECREF(self->session);
    PyObject_Del(self);
}

PyGetSetDef Hashcat_GetSet[] = {
    {"backend", (getter) Hashcat_GetBackend, (setter) Hashcat_SetBackend, /* Hashcat_DocBackend */ NULL, NULL},
    {"hashes", (getter) Hashcat_GetHashes, (setter) Hashcat_SetHashes, /* Hashcat_DocHashes */ NULL, NULL},
    {"options", (getter) Hashcat_GetOptions, (setter) Hashcat_SetOptions, /* Hashcat_DocOptions */ NULL, NULL},
    {"session", (getter) Hashcat_GetSession, (setter) Hashcat_SetSession, /* Hashcat_DocSession */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(Hashcat_DocType, ""); // todo

PyTypeObject Hashcat_Type = {
    PyVarObject_HEAD_INIT(NULL, 0) /* ob_size */
    "pyhashcat.hashcat.Hashcat",   /* tp_name */
    sizeof(Hashcat),               /* tp_basicsize */
    NULL,                          /* tp_itemsize */
    (destructor) Hashcat_Del,      /* tp_dealloc */
    NULL,                          /* tp_print */
    NULL,                          /* tp_getattr */
    NULL,                          /* tp_setattr */
    NULL,                          /* tp_compare */
    Hashcat_Repr,                  /* tp_repr */
    NULL,                          /* tp_as_number */
    NULL,                          /* tp_as_sequence */
    NULL,                          /* tp_as_mapping */
    NULL,                          /* tp_hash */
    NULL,                          /* tp_call */
    NULL,                          /* tp_str */
    NULL,                          /* tp_getattro */
    NULL,                          /* tp_setattro */
    NULL,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,            /* tp_flags */
    /* Hashcat_DocType */ NULL,    /* tp_doc */
    NULL,                          /* tp_traverse */
    NULL,                          /* tp_clear */
    NULL,                          /* tp_richcompare */
    NULL,                          /* tp_weaklistoffset */
    NULL,                          /* tp_iter */
    NULL,                          /* tp_iternext */
    NULL,                          /* tp_methods */
    NULL,                          /* tp_members */
    Hashcat_GetSet,                /* tp_getset */
    NULL,                          /* tp_base */
    NULL,                          /* tp_dict */
    NULL,                          /* tp_descr_get */
    NULL,                          /* tp_descr_set */
    NULL,                          /* tp_dictoffset */
    NULL,                          /* tp_init */
    NULL,                          /* tp_alloc */
    Hashcat_New,                   /* tp_new */
};

// MODULE

// PyDoc_STRVAR(Hashcat_DocModule, ""); // todo

struct PyModuleDef Hashcat_Module = {
    PyModuleDef_HEAD_INIT,
    "pyhashcat.hashcat",          /* m_name */
    /* Hashcat_DocModule */ NULL, /* m_doc */
    -1,                           /* m_size */
    NULL,                         /* m_methods */
    NULL,                         /* m_reload */
    NULL,                         /* m_traverse */
    NULL,                         /* m_clear */
    NULL,                         /* m_free */
};

PyMODINIT_FUNC PyInit_Hashcat(void) {
    PyObject *module = PyModule_Create(&Hashcat_Module);
    if (module == NULL) {
        return NULL;
    }

    if (PyType_Ready(&Hashcat_Type) < 0) {
        return NULL;
    }
    PyModule_AddObject(module, "Hashcat", (PyObject *) &Hashcat_Type);

    return module;
}
