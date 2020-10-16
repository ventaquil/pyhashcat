#ifndef PYHASHCAT_BACKEND_H
#define PYHASHCAT_BACKEND_H

#include <Python.h>

#include "hash.h"
#include "hashcat.h"

// DEVICE TYPE

typedef enum {
    HashcatBackendDeviceType_CPU = 1,
    HashcatBackendDeviceType_GPU = 2,
    HashcatBackendDeviceType_OTHER = 3,
} HashcatBackendDeviceType;

// DEVICE

typedef struct {
    PyObject_HEAD

    Hashcat *hashcat;

    unsigned int id;
    unsigned int global_id;
    HashcatBackendDeviceType type;
    char *name;
    char *vendor;
} HashcatBackendDevice;

PyTypeObject HashcatBackendDevice_Type;

#define HashcatBackendDevice_Check(v) (Py_TYPE(v) == &HashcatBackendDevice_Type)

HashcatBackendDevice *HashcatBackendDevice_New(Hashcat *hashcat, unsigned int id, unsigned int global_id, HashcatBackendDeviceType type, const char *name, const char *vendor);

PyObject *HashcatBackendDevice_Benchmark(HashcatBackendDevice *self, PyObject *args);

PyObject *HashcatBackendDevice_GetId(HashcatBackendDevice *self);

int HashcatBackendDevice_SetId(HashcatBackendDevice *self, PyObject *value, void *closure);

PyObject *HashcatBackendDevice_GetGlobalId(HashcatBackendDevice *self);

int HashcatBackendDevice_SetGlobalId(HashcatBackendDevice *self, PyObject *value, void *closure);

PyObject *HashcatBackendDevice_GetType(HashcatBackendDevice *self);

int HashcatBackendDevice_SetType(HashcatBackendDevice *self, PyObject *value, void *closure);

PyObject *HashcatBackendDevice_GetName(HashcatBackendDevice *self);

int HashcatBackendDevice_SetName(HashcatBackendDevice *self, PyObject *value, void *closure);

PyObject *HashcatBackendDevice_GetVendor(HashcatBackendDevice *self);

int HashcatBackendDevice_SetVendor(HashcatBackendDevice *self, PyObject *value, void *closure);

PyObject *HashcatBackendDevice_Repr(HashcatBackendDevice *self);

void HashcatBackendDevice_Del(HashcatBackendDevice *self);

// PLATFORM

typedef struct {
    PyObject_HEAD

    Hashcat *hashcat;

    unsigned int id;
    char *name;
    char *vendor;
    PyObject *devices;
} HashcatBackendPlatform;

PyTypeObject HashcatBackendPlatform_Type;

#define HashcatBackendPlatform_Check(v) (Py_TYPE(v) == &HashcatBackendPlatform_Type)

HashcatBackendPlatform *HashcatBackendPlatform_New(Hashcat *hashcat, unsigned int id, const char *name, const char *vendor, PyObject *devices);

PyObject *HashcatBackendPlatform_Benchmark(HashcatBackendPlatform *self, PyObject *args);

PyObject *HashcatBackendPlatform_GetId(HashcatBackendPlatform *self);

int HashcatBackendPlatform_SetId(HashcatBackendPlatform *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatform_GetName(HashcatBackendPlatform *self);

int HashcatBackendPlatform_SetName(HashcatBackendPlatform *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatform_GetVendor(HashcatBackendPlatform *self);

int HashcatBackendPlatform_SetVendor(HashcatBackendPlatform *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatform_GetDevices(HashcatBackendPlatform *self);

int HashcatBackendPlatform_SetDevices(HashcatBackendPlatform *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatform_Repr(HashcatBackendPlatform *self);

void HashcatBackendPlatform_Del(HashcatBackendPlatform *self);

// BENCHMARK RESULT

#define HashcatBackendBenchmarkResult_HEAD \
    HashcatHash *hash;                     \
    double hashrate;                       \
    char *_hashrate;

typedef struct {
    PyObject_HEAD

    HashcatBackendBenchmarkResult_HEAD
} HashcatBackendBenchmarkResult;

PyTypeObject HashcatBackendBenchmarkResult_Type;

#define HashcatBackendBenchmarkResult_Check(v) (Py_TYPE(v) == &HashcatBackendBenchmarkResult_Type)

HashcatBackendBenchmarkResult *HashcatBackendBenchmarkResult_New(HashcatBackendBenchmarkResult *self, HashcatHash *hash, double hashrate, const char *_hashrate);

PyObject *HashcatBackendBenchmarkResult_GetHash(HashcatBackendBenchmarkResult *self);

int HashcatBackendBenchmarkResult_SetHash(HashcatBackendBenchmarkResult *self, PyObject *value, void *closure);

PyObject *HashcatBackendBenchmarkResult_GetHashrate(HashcatBackendBenchmarkResult *self);

int HashcatBackendBenchmarkResult_SetHashrate(HashcatBackendBenchmarkResult *self, PyObject *value, void *closure);

void HashcatBackendBenchmarkResult_Del(HashcatBackendBenchmarkResult *self);

// DEVICE BENCHMARK RESULT

typedef struct {
    PyObject_HEAD

    HashcatBackendBenchmarkResult_HEAD

    HashcatBackendDevice *device;
} HashcatBackendDeviceBenchmarkResult;

PyTypeObject HashcatBackendDeviceBenchmarkResult_Type;

#define HashcatBackendDeviceBenchmarkResult_Check(v) (Py_TYPE(v) == &HashcatBackendDeviceBenchmarkResult_Type)

HashcatBackendDeviceBenchmarkResult *HashcatBackendDeviceBenchmarkResult_New(HashcatBackendDevice *device, HashcatHash *hash, double hashrate, const char *_hashrate);

PyObject *HashcatBackendDeviceBenchmarkResult_GetDevice(HashcatBackendDeviceBenchmarkResult *self);

int HashcatBackendDeviceBenchmarkResult_SetDevice(HashcatBackendDeviceBenchmarkResult *self, PyObject *value, void *closure);

PyObject *HashcatBackendDeviceBenchmarkResult_Repr(HashcatBackendDeviceBenchmarkResult *self);

void HashcatBackendDeviceBenchmarkResult_Del(HashcatBackendDeviceBenchmarkResult *self);

// PLATFORM BENCHMARK RESULT

typedef struct {
    PyObject_HEAD

    HashcatBackendBenchmarkResult_HEAD

    HashcatBackendPlatform *platform;
    PyObject *devices;
} HashcatBackendPlatformBenchmarkResult;

PyTypeObject HashcatBackendPlatformBenchmarkResult_Type;

#define HashcatBackendPlatformBenchmarkResult_Check(v) (Py_TYPE(v) == &HashcatBackendPlatformBenchmarkResult_Type)

HashcatBackendPlatformBenchmarkResult *HashcatBackendPlatformBenchmarkResult_New(HashcatBackendPlatform *platform, HashcatHash *hash, double hashrate, const char *_hashrate, PyObject *devices);

PyObject *HashcatBackendPlatformBenchmarkResult_GetDevices(HashcatBackendPlatformBenchmarkResult *self);

int HashcatBackendPlatformBenchmarkResult_SetDevices(HashcatBackendPlatformBenchmarkResult *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatformBenchmarkResult_GetPlatform(HashcatBackendPlatformBenchmarkResult *self);

int HashcatBackendPlatformBenchmarkResult_SetPlatform(HashcatBackendPlatformBenchmarkResult *self, PyObject *value, void *closure);

PyObject *HashcatBackendPlatformBenchmarkResult_Repr(HashcatBackendPlatformBenchmarkResult *self);

void HashcatBackendPlatformBenchmarkResult_Del(HashcatBackendPlatformBenchmarkResult *self);

// BACKEND

typedef struct {
    PyObject_HEAD

    Hashcat *hashcat;
} HashcatBackend;

PyTypeObject HashcatBackend_Type;

#define HashcatBackend_Check(v) (Py_TYPE(v) == &HashcatBackend_Type)

HashcatBackend *HashcatBackend_New(Hashcat *hashcat);

PyObject *HashcatBackend_GetDevices(HashcatBackend *self);

int HashcatBackend_SetDevices(HashcatBackend *self, PyObject *value, void *closure);

PyObject *HashcatBackend_GetPlatforms(HashcatBackend *self);

int HashcatBackend_SetPlatforms(HashcatBackend *self, PyObject *value, void *closure);

PyObject *HashcatBackend_Repr(HashcatBackend *self);

void HashcatBackend_Del(HashcatBackend *self);

// MODULE

struct PyModuleDef HashcatBackend_Module;

PyMODINIT_FUNC PyInit_HashcatBackend(void);

#endif // PYHASHCAT_BACKEND_H
