# cython: c_string_type=unicode, c_string_encoding=utf8
# distutils: language = c++

cimport ops
from libcpp cimport bool
from libcpp.map cimport map
from libcpp.vector cimport vector
from libcpp.string cimport string
from cython.operator cimport dereference as deref

cdef bint boolean_variable = True

#### CSVParser ####
cdef extern from "refacto/controller.hpp":
     cpdef cppclass CustomParserWrapper:
         CustomParserWrapper(vector[map[string, string]], map[string,map[string,string]]) nogil except+;
         void save(string) nogil except+;
         void operator()(string, string, int) nogil except+;


cdef class CSVParser:
    """ Python Interface for CustomParserWrapper
        Overload operator() accepts dict"""
    cdef CustomParserWrapper* thisptr

    
    def __cinit__(self, list vals):
        cdef vector[map[string, string]] entity
        cdef map[string, map[string, string]] condition
        for i in vals:
            if (i["type"] == "series" or i["type"] == "group") and i.get("condition", False):
                temp = i["condition"]
                condition_name = i["key"] + i["name"]
                i["condition"] = condition_name
                entity.push_back(i)
                condition[condition_name] = temp
            else:
                entity.push_back(i) 
        self.thisptr = new  CustomParserWrapper(entity, condition)

    def __call__(self,str dir, str delimeter, int skip):
        return self.thisptr[0](ops._string(dir), ops._string(delimeter), skip)


    def to_csv(self, string dir):
        return self.thisptr.save(dir)

    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr
    
#### XMLParser ####
cdef extern from "refacto/controller.hpp":
    cdef cppclass XMLParserController:
        XMLParserController(vector[map[string,string]], map[string,map[string, string]]) nogil except +;
        bool to_csv(string) nogil except+;
        void operator()(string, const string) nogil except +;

cdef class XMLParser:
    """ Python Interface for XMLParserController
        Constructor expect dict with all parameters"""
    cdef XMLParserController* thisptr

    def __cinit__(self, list values):
        cdef map[string, map[string, string]] condition
        cdef vector[map[string, string]] entity

        for item in values:
            if item["type"] == "single" or item["type"] == "group":
                item["output"] = item.get("output", item["key"])
            if item["type"] == "group":
                temp = item["condition"]
                condition_name = item["key"] + item["name"]
                item["condition"] = condition_name
                entity.push_back(item)
                condition[condition_name] = temp
            elif item["type"] == "single" or item["type"] == "multi":
                entity.push_back(item) 
            else:
                raise ValueError("Type not recognised.")
            
        self.thisptr = new XMLParserController(entity, condition)

    def __call__(self,str dir, str root):
        return self.thisptr[0](ops._string(dir), ops._string(root))
    
    def to_csv(self, str dir):
        return self.thisptr.to_csv(ops._string(dir))

    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr
  



#### Decompressdir ####
cdef extern from "refacto/cod.hpp":
    cdef cppclass DECOMPRESSDIR:
        DECOMPRESSDIR(string) nogil except +;
        void operator()(string) nogil except +;

cdef class Decompressdir:
    """Python DECOMPRESSDIR operator extension
    """
    cdef DECOMPRESSDIR *thisptr 

    def __cinit__(self, str extension = None):
        if(extension):
            self.thisptr = new DECOMPRESSDIR(ops._string(extension))
        else:
            self.thisptr = new DECOMPRESSDIR(ops._string(""))
            
    
    def __call__(self, str extension=None):
        if(extension):
            self.thisptr[0](extension)


    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr

#### Compressdir ####
cdef extern from "refacto/cod.hpp":
    cdef cppclass COMPRESSDIR:
        void operator()(string,string) nogil except +;

cdef class Compressdir:
    """Python COMPRESSDIR operator extension
    """
    cdef COMPRESSDIR *thisptr 
    
    def __cinit__(self):
        self.thisptr =  new COMPRESSDIR()


    def __call__(self, str path, str file_name):
        return self.thisptr[0](ops._string(path), ops._string(file_name))

        
    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr

#### Cleandir ####
cdef extern from "refacto/cod.hpp":
    cdef cppclass CLEANFILES:
        void operator()(string,string) nogil except +;

cdef class Cleandir:
    """Python CLEANFILES operator extension
    """
    cdef CLEANFILES *thisptr 
    
    def __cinit__(self):
        self.thisptr =  new CLEANFILES()


    def __call__(self, str path, str pattern):
        return self.thisptr[0](ops._string(path), ops._string(pattern))

        
    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr



cdef extern from "refacto/table.hpp" namespace "table":
    cdef cppclass Series:
        string name;
        map[string,vector[string]] values;


cdef extern from "refacto/table.hpp" namespace "table":
    cdef cppclass Table:
        bool save(string) except +;
        bool insert(Series) except +;
        bool insert(string, Series) except +;
        bool insert(string, vector[string]) except +;

cdef class _Table:

    cdef Table *thisptr 

    def __cinit__(self):
        self.thisptr = new Table()

    def to_csv(self, string path):
        return self.thisptr.save(ops._string(path))



    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr


cdef extern from "refacto/csvparser.hpp":
    cdef cppclass Loc:
        string name;
        string orient;
        int row;
        int column;


cdef class _Location:
    cdef Loc *thisptr

    def __cinit__(self, str name ="", str orient="", int row=0, int column=0):
        self.thisptr = new Loc()
        if(name): self.thisptr.name = ops._string(name)
        if(orient): self.thisptr.orient = ops._string(orient)
        if(row): self.thisptr.row = row
        if(column): self.thisptr.column = column







# CC="gcc" CXX="g++" OPT="" CFLAGS="-std=gnu++17" BASECFLAGS="" LDFLAGS="" CCSHARED="" LDSHARED="gcc -shared" PY_CORE_CFLAGS="" PY_CFLAGS=""  SO="" python3 setup.py build_ext --inplace
