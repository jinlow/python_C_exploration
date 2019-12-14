#include <Python.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <cmath>

double* uni_regC(std::vector<double> xvar, 
                 std::vector<double> yvar,
                 double eta,
                 int epochs)
{
    double b0 = 0;
    double b1 = 0;
    int nvars = yvar.size();
    int rounds = epochs * nvars;

    // Simple gradient descent, one epoch is a full pass through the data.
    for(int i = 0; i < rounds; i++) {
        int idx = i % nvars;
        double pred = b0 + b1 * xvar[idx];
        double err = pred - yvar[idx];
        b0 = b0 - eta * err;
        b1 = b1 - eta * err * xvar[idx];
    }
    
    double* preds = (double*) malloc(sizeof(double)*2);
    preds[0] = b0;
    preds[1] = b1;
    return preds;
}

static PyObject* uni_regPy(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject* x_list, *y_list;
    double eta = 0.05;
    int epochs = 50;

    static char const* kwlist[] = {"x_list", "y_list", "eta", "epochs", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|di", (char**)kwlist,
                                     &x_list, &y_list, &eta, &epochs))
        return NULL;

    // List sizes
    Py_ssize_t xsize = PyObject_Length(x_list);
    Py_ssize_t ysize = PyObject_Length(y_list);
    if ((xsize < 0) || (ysize < 0))
        return NULL;

    if (xsize != ysize) {
        PyErr_SetString(PyExc_ValueError, "x, and y, must all be the same length");
        return NULL;
    }

    if ((eta > 1) || (eta <= 0)) {
        PyErr_SetString(PyExc_ValueError, "eta must be greater than 0 or less than 1.");
        return NULL;
    }

    // Create vectors
    std::vector<double> xvar, yvar;
    xvar.resize(xsize);
    yvar.resize(ysize);

    // Convert pyvars to vectors
    for (int i = 0; i < ysize; i++) {
        PyObject* index = PyLong_FromLong(i);
        PyObject* xitem = PyObject_GetItem(x_list, index);
        PyObject* yitem = PyObject_GetItem(y_list, index);
        xvar[i] = PyFloat_AsDouble(xitem);
        yvar[i] = PyFloat_AsDouble(yitem);
        Py_DECREF(xitem);
        Py_DECREF(yitem);
        Py_DECREF(index);
    }

    double* preds = uni_regC(xvar, yvar, eta, epochs);

    if (isnan(preds[0]) || isnan(preds[1])) {
        PyErr_SetString(PyExc_ValueError, "function did not converge. \n Try setting a smaller eta value.");
        return NULL;
    }

    Py_ssize_t size = 2;
    PyObject* new_list = PyList_New(size);
    for (int index = 0; index < size; index++)
        PyList_SetItem(new_list, index, Py_BuildValue("d", preds[index]));

    free(preds);
    return new_list;
}

double standard_devC(std::vector<double> vec)
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

static PyObject* std_devPy(PyObject *self, PyObject *args)
{
    PyObject* input;
    if (!PyArg_ParseTuple(args, "O", &input))
        return NULL;

    Py_ssize_t size = PyObject_Length(input);
    if (size < 0)
        return NULL;

    std::vector<double> list;
    list.resize(size);

    for (int i = 0; i < size; i++) {
        PyObject* index = PyLong_FromLong(i);
        PyObject* item = PyObject_GetItem(input, index);
        list[i] = PyFloat_AsDouble(item);
        Py_DECREF(item);
        Py_DECREF(index);
    }
    return PyFloat_FromDouble(standard_devC(list));
}

static PyMethodDef module_methods[] = {
	{"stdev", (PyCFunction)std_devPy,	METH_VARARGS,
    "Standard deviation of iterable python object"}, 
    {"uni_reg", (PyCFunction)uni_regPy,	METH_VARARGS | METH_KEYWORDS,
    "Univariate regression with gradient decent."},
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
