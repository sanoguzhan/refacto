from __future__ import print_function 

import setuptools
import os
import subprocess
import distutils
import logging
from copy import copy
from logging import Formatter
from setuptools import setup
from distutils.extension import Extension
from distutils.command.sdist import sdist as _sdist
from logging import log
import pathlib
setuptools.dist.Distribution().fetch_build_eggs(['Cython'])
from Cython.Distutils import build_ext
from Cython.Build import cythonize 

BASE_DIR = pathlib.Path(__file__).parent
README = ( BASE_DIR / "README.md").read_text()
__version__ = os.getenv('LIB_VERSION')


MAPPING = {
    'DEBUG'   : 37, # white
    'INFO'    : 36, # cyan
    'WARNING' : 33, # yellow
    'ERROR'   : 31, # red
    'CRITICAL': 41, # white on red bg
}

PREFIX = '\033['
SUFFIX = '\033[0m'

class ColoredFormatter(Formatter):

    def __init__(self, patern):
        Formatter.__init__(self, patern)

    def format(self, record):
        colored_record = copy(record)
        levelname = colored_record.levelname
        seq = MAPPING.get(levelname, 37) # default white
        colored_levelname = ('{0}{1}m{2}{3}') \
            .format(PREFIX, seq, levelname, SUFFIX)
        colored_record.levelname = colored_levelname
        return Formatter.format(self, colored_record)

log = logging.getLogger("Refacto")

ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
cf = ColoredFormatter("[%(name)s][%(levelname)s]  %(message)s (%(filename)s:%(lineno)d)")
ch.setFormatter(cf)
log.addHandler(ch)

fh = logging.FileHandler('parsing/lib.log')
fh.setLevel(logging.DEBUG)
ff = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(ff)
log.addHandler(fh)

log.setLevel(logging.DEBUG)


# Linking C++ Refacto Library to Cython Parser.Pyx

os.environ['LD_LIBRARY_PATH'] = "parsing/core/usr/local/lib/"
os.environ["CC"] = "clang"
_path = "/core/usr/local/lib/"
ext_modules = [Extension("parsing.engine",
                     ["parsing/core/parser.pyx",],
                     language='c++',
                     include_dirs= ["/include", ],
                      extra_compile_args=['-std=c++17'],
                     libraries= ["refacto"],
                     library_dirs= [_path],
                     runtime_library_dirs=[_path]
                     )]

# Running Cmake and Make as Subprocess 
# So we can build library on each platform with command and install python library
class Pwd:
    dir_stack = []

    def __init__(self, dirname):
        self.dirname = os.path.realpath(os.path.expanduser(dirname))

    def __enter__(self):
        Pwd.dir_stack.append(self.dirname)
        return self

    def __exit__(self,  type, value, traceback):
        Pwd.dir_stack.pop()

    def run(self, cmdline, **kwargs):
        return subprocess.check_call(cmdline, cwd=Pwd.dir_stack[-1], **kwargs)


class InstallCppLib(distutils.cmd.Command):
  """A custom command to run install command for c++ library."""

  description = 'run make to install C++ library'
  user_options = [
      ('path-to-build=', None, 'path to make file'),
  ]

  def initialize_options(self):
    """Set default values for options."""
    self.path_to_build = 'parsing/core'

  def finalize_options(self):
    """Post-process options."""
    if self.path_to_build:
      assert os.path.exists(self.path_to_build), (
          'Core %s does not exist.' % self.path_to_build)

  def run(self):
    """Run command."""
    log.info("Compiling the code at %s..." % (self.path_to_build))
   
    with Pwd(self.path_to_build) as shell:
      shell.run(["mkdir", "-p", "build"],  stdout=open(os.devnull, 'wb'))
      shell.run(["conan", "profile", "update", "settings.compiler.libcxx=libstdc++11", "default"],  stdout=open(os.devnull, 'wb'))
      with Pwd('parsing/core/build') as shell:
        shell.run(["conan", "install", "..", "--build=missing"],  stdout=open(os.devnull, 'wb'))
        shell.run(["cmake", ".."],
        stdout=open(os.devnull, 'wb'))
        log.info("Building static library...",)
       
        shell.run(["make", "install"],  
                ) 
        shell.run(["make", "install", "DESTDIR=.."],  
                stdout=open(os.devnull, 'wb'))  
    log.info("Refacto Library Installed!")  
    self.announce("Done!",
        level=distutils.log.INFO)


setup(
  name = "parsing",
  version = __version__,
  author='Oguzhan San',
  package_data={'': ["*.so",'*.pyx', '*.pxd', '*.h', '*.cpp', '*.hpp']},
  long_description=README,
  long_description_content_type="text/markdown",
  packages=setuptools.find_packages(exclude=[ "tests/*", "utils"]),
  include_package_data=True,
  ext_modules = cythonize(ext_modules, 
  compiler_directives={'language_level' : "3"},
  ),
  install_requires=["cython"],
  cmdclass = {'install_library': InstallCppLib,
              'build_ext': build_ext},
  setup_requires=['wheel', 'cython', 'pytest-runner'],
  tests_require = ['pytest'],
  test_suite ='tests',
  python_requires='>=3.0',
    classifiers=[
    'Development Status :: 3 - Alpha',      
    'Intended Audience :: Developers',      
    'Topic :: Software Development :: Build Tools',
    'Natural Language :: English',   
    'Programming Language :: Python :: 3',      
    'Programming Language :: Python :: 3.4',
    'Programming Language :: Python :: 3.5',
    'Programming Language :: Python :: 3.6',
  ],
  keywords=[
        'Cython',
        'Parsing',
        'C++',
        'Refacto'
    ],
)
