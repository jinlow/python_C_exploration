#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <cmath>

double standard_dev(std::vector<double> vec)
{
    // Mean of vector
    double vec_sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    double vec_mean = vec_sum / vec.size();

    // Square each element
    for (auto& element : vec)
        element = pow(element - vec_mean, 2);

    // Variance
    double vec_var = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();

    return sqrt(vec_var);
}

double uni_regC(std::vector<double> xvar, 
               std::vector<double> yvar,
               double eta = 0.05,
               int epochs = 50)
{
    double b0 = 0;
    double b1 = 0;
    int nvars = yvar.size();
    int rounds = epochs * nvars;

    // Simple gradient descent, one epoch is 
    // a full pass through the data.
    for(int i = 0, i < rounds, i++) {
        int idx = i % nvars;
        double pred = b0 + b1 * x[idx];
        double err = p - y[idx];
        b0 = b0 - eta * err;
        b1 = b1 - eta * err * x[idx];
    }
    double preds[] = {b0, b1};
    reutn preds;
}

static PyObject *std_devPy(PyObject *self, PyObject *args)
{
    PyObject *input;
    if (!PyArg_ParseTuple(args, "O", &input))
        return NULL;

    Py_ssize_t size = PyObject_Length(input);
    if (size < 0)
        return NULL;

    std::vector<double> list;
    list.resize(size);

    for (int i = 0; i < size; i++) {
        PyObject *index = PyLong_FromLong(i);
        PyObject *item = PyObject_GetItem(input, index);
        list[i] = PyFloat_AsDouble(item);
        Py_DECREF(item);
        Py_DECREF(index);
    }
    return PyFloat_FromDouble(standard_dev(list));
}

static PyObject *uni_regPy(PyObject *self, PyObject *args, PyObject *kwargs)
{
    
}

static PyMethodDef module_methods[] = {
	{"stdev", (PyCFunction)std_devPy,	METH_VARARGS,
    "Standard deviation of iterable python object"}, 
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef sumCmodule = {
    PyModuleDef_HEAD_INIT,
    "sumC", /* name of module */
    "Simple module for computing summary statistics.", /* module documentation, may be NULL */
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_sumC(void)
{
    return PyModule_Create(&sumCmodule);
}
