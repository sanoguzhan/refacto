import setuptools
import os
from re import sub
from distutils.core import setup
from distutils.extension import Extension
from distutils.command.sdist import sdist as _sdist
from distutils.errors import DistutilsSetupError
from distutils.command.build_clib import build_clib
from logging import log
from pathlib import Path
setuptools.dist.Distribution().fetch_build_eggs(['Cython'])
from Cython.Distutils import build_ext
from Cython.Build import cythonize 

os.environ['LD_LIBRARY_PATH'] = "/parsing/core/usr/local/lib/"
# _path = str("requirements.txt")
# with open(_path) as f:
#     required = f.read().splitlines()

os.environ["CC"] = "clang"
# _path = Path(os.path.dirname(__file__))
_path = "/parsing/core/usr/local/lib/"
# dir_search = os.path.join(, "search")
ext_modules = [Extension("core",
                     ["parsing/parser.pyx"],
                     language='c++',
                     include_dirs= ["parsing/core/include", ],
                     libraries= ["refacto"],
                     library_dirs= [_path],
                     runtime_library_dirs=[_path]
                     )]



setup(
  name = "stalib",
  package_data={'': ['*.pyx', '*.pxd', '*.h', '*.cpp', '*.hpp']},
  packages=setuptools.find_packages(exclude=[ "tests/*"]),
  include_package_data=True,
  ext_modules = cythonize(ext_modules, 
  compiler_directives={'language_level' : "3"},
  ),
#   libraries=ext_modules,
  cmdclass = {'build_ext': build_ext},
  setup_requires=['cython', 'pytest-runner'],
  tests_require = ['pytest'],
  test_suite ='tests',
  python_requires='>=3.0',
)