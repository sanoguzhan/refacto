# cython: c_string_type=unicode, c_string_encoding=utf8
# distutils: language = c++

from parsing cimport ops
from libcpp cimport bool
from libcpp.map cimport map
from libcpp.vector cimport vector
from libcpp.string cimport string

from cython.operator cimport dereference as deref

cdef bint boolean_variable = True

cdef extern from "core/include/table.hpp" namespace "table":
    cdef cppclass Series:
        string name;
        map[string,vector[string]] values;


cdef extern from "core/include/table.hpp" namespace "table":
    cdef cppclass Table:
        bool save(string) except +;
        bool insert(Series) except +;
        bool insert(string, Series) except +;
        bool insert(string, vector[string]) except +;



cdef extern from "core/include/csvparser.hpp":
    cdef cppclass Loc:
        string name;
        string orient;
        int row;
        int column;



cdef extern from "core/include/csvparser.hpp":
    cdef cppclass CSVParser:
        CSVParser(string, string, int);
        CSVParser(string, string);
        CSVParser(string, int);
        CSVParser(string);
        Series values(string, int, Loc, Loc);
        Series values(string, int, Loc, Loc, Loc);
        Series values(string, int, int, int);
        string value(Loc);


cdef class _Table:

    cdef Table *thisptr 

    def __cinit__(self):
        self.thisptr = new Table()

    def to_csv(self, string path):
        return self.thisptr.save(ops._string(path))



    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr


cdef class _Location:
    cdef Loc *thisptr

    def __cinit__(self, str name ="", str orient="", int row=0, int column=0):
        self.thisptr = new Loc()
        if(name): self.thisptr.name = ops._string(name)
        if(orient): self.thisptr.orient = ops._string(orient)
        if(row): self.thisptr.row = row
        if(column): self.thisptr.column = column



cdef class _CSVParser:
    cdef CSVParser *thisptr 

    def __cinit__(self, string path, string delim, int skip):

        print(path)
        print(delim)
        print(skip)
        self.thisptr = new CSVParser(path, delim,skip)


    def __call__(self, dict kwargs):
        if(kwargs.get("conditions")):
            if(len(kwargs.get("conditions")) == 1):
                return self.thisptr.value(deref(_Location(
                    name="date",
                    orient="row",
                    row=11,
                    column=0
                ).thisptr))

    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr

cdef class Parser:
    cdef object obj
    cdef dict kwargs
    def __cinit__(self,):
        self.obj = None
        self.kwargs = dict()


    def assign(self, str _type, dict kwargs = None):
        if self.obj == None:
            if _type == "csvparser":
                kwargs = self.validate(kwargs)
                self.obj = _CSVParser(kwargs.get("path"),
                                              kwargs.get("delim"),
                                              kwargs.get("skip"))

                print("init csvparser")


    cdef validate(self,dict kwargs): 
        if not kwargs.get("path"):
            raise KeyError("Path is missing")
        if not kwargs.get("delim"):
            kwargs["delim"] = ";"
        if not kwargs.get("skip"):
            kwargs["skip"] = 0   
        return kwargs
        

# CC="gcc" CXX="g++" OPT="" CFLAGS="-std=gnu++17" BASECFLAGS="" LDFLAGS="" CCSHARED="" LDSHARED="gcc -shared" PY_CORE_CFLAGS="" PY_CFLAGS=""  SO="" python3 setup.py build_ext --inplace

# CC="gcc" CXX="g++" OPT="" CFLAGS="-std=gnu++17" BASECFLAGS="" LDFLAGS="" CCSHARED="" LDSHARED="gcc -shared" PY_CORE_CFLAGS="" PY_CFLAGS=""  SO="" python3 setup.py build_ext --inplace