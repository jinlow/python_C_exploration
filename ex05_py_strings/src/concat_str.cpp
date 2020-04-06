#include <Python.h>
#include <string>

std::string concat_strC(int num)
{
    std::string str;
    for (int i = 0; i < num; i++)
    {
        str.append(std::to_string(i));
    }
    return str;
}

static PyObject *concat_strPy(PyObject *self, PyObject *args)
{
    int numcs;
    if (!PyArg_ParseTuple(args, "i", &numcs))
        return NULL;

    std::string str;
    str = concat_strC(numcs);
    return PyUnicode_FromString(str.c_str());
}

static PyMethodDef module_methods[] = {
    {"concat_str", (PyCFunction)concat_strPy, METH_VARARGS,
     "Concat int of strings"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef stringCmodule = {
    PyModuleDef_HEAD_INIT,
    "stringC",                      /* name of module */
    "Working with strings with C.", /* module documentation, may be NULL */
    -1,
    module_methods};

PyMODINIT_FUNC PyInit_stringC(void)
{
    return PyModule_Create(&stringCmodule);
}
