#include <stdbool.h>
#include <stdlib.h>

#include <hashcat/types.h>
#include <hashcat/hashcat.h>
#include <hashcat/memory.h>

#include "backend.h"
#include "hash.h"
#include "hashcat.h"
#include "session.h"

// DEVICE

HashcatBackendDevice *HashcatBackendDevice_New(Hashcat *hashcat, unsigned int id, unsigned int global_id, HashcatBackendDeviceType type, const char *name, const char *vendor) {
    HashcatBackendDevice *self = PyObject_New(HashcatBackendDevice, &HashcatBackendDevice_Type);
    if (self == NULL) {
        return NULL;
    }
    self->hashcat = hashcat;
    self->id = id;
    self->global_id = global_id;
    self->type = type;
    self->name = (char *) calloc(strlen(name) + 1, sizeof(char));
    if (self->name == NULL) {
        return NULL; // todo
    }
    strcpy(self->name, name);
    self->vendor = (char *) calloc(strlen(vendor) + 1, sizeof(char));
    if (self->vendor == NULL) {
        return NULL; // todo
    }
    strcpy(self->vendor, vendor);
    return self;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocBenchmark, ""); // todo

PyObject *HashcatBackendDevice_Benchmark(HashcatBackendDevice *self, PyObject *args) {
    PyObject *hashes = NULL;

    if (!PyArg_ParseTuple(args, "O", &hashes)) {
        return NULL; // todo is valid error set?
    }

    bool single_result = false;

    if (HashcatHash_Check(hashes)) {
        const HashcatHash *hash = (HashcatHash *) hashes;
        hashes = PyList_New(0);
        PyList_Append(hashes, (PyObject *) hash);

        single_result = true;
    } else if (!PyList_Check(hashes)) {
        return NULL; // todo expected list
    } else {
        for (unsigned int hash_id = 0, hashes_count = PyList_Size(hashes); hash_id < hashes_count; hash_id++) {
            PyObject *hash = PyList_GetItem(hashes, hash_id);
            if (!HashcatHash_Check(hash)) {
                return NULL; // todo expected list of hashes
            }
        }
    }

    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;

    const unsigned int hashes_count = PyList_Size(hashes);
    PyObject *results = PyList_New(hashes_count);
    for (unsigned int hash_id = 0; hash_id < hashes_count; hash_id++) {
        HashcatHash *hash = (HashcatHash *) PyList_GetItem(hashes, hash_id);

        user_options_t *user_options = hashcat_ctx->user_options;
        user_options->hash_mode = hash->mode;
        user_options->hash_mode_chgd = true;
        user_options->benchmark = true;
        user_options->backend_ignore_cuda = true;

        PyObject *type = HashcatBackendDevice_GetType(self);
        PyObject *opencl_device_types = PyUnicode_FromFormat("%S", type);
        Py_DECREF(type);
        user_options->opencl_device_types = PyUnicode_AsUTF8(opencl_device_types);
        if (user_options->opencl_device_types == NULL) {
            // todo
        }

        PyObject *global_id = HashcatBackendDevice_GetGlobalId(self);
        PyObject *backend_devices = PyUnicode_FromFormat("%S", global_id);
        Py_DECREF(global_id);
        user_options->backend_devices = PyUnicode_AsUTF8(backend_devices);
        if (user_options->backend_devices == NULL) {
            // todo
        }

        HashcatSession *session = hashcat->session;
        Py_DECREF(HashcatSession_Initialize(session, NULL));

        Py_DECREF(HashcatSession_Execute(session, NULL));

        hashcat_status_t *hashcat_status = (hashcat_status_t *) hcmalloc(sizeof(hashcat_status_t));
        if (hashcat_status == NULL) {
            // todo
        }
        if (hashcat_get_status(hashcat_ctx, hashcat_status) != 0) {
            hcfree(hashcat_status);

            // todo
        }

        const device_info_t *device_info = &hashcat_status->device_info_buf[self->global_id - 1];
        Py_INCREF(self);
        Py_INCREF(hash);
        const HashcatBackendDeviceBenchmarkResult *result = HashcatBackendDeviceBenchmarkResult_New(self, hash, device_info->hashes_msec_dev, device_info->speed_sec_dev);
        PyList_SetItem(results, hash_id, (PyObject *) result);

        Py_DECREF(HashcatSession_Destroy(session, NULL));

        Py_DECREF(opencl_device_types);
        Py_DECREF(backend_devices);

        if (single_result) {
            Py_INCREF(result);
            Py_DECREF(results);
            Py_DECREF(hashes);

            return (PyObject *) result;
        }
    }

    return results;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocId, ""); // todo

PyObject *HashcatBackendDevice_GetId(HashcatBackendDevice *self) {
    PyObject *id = PyLong_FromLong(self->id);
    return id;
}

int HashcatBackendDevice_SetId(HashcatBackendDevice *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify id attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocGlobalId, ""); // todo

PyObject *HashcatBackendDevice_GetGlobalId(HashcatBackendDevice *self) {
    PyObject *global_id = PyLong_FromLong(self->global_id);
    return global_id;
}

int HashcatBackendDevice_SetGlobalId(HashcatBackendDevice *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify global id attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocType, ""); // todo

PyObject *HashcatBackendDevice_GetType(HashcatBackendDevice *self) {
    PyObject *type = PyLong_FromLong(self->type);
    return type;
}

int HashcatBackendDevice_SetType(HashcatBackendDevice *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify type attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocName, ""); // todo

PyObject *HashcatBackendDevice_GetName(HashcatBackendDevice *self) {
    PyObject *name = PyUnicode_FromString(self->name);
    return name;
}

int HashcatBackendDevice_SetName(HashcatBackendDevice *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify name attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendDevice_DocVendor, ""); // todo

PyObject *HashcatBackendDevice_GetVendor(HashcatBackendDevice *self) {
    PyObject *vendor = PyUnicode_FromString(self->vendor);
    return vendor;
}

int HashcatBackendDevice_SetVendor(HashcatBackendDevice *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify vendor attribute");
    return -1;
}

PyObject *HashcatBackendDevice_Repr(HashcatBackendDevice *self) {
    PyObject *id = HashcatBackendDevice_GetId(self);
    PyObject *global_id = HashcatBackendDevice_GetGlobalId(self);
    PyObject *type = HashcatBackendDevice_GetType(self);
    PyObject *name = HashcatBackendDevice_GetName(self);
    PyObject *vendor = HashcatBackendDevice_GetVendor(self);
    PyObject *repr = PyUnicode_FromFormat("Device(id=%R, global_id=%R, type=%R, name=%R, vendor=%R)", id, global_id, type, name, vendor);
    Py_DECREF(id);
    Py_DECREF(global_id);
    Py_DECREF(type);
    Py_DECREF(name);
    Py_DECREF(vendor);
    return repr;
}

void HashcatBackendDevice_Del(HashcatBackendDevice *self) {
    Py_DECREF(self->hashcat);
    free(self->vendor);
    free(self->name);
    PyObject_Del(self);
}

PyMethodDef HashcatBackendDevice_Methods[] = {
    {"benchmark", (PyCFunction) HashcatBackendDevice_Benchmark, METH_VARARGS, /* HashcatBackendDevice_DocBenchmark */ NULL},
    {NULL},
};

PyGetSetDef HashcatBackendDevice_GetSet[] = {
    {"id", (getter) HashcatBackendDevice_GetId, (setter) HashcatBackendDevice_SetId, /* HashcatBackendDevice_DocId */ NULL, NULL},
    {"global_id", (getter) HashcatBackendDevice_GetGlobalId, (setter) HashcatBackendDevice_SetGlobalId, /* HashcatBackendDevice_DocGlobalId */ NULL, NULL},
    {"type", (getter) HashcatBackendDevice_GetType, (setter) HashcatBackendDevice_SetType, /* HashcatBackendDevice_DocType */ NULL, NULL},
    {"name", (getter) HashcatBackendDevice_GetName, (setter) HashcatBackendDevice_SetName, /* HashcatBackendDevice_DocName */ NULL, NULL},
    {"vendor", (getter) HashcatBackendDevice_GetVendor, (setter) HashcatBackendDevice_SetVendor, /* HashcatBackendDevice_DocVendor */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackendDevice_DocType, ""); // todo

PyTypeObject HashcatBackendDevice_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)           /* ob_size */
    "pyhashcat.backend.Device",              /* tp_name */
    sizeof(HashcatBackendDevice),            /* tp_basicsize */
    NULL,                                    /* tp_itemsize */
    (destructor) HashcatBackendDevice_Del,   /* tp_dealloc */
    NULL,                                    /* tp_print */
    NULL,                                    /* tp_getattr */
    NULL,                                    /* tp_setattr */
    NULL,                                    /* tp_compare */
    HashcatBackendDevice_Repr,               /* tp_repr */
    NULL,                                    /* tp_as_number */
    NULL,                                    /* tp_as_sequence */
    NULL,                                    /* tp_as_mapping */
    NULL,                                    /* tp_hash */
    NULL,                                    /* tp_call */
    NULL,                                    /* tp_str */
    NULL,                                    /* tp_getattro */
    NULL,                                    /* tp_setattro */
    NULL,                                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                      /* tp_flags */
    /* HashcatBackendDevice_DocType */ NULL, /* tp_doc */
    NULL,                                    /* tp_traverse */
    NULL,                                    /* tp_clear */
    NULL,                                    /* tp_richcompare */
    NULL,                                    /* tp_weaklistoffset */
    NULL,                                    /* tp_iter */
    NULL,                                    /* tp_iternext */
    HashcatBackendDevice_Methods,            /* tp_methods */
    NULL,                                    /* tp_members */
    HashcatBackendDevice_GetSet,             /* tp_getset */
    NULL,                                    /* tp_base */
    NULL,                                    /* tp_dict */
    NULL,                                    /* tp_descr_get */
    NULL,                                    /* tp_descr_set */
    NULL,                                    /* tp_dictoffset */
    NULL,                                    /* tp_init */
    NULL,                                    /* tp_alloc */
    NULL,                                    /* tp_new */
};

// PLATFORM

HashcatBackendPlatform *HashcatBackendPlatform_New(Hashcat *hashcat, unsigned int id, const char *name, const char *vendor, PyObject *devices) {
    HashcatBackendPlatform *self = PyObject_New(HashcatBackendPlatform, &HashcatBackendPlatform_Type);
    if (self == NULL) {
        return NULL;
    }
    self->hashcat = hashcat;
    self->id = id;
    self->name = (char *) calloc(strlen(name) + 1, sizeof(char));
    if (self->name == NULL) {
        return NULL; // todo
    }
    strcpy(self->name, name);
    self->vendor = (char *) calloc(strlen(vendor) + 1, sizeof(char));
    if (self->vendor == NULL) {
        return NULL; // todo
    }
    strcpy(self->vendor, vendor);
    self->devices = devices;
    return self;
}

// PyDoc_STRVAR(HashcatBackendPlatform_DocBenchmark, ""); // todo

PyObject *HashcatBackendPlatform_Benchmark(HashcatBackendPlatform *self, PyObject *args) {
    PyObject *hashes = NULL;

    if (!PyArg_ParseTuple(args, "O", &hashes)) {
        return NULL; // todo is valid error set?
    }

    bool single_result = false;

    if (HashcatHash_Check(hashes)) {
        const HashcatHash *hash = (HashcatHash *) hashes;
        hashes = PyList_New(0);
        PyList_Append(hashes, (PyObject *) hash);

        single_result = true;
    } else if (!PyList_Check(hashes)) {
        return NULL; // todo expected list
    } else {
        for (unsigned int hash_id = 0, hashes_count = PyList_Size(hashes); hash_id < hashes_count; hash_id++) {
            PyObject *hash = PyList_GetItem(hashes, hash_id);
            if (!HashcatHash_Check(hash)) {
                return NULL; // todo expected list of hashes
            }
        }
    }

    const unsigned int devices_count = PyList_Size(self->devices);

    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;

    const unsigned int hashes_count = PyList_Size(hashes);
    PyObject *results = PyList_New(hashes_count);
    for (unsigned int hash_id = 0; hash_id < hashes_count; hash_id++) {
        HashcatHash *hash = (HashcatHash *) PyList_GetItem(hashes, hash_id);

        user_options_t *user_options = hashcat_ctx->user_options;
        user_options->hash_mode = hash->mode;
        user_options->hash_mode_chgd = true;
        user_options->benchmark = true;
        user_options->backend_ignore_cuda = true;

        PyObject *types = PySet_New(NULL);
        PyObject *global_ids = PySet_New(NULL);
        for (unsigned int device_id = 0; device_id < devices_count; device_id++) {
            const HashcatBackendDevice *device = (HashcatBackendDevice *) PyList_GetItem(self->devices, device_id);
            PyObject *type = HashcatBackendDevice_GetType(device);
            PyObject *string_type = PyUnicode_FromFormat("%S", type);
            Py_DECREF(type);
            PySet_Add(types, string_type);

            PyObject *global_id = HashcatBackendDevice_GetGlobalId(device);
            PyObject *string_global_id = PyUnicode_FromFormat("%S", global_id);
            Py_DECREF(global_id);
            PySet_Add(global_ids, string_global_id);
        }

        PyObject *separator = PyUnicode_FromString(",");

        PyObject *opencl_device_types = PyUnicode_Join(separator, types);
        Py_DECREF(types);
        user_options->opencl_device_types = PyUnicode_AsUTF8(opencl_device_types);
        if (user_options->opencl_device_types == NULL) {
            // todo
        }

        PyObject *backend_devices = PyUnicode_Join(separator, global_ids);
        Py_DECREF(global_ids);
        user_options->backend_devices = PyUnicode_AsUTF8(backend_devices);
        if (user_options->backend_devices == NULL) {
            // todo
        }

        Py_DECREF(separator);

        HashcatSession *session = hashcat->session;
        Py_DECREF(HashcatSession_Initialize(session, NULL));

        Py_DECREF(HashcatSession_Execute(session, NULL));

        hashcat_status_t *hashcat_status = (hashcat_status_t *) hcmalloc(sizeof(hashcat_status_t));
        if (hashcat_status == NULL) {
            // todo
        }
        if (hashcat_get_status(hashcat_ctx, hashcat_status) != 0) {
            hcfree(hashcat_status);

            // todo
        }

        PyObject *devices_results = PyList_New(devices_count);
        for (unsigned int device_id = 0; device_id < devices_count; device_id++) {
            HashcatBackendDevice *device = (HashcatBackendDevice *) PyList_GetItem(self->devices, device_id);

            const device_info_t *device_info = &hashcat_status->device_info_buf[device->global_id - 1];
            Py_INCREF(device);
            Py_INCREF(hash);
            const HashcatBackendDeviceBenchmarkResult *result = HashcatBackendDeviceBenchmarkResult_New(device, hash, device_info->hashes_msec_dev, device_info->speed_sec_dev);
            PyList_SetItem(devices_results, device_id, (PyObject *) result);
        }

        Py_INCREF(self);
        Py_INCREF(hash);
        const HashcatBackendPlatformBenchmarkResult *result = HashcatBackendPlatformBenchmarkResult_New(self, hash, hashcat_status->hashes_msec_all, hashcat_status->speed_sec_all, devices_results);
        PyList_SetItem(results, hash_id, (PyObject *) result);

        Py_DECREF(HashcatSession_Destroy(session, NULL));

        Py_DECREF(opencl_device_types);
        Py_DECREF(backend_devices);

        if (single_result) {
            Py_INCREF(result);
            Py_DECREF(results);
            Py_DECREF(hashes);

            return (PyObject *) result;
        }
    }

    return results;
}

// PyDoc_STRVAR(HashcatBackendPlatform_DocId, ""); // todo

PyObject *HashcatBackendPlatform_GetId(HashcatBackendPlatform *self) {
    PyObject *id = PyLong_FromLong(self->id);
    return id;
}

int HashcatBackendPlatform_SetId(HashcatBackendPlatform *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify id attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendPlatform_DocName, ""); // todo

PyObject *HashcatBackendPlatform_GetName(HashcatBackendPlatform *self) {
    PyObject *name = PyUnicode_FromString(self->name);
    return name;
}

int HashcatBackendPlatform_SetName(HashcatBackendPlatform *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify name attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendPlatform_DocVendor, ""); // todo

PyObject *HashcatBackendPlatform_GetVendor(HashcatBackendPlatform *self) {
    PyObject *vendor = PyUnicode_FromString(self->vendor);
    return vendor;
}

int HashcatBackendPlatform_SetVendor(HashcatBackendPlatform *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify vendor attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendPlatform_DocDevices, ""); // todo

PyObject *HashcatBackendPlatform_GetDevices(HashcatBackendPlatform *self) {
    const unsigned int devices_count = PyList_Size(self->devices);
    PyObject *devices = PyList_New(devices_count);
    for (unsigned int device_id = 0; device_id < devices_count; device_id++) {
        PyObject *device = PyList_GetItem(self->devices, device_id);
        Py_INCREF(device);
        PyList_SetItem(devices, device_id, device);
    }
    return devices;
}

int HashcatBackendPlatform_SetDevices(HashcatBackendPlatform *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify devices attribute");
    return -1;
}

PyObject *HashcatBackendPlatform_Repr(HashcatBackendPlatform *self) {
    PyObject *id = HashcatBackendPlatform_GetId(self);
    PyObject *name = HashcatBackendPlatform_GetName(self);
    PyObject *vendor = HashcatBackendPlatform_GetVendor(self);
    PyObject *devices = HashcatBackendPlatform_GetDevices(self);
    PyObject *repr = PyUnicode_FromFormat("Platform(id=%R, name=%R, vendor=%R, devices=%R)", id, name, vendor, devices);
    Py_DECREF(id);
    Py_DECREF(name);
    Py_DECREF(vendor);
    Py_DECREF(devices);
    return repr;
}

void HashcatBackendPlatform_Del(HashcatBackendPlatform *self) {
    Py_DECREF(self->hashcat);
    free(self->vendor);
    free(self->name);
    Py_DECREF(self->devices);
    PyObject_Del(self);
}

PyMethodDef HashcatBackendPlatform_Methods[] = {
    {"benchmark", (PyCFunction) HashcatBackendPlatform_Benchmark, METH_VARARGS, /* HashcatBackendPlatform_DocBenchmark */ NULL},
    {NULL},
};

PyGetSetDef HashcatBackendPlatform_GetSet[] = {
    {"id", (getter) HashcatBackendPlatform_GetId, (setter) HashcatBackendPlatform_SetId, /* HashcatBackendPlatform_DocId */ NULL, NULL},
    {"name", (getter) HashcatBackendPlatform_GetName, (setter) HashcatBackendPlatform_SetName, /* HashcatBackendPlatform_DocName */ NULL, NULL},
    {"vendor", (getter) HashcatBackendPlatform_GetVendor, (setter) HashcatBackendPlatform_SetVendor, /* HashcatBackendPlatform_DocVendor */ NULL, NULL},
    {"devices", (getter) HashcatBackendPlatform_GetDevices, (setter) HashcatBackendPlatform_SetDevices, /* HashcatBackendPlatform_DocDevices */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackendPlatform_DocType, ""); // todo

PyTypeObject HashcatBackendPlatform_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)             /* ob_size */
    "pyhashcat.backend.Platform",              /* tp_name */
    sizeof(HashcatBackendPlatform),            /* tp_basicsize */
    NULL,                                      /* tp_itemsize */
    (destructor) HashcatBackendPlatform_Del,   /* tp_dealloc */
    NULL,                                      /* tp_print */
    NULL,                                      /* tp_getattr */
    NULL,                                      /* tp_setattr */
    NULL,                                      /* tp_compare */
    HashcatBackendPlatform_Repr,               /* tp_repr */
    NULL,                                      /* tp_as_number */
    NULL,                                      /* tp_as_sequence */
    NULL,                                      /* tp_as_mapping */
    NULL,                                      /* tp_hash */
    NULL,                                      /* tp_call */
    NULL,                                      /* tp_str */
    NULL,                                      /* tp_getattro */
    NULL,                                      /* tp_setattro */
    NULL,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                        /* tp_flags */
    /* HashcatBackendPlatform_DocType */ NULL, /* tp_doc */
    NULL,                                      /* tp_traverse */
    NULL,                                      /* tp_clear */
    NULL,                                      /* tp_richcompare */
    NULL,                                      /* tp_weaklistoffset */
    NULL,                                      /* tp_iter */
    NULL,                                      /* tp_iternext */
    HashcatBackendPlatform_Methods,            /* tp_methods */
    NULL,                                      /* tp_members */
    HashcatBackendPlatform_GetSet,             /* tp_getset */
    NULL,                                      /* tp_base */
    NULL,                                      /* tp_dict */
    NULL,                                      /* tp_descr_get */
    NULL,                                      /* tp_descr_set */
    NULL,                                      /* tp_dictoffset */
    NULL,                                      /* tp_init */
    NULL,                                      /* tp_alloc */
    NULL,                                      /* tp_new */
};

// BENCHMARK RESULT

HashcatBackendBenchmarkResult *HashcatBackendBenchmarkResult_New(HashcatBackendBenchmarkResult *self, HashcatHash *hash, double hashrate, const char *_hashrate) {
    if (self == NULL) {
        return NULL;
    }
    self->hash = hash;
    self->hashrate = hashrate;
    const char suffix[] = "H/s";
    self->_hashrate = (char *) calloc(strlen(_hashrate) + strlen(suffix) + 1, sizeof(char));
    if (self->_hashrate == NULL) {
        // todo
    }
    strcpy(self->_hashrate, _hashrate);
    strcpy(self->_hashrate + strlen(_hashrate), suffix);
    return self;
}

// PyDoc_STRVAR(HashcatBackendBenchmarkResult_DocHash, ""); // todo

PyObject *HashcatBackendBenchmarkResult_GetHash(HashcatBackendBenchmarkResult *self) {
    PyObject *hash = self->hash;
    Py_INCREF(hash);
    return hash;
}

int HashcatBackendBenchmarkResult_SetHash(HashcatBackendBenchmarkResult *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify hash attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendBenchmarkResult_DocHashrate, ""); // todo

PyObject *HashcatBackendBenchmarkResult_GetHashrate(HashcatBackendBenchmarkResult *self) {
    PyObject *hashes = PyFloat_FromDouble(self->hashrate);
    return hashes;
}

int HashcatBackendBenchmarkResult_SetHashrate(HashcatBackendBenchmarkResult *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify hashrate attribute");
    return -1;
}

void HashcatBackendBenchmarkResult_Del(HashcatBackendBenchmarkResult *self) {
    Py_DECREF(self->hash);
    free(self->_hashrate);
    PyObject_Del(self);
}

PyGetSetDef HashcatBackendBenchmarkResult_GetSet[] = {
    {"hash", (getter) HashcatBackendBenchmarkResult_GetHash, (setter) HashcatBackendBenchmarkResult_SetHash, /* HashcatBackendBenchmarkResult_DocHash */ NULL, NULL},
    {"hashrate", (getter) HashcatBackendBenchmarkResult_GetHashrate, (setter) HashcatBackendBenchmarkResult_SetHashrate, /* HashcatBackendBenchmarkResult_DocHashrate */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackendBenchmarkResult_DocType, ""); // todo

PyTypeObject HashcatBackendBenchmarkResult_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)                    /* ob_size */
    "pyhashcat.backend.BenchmarkResult",              /* tp_name */
    sizeof(HashcatBackendBenchmarkResult),            /* tp_basicsize */
    NULL,                                             /* tp_itemsize */
    (destructor) HashcatBackendBenchmarkResult_Del,   /* tp_dealloc */
    NULL,                                             /* tp_print */
    NULL,                                             /* tp_getattr */
    NULL,                                             /* tp_setattr */
    NULL,                                             /* tp_compare */
    NULL,                                             /* tp_repr */
    NULL,                                             /* tp_as_number */
    NULL,                                             /* tp_as_sequence */
    NULL,                                             /* tp_as_mapping */
    NULL,                                             /* tp_hash */
    NULL,                                             /* tp_call */
    NULL,                                             /* tp_str */
    NULL,                                             /* tp_getattro */
    NULL,                                             /* tp_setattro */
    NULL,                                             /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                               /* tp_flags */
    /* HashcatBackendBenchmarkResult_DocType */ NULL, /* tp_doc */
    NULL,                                             /* tp_traverse */
    NULL,                                             /* tp_clear */
    NULL,                                             /* tp_richcompare */
    NULL,                                             /* tp_weaklistoffset */
    NULL,                                             /* tp_iter */
    NULL,                                             /* tp_iternext */
    NULL,                                             /* tp_methods */
    NULL,                                             /* tp_members */
    HashcatBackendBenchmarkResult_GetSet,             /* tp_getset */
    NULL,                                             /* tp_base */
    NULL,                                             /* tp_dict */
    NULL,                                             /* tp_descr_get */
    NULL,                                             /* tp_descr_set */
    NULL,                                             /* tp_dictoffset */
    NULL,                                             /* tp_init */
    NULL,                                             /* tp_alloc */
    NULL,                                             /* tp_new */
};

// DEVICE BENCHMARK RESULT

HashcatBackendDeviceBenchmarkResult *HashcatBackendDeviceBenchmarkResult_New(HashcatBackendDevice *device, HashcatHash *hash, double hashrate, const char *_hashrate) {
    HashcatBackendDeviceBenchmarkResult *self = PyObject_New(HashcatBackendDeviceBenchmarkResult, &HashcatBackendDeviceBenchmarkResult_Type);
    self = HashcatBackendBenchmarkResult_New((HashcatBackendBenchmarkResult *) self, hash, hashrate, _hashrate);
    if (self == NULL) {
        return NULL;
    }
    self->device = device;
    return self;
}

// PyDoc_STRVAR(HashcatBackendDeviceBenchmarkResult_DocDevice, ""); // todo

PyObject *HashcatBackendDeviceBenchmarkResult_GetDevice(HashcatBackendDeviceBenchmarkResult *self) {
    PyObject *device = self->device;
    Py_INCREF(device);
    return device;
}

int HashcatBackendDeviceBenchmarkResult_SetDevice(HashcatBackendDeviceBenchmarkResult *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify device attribute");
    return -1;
}

PyObject *HashcatBackendDeviceBenchmarkResult_Repr(HashcatBackendDeviceBenchmarkResult *self) {
    PyObject *device = HashcatBackendDeviceBenchmarkResult_GetDevice(self);
    PyObject *hash = HashcatBackendBenchmarkResult_GetHash(self);
    PyObject *hashrate = PyUnicode_FromString(self->_hashrate);
    PyObject *repr = PyUnicode_FromFormat("BencharkResult(device=%R, hash=%R, hashrate=%R)", device, hash, hashrate);
    Py_DECREF(device);
    Py_DECREF(hash);
    Py_DECREF(hashrate);
    return repr;
}

void HashcatBackendDeviceBenchmarkResult_Del(HashcatBackendDeviceBenchmarkResult *self) {
    Py_DECREF(self->device);
    HashcatBackendBenchmarkResult_Del((HashcatBackendBenchmarkResult *) self);
}

PyGetSetDef HashcatBackendDeviceBenchmarkResult_GetSet[] = {
    {"device", (getter) HashcatBackendDeviceBenchmarkResult_GetDevice, (setter) HashcatBackendDeviceBenchmarkResult_SetDevice, /* HashcatBackendDeviceBenchmarkResult_DocDevice */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackendDeviceBenchmarkResult_DocType, ""); // todo

PyTypeObject HashcatBackendDeviceBenchmarkResult_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)                          /* ob_size */
    "pyhashcat.backend.Device.BenchmarkResult",             /* tp_name */
    sizeof(HashcatBackendDeviceBenchmarkResult),            /* tp_basicsize */
    NULL,                                                   /* tp_itemsize */
    (destructor) HashcatBackendDeviceBenchmarkResult_Del,   /* tp_dealloc */
    NULL,                                                   /* tp_print */
    NULL,                                                   /* tp_getattr */
    NULL,                                                   /* tp_setattr */
    NULL,                                                   /* tp_compare */
    HashcatBackendDeviceBenchmarkResult_Repr,               /* tp_repr */
    NULL,                                                   /* tp_as_number */
    NULL,                                                   /* tp_as_sequence */
    NULL,                                                   /* tp_as_mapping */
    NULL,                                                   /* tp_hash */
    NULL,                                                   /* tp_call */
    NULL,                                                   /* tp_str */
    NULL,                                                   /* tp_getattro */
    NULL,                                                   /* tp_setattro */
    NULL,                                                   /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                     /* tp_flags */
    /* HashcatBackendDeviceBenchmarkResult_DocType */ NULL, /* tp_doc */
    NULL,                                                   /* tp_traverse */
    NULL,                                                   /* tp_clear */
    NULL,                                                   /* tp_richcompare */
    NULL,                                                   /* tp_weaklistoffset */
    NULL,                                                   /* tp_iter */
    NULL,                                                   /* tp_iternext */
    NULL,                                                   /* tp_methods */
    NULL,                                                   /* tp_members */
    HashcatBackendDeviceBenchmarkResult_GetSet,             /* tp_getset */
    &HashcatBackendBenchmarkResult_Type,                    /* tp_base */
    NULL,                                                   /* tp_dict */
    NULL,                                                   /* tp_descr_get */
    NULL,                                                   /* tp_descr_set */
    NULL,                                                   /* tp_dictoffset */
    NULL,                                                   /* tp_init */
    NULL,                                                   /* tp_alloc */
    NULL,                                                   /* tp_new */
};

// PLATFORM BENCHMARK RESULT

HashcatBackendPlatformBenchmarkResult *HashcatBackendPlatformBenchmarkResult_New(HashcatBackendPlatform *platform, HashcatHash *hash, double hashrate, const char *_hashrate, PyObject *devices) {
    HashcatBackendPlatformBenchmarkResult *self = PyObject_New(HashcatBackendPlatformBenchmarkResult, &HashcatBackendPlatformBenchmarkResult_Type);
    self = HashcatBackendBenchmarkResult_New((HashcatBackendBenchmarkResult *) self, hash, hashrate, _hashrate);
    if (self == NULL) {
        return NULL;
    }
    self->platform = platform;
    self->devices = devices;
    return self;
}

// PyDoc_STRVAR(HashcatBackendPlatformBenchmarkResult_DocDevices, ""); // todo

PyObject *HashcatBackendPlatformBenchmarkResult_GetDevices(HashcatBackendPlatformBenchmarkResult *self) {
    const unsigned int devices_count = PyList_Size(self->devices);
    PyObject *devices = PyList_New(devices_count);
    for (unsigned int device_id = 0; device_id < devices_count; device_id++) {
        PyObject *device = PyList_GetItem(self->devices, device_id);
        Py_INCREF(device);
        PyList_SetItem(devices, device_id, device);
    }
    return devices;
}

int HashcatBackendPlatformBenchmarkResult_SetDevices(HashcatBackendPlatformBenchmarkResult *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify devices attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackendPlatformBenchmarkResult_DocPlatform, ""); // todo

PyObject *HashcatBackendPlatformBenchmarkResult_GetPlatform(HashcatBackendPlatformBenchmarkResult *self) {
    PyObject *platform = self->platform;
    Py_INCREF(platform);
    return platform;
}

int HashcatBackendPlatformBenchmarkResult_SetPlatform(HashcatBackendPlatformBenchmarkResult *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify platform attribute");
    return -1;
}

PyObject *HashcatBackendPlatformBenchmarkResult_Repr(HashcatBackendPlatformBenchmarkResult *self) {
    PyObject *platform = HashcatBackendPlatformBenchmarkResult_GetPlatform(self);
    PyObject *hash = HashcatBackendBenchmarkResult_GetHash(self);
    PyObject *hashrate = PyUnicode_FromString(self->_hashrate);
    PyObject *devices = HashcatBackendPlatformBenchmarkResult_GetDevices(self);
    PyObject *repr = PyUnicode_FromFormat("BencharkResult(platform=%R, hash=%R, hashrate=%R, devices=%R)", platform, hash, hashrate, devices);
    Py_DECREF(platform);
    Py_DECREF(hash);
    Py_DECREF(hashrate);
    Py_DECREF(devices);
    return repr;
}

void HashcatBackendPlatformBenchmarkResult_Del(HashcatBackendPlatformBenchmarkResult *self) {
    Py_DECREF(self->devices);
    Py_DECREF(self->platform);
    HashcatBackendBenchmarkResult_Del((HashcatBackendBenchmarkResult *) self);
}

PyGetSetDef HashcatBackendPlatformBenchmarkResult_GetSet[] = {
    {"devices", (getter) HashcatBackendPlatformBenchmarkResult_GetDevices, (setter) HashcatBackendPlatformBenchmarkResult_SetDevices, /* HashcatBackendPlatformBenchmarkResult_DocDevices */ NULL, NULL},
    {"platform", (getter) HashcatBackendPlatformBenchmarkResult_GetPlatform, (setter) HashcatBackendPlatformBenchmarkResult_SetPlatform, /* HashcatBackendPlatformBenchmarkResult_DocPlatform */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackendPlatformBenchmarkResult_DocType, ""); // todo

PyTypeObject HashcatBackendPlatformBenchmarkResult_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)                            /* ob_size */
    "pyhashcat.backend.Platform.BenchmarkResult",             /* tp_name */
    sizeof(HashcatBackendPlatformBenchmarkResult),            /* tp_basicsize */
    NULL,                                                     /* tp_itemsize */
    (destructor) HashcatBackendPlatformBenchmarkResult_Del,   /* tp_dealloc */
    NULL,                                                     /* tp_print */
    NULL,                                                     /* tp_getattr */
    NULL,                                                     /* tp_setattr */
    NULL,                                                     /* tp_compare */
    HashcatBackendPlatformBenchmarkResult_Repr,               /* tp_repr */
    NULL,                                                     /* tp_as_number */
    NULL,                                                     /* tp_as_sequence */
    NULL,                                                     /* tp_as_mapping */
    NULL,                                                     /* tp_hash */
    NULL,                                                     /* tp_call */
    NULL,                                                     /* tp_str */
    NULL,                                                     /* tp_getattro */
    NULL,                                                     /* tp_setattro */
    NULL,                                                     /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                       /* tp_flags */
    /* HashcatBackendPlatformBenchmarkResult_DocType */ NULL, /* tp_doc */
    NULL,                                                     /* tp_traverse */
    NULL,                                                     /* tp_clear */
    NULL,                                                     /* tp_richcompare */
    NULL,                                                     /* tp_weaklistoffset */
    NULL,                                                     /* tp_iter */
    NULL,                                                     /* tp_iternext */
    NULL,                                                     /* tp_methods */
    NULL,                                                     /* tp_members */
    HashcatBackendPlatformBenchmarkResult_GetSet,             /* tp_getset */
    &HashcatBackendBenchmarkResult_Type,                      /* tp_base */
    NULL,                                                     /* tp_dict */
    NULL,                                                     /* tp_descr_get */
    NULL,                                                     /* tp_descr_set */
    NULL,                                                     /* tp_dictoffset */
    NULL,                                                     /* tp_init */
    NULL,                                                     /* tp_alloc */
    NULL,                                                     /* tp_new */
};

// BACKEND

HashcatBackend *HashcatBackend_New(Hashcat *hashcat) {
    HashcatBackend *self = PyObject_New(HashcatBackend, &HashcatBackend_Type);
    if (self == NULL) {
        return NULL;
    }
    self->hashcat = hashcat;
    return self;
}

// PyDoc_STRVAR(HashcatBackend_DocDevices, ""); // todo

PyObject *HashcatBackend_GetDevices(HashcatBackend *self) {
    unsigned int total_devices_count = 0;

    PyObject *platforms = HashcatBackend_GetPlatforms(self);
    const unsigned int platforms_count = PyList_Size(platforms);
    for (unsigned int platform_id = 0; platform_id < platforms_count; platform_id++) {
        const HashcatBackendPlatform *platform = PyList_GetItem(platforms, platform_id);
        const unsigned int devices_count = PyList_Size(platform->devices);
        total_devices_count += devices_count;
    }

    PyObject *devices = PyList_New(total_devices_count);

    for (unsigned int platform_id = 0, total_device_id = 0; platform_id < platforms_count; platform_id++) {
        HashcatBackendPlatform *platform = PyList_GetItem(platforms, platform_id);
        const unsigned int devices_count = PyList_Size(platform->devices);
        for (unsigned int device_id = 0; device_id < devices_count; device_id++, total_device_id++) {
            HashcatBackendDevice *device = (HashcatBackendDevice *) PyList_GetItem(platform->devices, device_id);
            Py_INCREF(device);
            PyList_SetItem(devices, total_device_id, (PyObject *) device);
        }
    }
    Py_DECREF(platforms);

    return devices;
}

int HashcatBackend_SetDevices(HashcatBackend *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify devices attribute");
    return -1;
}

// PyDoc_STRVAR(HashcatBackend_DocPlatforms, ""); // todo

PyObject *HashcatBackend_GetPlatforms(HashcatBackend *self) {
    const Hashcat *hashcat = self->hashcat;
    hashcat_ctx_t *hashcat_ctx = hashcat->ctx;
    user_options_t *user_options = hashcat_ctx->user_options;

    HashcatSession *session = hashcat->session;
    Py_DECREF(HashcatSession_Initialize(session, NULL));

    const backend_ctx_t *backend_ctx = hashcat_ctx->backend_ctx;
    const hc_device_param_t *devices_param = backend_ctx->devices_param;
    PyObject *platforms = PyList_New(backend_ctx->opencl_platforms_cnt);
    for (unsigned int platform_id = 0; platform_id < backend_ctx->opencl_platforms_cnt; platform_id++) {
        const char *platform_name = backend_ctx->opencl_platforms_name[platform_id];
        const char *platform_vendor = backend_ctx->opencl_platforms_vendor[platform_id];
        const unsigned int platform_devices_count = backend_ctx->opencl_platforms_devices_cnt[platform_id];

        PyObject *devices = PyList_New(platform_devices_count);
        for (unsigned int device_id = 0; device_id < platform_devices_count; device_id++) {
            const unsigned int device_global_id = backend_ctx->backend_device_from_opencl_platform[platform_id][device_id];
            const hc_device_param_t *device_param = &devices_param[device_global_id];
            const char *device_name = device_param->device_name;
            const char *device_vendor = device_param->opencl_device_vendor;
            HashcatBackendDeviceType device_type;
            switch (device_param->opencl_device_type) {
            case CL_DEVICE_TYPE_CPU:
                device_type = HashcatBackendDeviceType_CPU;
                break;
            case CL_DEVICE_TYPE_GPU:
                device_type = HashcatBackendDeviceType_GPU;
                break;
            default:
                device_type = HashcatBackendDeviceType_OTHER;
            }

            Py_INCREF(hashcat);
            const HashcatBackendDevice *device = HashcatBackendDevice_New(hashcat, device_id, device_global_id + 1, device_type, device_name, device_vendor);
            PyList_SetItem(devices, device_id, (PyObject *) device);
        }

        Py_INCREF(hashcat);
        const HashcatBackendPlatform *platform = HashcatBackendPlatform_New(hashcat, platform_id, platform_name, platform_vendor, devices);
        PyList_SetItem(platforms, platform_id, (PyObject *) platform);
    }

    Py_DECREF(HashcatSession_Destroy(session, NULL));

    return platforms;
}

int HashcatBackend_SetPlatforms(HashcatBackend *self, PyObject *value, void *closure) {
    PyErr_SetString(PyExc_AttributeError, "Cannot modify platforms attribute");
    return -1;
}

PyObject *HashcatBackend_Repr(HashcatBackend *self) {
    PyObject *repr = PyUnicode_FromString("Backend()");
    return repr;
}

void HashcatBackend_Del(HashcatBackend *self) {
    Py_DECREF(self->hashcat);
    PyObject_Del(self);
}

PyGetSetDef HashcatBackend_GetSet[] = {
    {"devices", (getter) HashcatBackend_GetDevices, (setter) HashcatBackend_SetDevices, /* HashcatBackend_DocDevices */ NULL, NULL},
    {"platforms", (getter) HashcatBackend_GetPlatforms, (setter) HashcatBackend_SetPlatforms, /* HashcatBackend_DocPlatforms */ NULL, NULL},
    {NULL},
};

// PyDoc_STRVAR(HashcatBackend_DocType, ""); // todo

PyTypeObject HashcatBackend_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)     /* ob_size */
    "pyhashcat.backend.Backend",       /* tp_name */
    sizeof(HashcatBackend),            /* tp_basicsize */
    NULL,                              /* tp_itemsize */
    (destructor) HashcatBackend_Del,   /* tp_dealloc */
    NULL,                              /* tp_print */
    NULL,                              /* tp_getattr */
    NULL,                              /* tp_setattr */
    NULL,                              /* tp_compare */
    HashcatBackend_Repr,               /* tp_repr */
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
    /* HashcatBackend_DocType */ NULL, /* tp_doc */
    NULL,                              /* tp_traverse */
    NULL,                              /* tp_clear */
    NULL,                              /* tp_richcompare */
    NULL,                              /* tp_weaklistoffset */
    NULL,                              /* tp_iter */
    NULL,                              /* tp_iternext */
    NULL,                              /* tp_methods */
    NULL,                              /* tp_members */
    HashcatBackend_GetSet,             /* tp_getset */
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

// PyDoc_STRVAR(HashcatBackend_DocModule, ""); // todo

struct PyModuleDef HashcatBackend_Module = {
    PyModuleDef_HEAD_INIT,
    "pyhashcat.backend",                 /* m_name */
    /* HashcatBackend_DocModule */ NULL, /* m_doc */
    -1,                                  /* m_size */
    NULL,                                /* m_methods */
    NULL,                                /* m_reload */
    NULL,                                /* m_traverse */
    NULL,                                /* m_clear */
    NULL,                                /* m_free */
};

PyMODINIT_FUNC PyInit_HashcatBackend(void) {
    PyObject *module = PyModule_Create(&HashcatBackend_Module);
    if (module == NULL) {
        return NULL;
    }

    if (PyType_Ready(&HashcatBackendBenchmarkResult_Type) < 0) {
        return NULL; // todo
    }
    PyModule_AddObject(module, "BenchmarkResult", (PyObject *) &HashcatBackendBenchmarkResult_Type);

    if (PyType_Ready(&HashcatBackendDeviceBenchmarkResult_Type) < 0) {
        return NULL; // todo
    }

    if (PyType_Ready(&HashcatBackendPlatformBenchmarkResult_Type) < 0) {
        return NULL; // todo
    }

    if (PyType_Ready(&HashcatBackendDevice_Type) < 0) {
        return NULL; // todo
    }
    PyDict_SetItemString(HashcatBackendDevice_Type.tp_dict, "BenchmarkResult", (PyObject *) &HashcatBackendDeviceBenchmarkResult_Type);
    PyModule_AddObject(module, "Device", (PyObject *) &HashcatBackendDevice_Type);

    if (PyType_Ready(&HashcatBackendPlatform_Type) < 0) {
        return NULL; // todo
    }
    PyDict_SetItemString(HashcatBackendPlatform_Type.tp_dict, "BenchmarkResult", (PyObject *) &HashcatBackendPlatformBenchmarkResult_Type);
    PyModule_AddObject(module, "Platform", (PyObject *) &HashcatBackendPlatform_Type);

    if (PyType_Ready(&HashcatBackend_Type) < 0) {
        return NULL; // todo
    }
    PyModule_AddObject(module, "Backend", (PyObject *) &HashcatBackend_Type);

    return module;
}
