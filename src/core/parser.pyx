# cython: c_string_type=unicode, c_string_encoding=utf8
# distutils: language = c++

from core cimport ops
from libcpp cimport bool
from libcpp.map cimport map
from libcpp.vector cimport vector
from libcpp.string cimport string

from cython.operator cimport dereference as deref

cdef bint boolean_variable = True

# Only extend the ones we use
cdef extern from "include/controller.hpp":
    cdef cppclass CSVParserWrapper:
        string path;
        CSVParserWrapper(map[string,map[string, string]])
        void init_csvparser(map[string,string]);
        void from_csv(string, Loc); #Single value
        void from_csv0(string, Loc, int); #Series
        void from_csv1(string, int, Loc, Loc); #Series
        void from_csv2(string, int, Loc, Loc, Loc); #Series
        void from_csv_vec(string, string, int, int, int); # Vector value
        void to_csv(string);



cdef class CSVParser:
    """ Python Interface for CSVParserWrapper
        Overload operator() accepts dict"""
    cdef CSVParserWrapper* thisptr
    cdef dict kwargs
    cdef str parser_type 

    def __cinit__(self, map[string, map[string,string]] kwargs):
        self.kwargs = kwargs
        self.parser_type = str(*self.kwargs.keys())

        self.thisptr =  new CSVParserWrapper(kwargs)

    def __call__(self,dict vars):
        cdef _Location target

        if self.parser_type != "csvparser":
            raise KeyError("wrong Key, Key must be csvparser")

        if vars.get("series"):
            for key in vars.get("series"):
                self.csv_series_func(vars.get("series").get(key))
        
        if vars.get("vector_value"):
            for key in vars.get("vector_value"):
                self.thisptr.from_csv_vec(ops._string(key), 
                                    vars.get("vector_value").get(key).get("orient"),
                                    vars.get("vector_value").get(key).get("index"),
                                    vars.get("vector_value").get(key).get("from"),
                                    vars.get("vector_value").get(key).get("to"))
        if vars.get("single_value"):
            for key in vars.get("vector_value"):
                target = self.init_loc(vars.get("single_value").get(key))
                self.thisptr.from_csv(ops._string(key),
                    deref(target.thisptr))


    cdef csv_series_func(self, dict args):
        """ Polymorphic methods of CSVParser each given with condition"""
        cdef:
            int cond_len
            _Location target, loc0,loc1
        cond_len = len(args.get("cond"))
        if cond_len == 0:
            target = self.init_loc(args)
            self.thisptr.from_csv0(args.get("orient"),
                                deref(target.thisptr), 
                                args.get("index",0))

        elif cond_len == 1:
            target = self.init_loc(args)
            loc0 = self.init_loc(args.get("cond")[0])
            self.thisptr.from_csv1(args.get("orient"),
                                args.get("index",0),
                                deref(target.thisptr), 
                                deref(loc0.thisptr))

        elif cond_len == 2:
           target = self.init_loc(args)
           loc0 = self.init_loc(args.get("cond")[0])
           loc1 = self.init_loc(args.get("cond")[1])
           self.thisptr.from_csv2(args.get("orient"),
                               args.get("index",0),
                               deref(target.thisptr),
                               deref(loc0.thisptr),
                               deref(loc1.thisptr))


    cdef init_loc(self, dict args):
        return _Location(args.get("name"),
                        args.get("orient"),
                        args.get("row", 0),
                        args.get("column", 0)) 


    cpdef to_csv(self, str path):
        self.thisptr.to_csv(ops._string(path))


    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr



cdef extern from "include/table.hpp" namespace "table":
    cdef cppclass Series:
        string name;
        map[string,vector[string]] values;


cdef extern from "include/table.hpp" namespace "table":
    cdef cppclass Table:
        bool save(string) except +;
        bool insert(Series) except +;
        bool insert(string, Series) except +;
        bool insert(string, vector[string]) except +;



cdef extern from "include/csvparser.hpp":
    cdef cppclass Loc:
        string name;
        string orient;
        int row;
        int column;


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



# CC="gcc" CXX="g++" OPT="" CFLAGS="-std=gnu++17" BASECFLAGS="" LDFLAGS="" CCSHARED="" LDSHARED="gcc -shared" PY_CORE_CFLAGS="" PY_CFLAGS=""  SO="" python3 setup.py build_ext --inplace
