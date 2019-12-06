#include <Python.h>
#include <math.h>

double calcSD(double data[], int size)
{
    int i;
    double sum = 0.0, mean, StandardDeviation = 0.0;

    for (i = 0; i < size; ++i)
    {
        sum += data[i];
    }

    mean = sum / size;
    for (i = 0; i < size; ++i)
    {
        StandardDeviation += pow(data[i] - mean, 2);
    }
    return sqrt(StandardDeviation / (size - 1));
}

static PyObject *std_standard_dev(PyObject *self, PyObject *args)
{
    PyObject *float_list;
    int pr_length;
    double *pr;

    if (!PyArg_ParseTuple(args, "O", &float_list))
        return NULL;
    pr_length = PyObject_Length(float_list);
    if (pr_length < 0)
        return NULL;
    pr = (double *)malloc(sizeof(double *) * pr_length);
    if (pr == NULL)
        return NULL;
    for (int index = 0; index < pr_length; index++)
    {
        PyObject *key = PyLong_FromLong(index);
        PyObject *item = PyObject_GetItem(float_list, key);
        pr[index] = PyFloat_AsDouble(item);
        Py_DECREF(item);
        Py_DECREF(key);
    }
    return PyFloat_FromDouble(calcSD(pr, pr_length));
}

static PyObject *return_listC(PyObject *self, PyObject *args)
{
    PyObject *float_list;
    int pr_length;
    double *pr;

    if (!PyArg_ParseTuple(args, "O", &float_list))
        return NULL;
    pr_length = PyObject_Length(float_list);
    if (pr_length < 0)
        return NULL;
    pr = (double *)malloc(sizeof(double *) * pr_length);
    if (pr == NULL)
        return NULL;
    for (int index = 0; index < pr_length; index++)
    {
        PyObject *key = PyLong_FromLong(index);
        PyObject *item = PyObject_GetItem(float_list, key);
        pr[index] = PyFloat_AsDouble(item);
        Py_DECREF(item);
        Py_DECREF(key);
    }

    PyObject *new_list = PyList_New(pr_length);
    for (int index = 0; index < pr_length; index++)
        PyList_SetItem(new_list, index, Py_BuildValue("d", pr[index]));

    return new_list;
}

static PyMethodDef std_methods[] = {
    {"standard_dev", std_standard_dev, METH_VARARGS,
     "Return the standard deviation of a list."},
    {"return_list", return_listC, METH_VARARGS,
     "Return the same list input"},
    {NULL, NULL, NULL} /* sentinel */
};

static struct PyModuleDef stdmodule = {
    PyModuleDef_HEAD_INIT,
    "std", /* name of module */
    NULL,  /* module documentation, may be NULL */
    -1,
    std_methods};

PyMODINIT_FUNC PyInit_std(void)
{
    return PyModule_Create(&stdmodule);
}

int main(int argc, char **argv)
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL)
    {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("std", PyInit_std);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    PyMem_RawFree(program);
    return 0;
}