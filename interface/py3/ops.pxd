import cython

from libcpp.string cimport string
from cpython.version cimport PY_MAJOR_VERSION





cdef inline unicode _text(s):
    if type(s) is unicode:
        return <unicode>s

    elif PY_MAJOR_VERSION < 3 and isinstance(s, bytes):
        return (<bytes>s).decode('ascii')
    
    elif isinstance(s, unicode):
        return unicode(s)
    
    else:
        raise TypeError("Could not convert to unicode.")

cdef inline string _string(basestring s) except *:
    cdef string c_str = _text(s).encode("utf-8")
    return c_str

