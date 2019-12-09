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
    // std::cout << vec_mean << std::endl;

    // Squared sum
    for (auto& element : vec)
        element = pow(element - vec_mean, 2);

    // Variance
    double vec_var = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();

    return sqrt(vec_var);
}

// int main()
// {
//     std::vector<double> new_vec = {1.0, 2.0, 3.0, 4.0, 5.0};
//     std::cout << standard_dev(new_vec) << std::endl;
//     return 0;
// }

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
