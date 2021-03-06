#include "render.h"

#ifdef PYTHON_RENDERING_SUPPORTED

void gen::render::drawMap(std::vector<char> &drawdata, std::string filename) {
	std::string drawModuleName = "render.rendermap";
    std::string drawFunctionName = "draw_map";

    Py_Initialize();

    std::string appendcmd = "sys.path.append('" + 
                             std::string(RESOURCES_EXECUTABLE_DIRECTORY) + "')";
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(appendcmd.c_str());

    PyObject *pModule = PyImport_ImportModule(drawModuleName.c_str());
    _checkPyObjectNotNull(pModule, "module import");

    PyObject *pModuleDict = PyModule_GetDict(pModule);
    _checkPyObjectNotNull(pModuleDict, "get module dict");

    PyObject *pFunctionName = _get_PyString_FromString(drawFunctionName.c_str());
    _checkPyObjectNotNull(pFunctionName, "function string");
    if (!PyDict_Contains(pModuleDict, pFunctionName)) {
        _checkPySuccess(-1, "function not in module dict");
    }

    PyObject *pFunction = PyDict_GetItem(pModuleDict, pFunctionName);
    Py_DECREF(pFunctionName);
    _checkPyObjectNotNull(pFunction, "get function from dict");
    if (!PyCallable_Check(pFunction)) {
        _checkPySuccess(-1, "function not callable");
    }

    PyObject *pDrawData = _get_PyString_FromString(drawdata.data());
    _checkPyObjectNotNull(pDrawData, "draw data string");

    PyObject *pFilename = _get_PyString_FromString(filename.c_str());
    _checkPyObjectNotNull(pDrawData, "draw data string");

    PyObject *pArgs = PyTuple_New(2);
    _checkPyObjectNotNull(pDrawData, "draw data string");
    if (PyTuple_SetItem(pArgs, 0, pDrawData) != 0) {
        _checkPySuccess(-1, "setting drawdata arg");
    }
    if (PyTuple_SetItem(pArgs, 1, pFilename) != 0) {
        _checkPySuccess(-1, "setting filename arg");
    }

    PyObject_CallObject(pFunction, pArgs);
    Py_DECREF(pArgs);

    if (PyErr_Occurred()) {
        _checkPySuccess(-1, "calling function");
    }

    Py_Finalize();
}

PyObject* gen::render::_get_PyString_FromString(const char *v) {
    #ifdef PYTHON_VERSION_2
    return PyString_FromString(v);
    #else
    return PyUnicode_FromString(v);
    #endif
}

void gen::render::_checkPyObjectNotNull(PyObject *obj, std::string err) {
    if (!obj) {
        std::cout << "Error: " << err << std::endl;
        PyErr_Print();
        exit(0);
    }
}

void gen::render::_checkPySuccess(int ret, std::string err) {
    if (ret == -1) {
        std::cout << "Error: " << err << std::endl;
        PyErr_Print();
        exit(0);
    }
}

#else

void gen::render::drawMap(std::vector<char>&, std::string) {}

#endif