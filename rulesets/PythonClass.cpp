// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2011 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#include <Python.h>

#include "PythonClass.h"

#include "rulesets/Python_Script_Utils.h"

#include "common/log.h"
#include "common/compose.hpp"

/// \brief ScriptKit constructor
/// 
/// @param package name of the script package scripts are to be created from
/// @param type name of the type instanced to create scripts
PythonClass::PythonClass(const std::string & package,
                         const std::string & type,
                         PyTypeObject * base) : m_package(package),
                                                m_type(type),
                                                m_base(base),
                                                m_module(0),
                                                m_class(0)
{
}

int PythonClass::load()
{
    m_module = Get_PyModule(m_package);
    if (m_module == NULL) {
        return -1;
    }
    return getClass(m_module);
}

PythonClass::~PythonClass()
{
    if (m_class != 0) {
        Py_DECREF(m_class);
    }
    if (m_module != 0) {
        Py_DECREF(m_module);
    }
}

/// \brief Retrieve the pythonclass object from the module which has
/// already been loaded.
int PythonClass::getClass(PyObject * module)
{
    PyObject * new_class = Get_PyClass(module, m_package, m_type);
    if (new_class == 0) {
        // If this is a new class, leave it as zero and fail and this
        // should be discarded by <*>RuleHandler
        // If this is an existing class, leave it as the old value, but
        // fail the update. This should signal back to the client.
        return -1;
    }
    if (!PyType_IsSubtype((PyTypeObject*)new_class, m_base)) {
        log(ERROR, String::compose("Python class \"%1.%2\" does not inherit "
                                   "from a core server type \"%3\".",
                                   m_package, m_type, m_base->tp_name));
        Py_DECREF(new_class);
        return -1;
    }
    // FIXME We own a reference to m_class. Need to decref
    if (m_class != 0) {
        log(NOTICE, "Reloading class");
        if (new_class != m_class) {
            log(WARNING, "Class pointer changes on reload");
        } else {
            log(WARNING, "Class pointer remains the same on reload");
        }
    }
    m_class = new_class;

    return 0;
}

int PythonClass::refresh()
{
    if (m_module == 0) {
        log(ERROR, "bort");
        return -1;
    }
    PyObject * new_module = PyImport_ReloadModule(m_module);
    if (new_module == 0) {
        log(ERROR, String::compose("Error reloading python module \"%1\"",
                                   m_package));
        PyErr_Clear();
        return -1;
    }
    int ret = getClass(new_module);
    if (ret != 0) {
        log(ERROR, String::compose("Error finding python class \"%1\" in \"%2\"",
                                   m_type, m_package));
        return -1;
        // After reloading, but failing to get the class, I think the old class
        // should still work, but is no longer bound to the name. It won't
        // be collected at least until we release the reference we hold
        // in m_class
    }
    printf("%p %p\n", m_module, new_module);
    Py_DECREF(m_module);
    // FIXME These are probably the same pointer - certainly mostly
    // We do need to decref though, as both calls to PyImport_...()
    // return new references
    if (m_module != new_module) {
        log(WARNING, "A reloaded module has change its pointer");
    } else {
        log(WARNING, "A reloaded module pointer stays the same");
    }
    m_module = new_module;
    return 0;
}
