import setuptools
import os
import subprocess
import distutils
import logging

from setuptools import setup
from distutils.extension import Extension
from distutils.command.sdist import sdist as _sdist
from distutils.errors import DistutilsSetupError
from distutils.command.build_clib import build_clib
from logging import log
from pathlib import Path
from utils import custom_logger

setuptools.dist.Distribution().fetch_build_eggs(['Cython'])
from Cython.Distutils import build_ext
from Cython.Build import cythonize 

__version__ = '0.0.1'

log = logging.getLogger("Refacto")

ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
cf = custom_logger.ColoredFormatter("[%(name)s][%(levelname)s]  %(message)s (%(filename)s:%(lineno)d)")
ch.setFormatter(cf)
log.addHandler(ch)

fh = logging.FileHandler('src/lib.log')
fh.setLevel(logging.DEBUG)
ff = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(ff)
log.addHandler(fh)

log.setLevel(logging.DEBUG)


# Linking C++ Refacto Library to Cython Parser.Pyx

os.environ['LD_LIBRARY_PATH'] = "src/core/usr/local/lib/"
os.environ["CC"] = "clang"
_path = "/core/usr/local/lib/"
ext_modules = [Extension("parsing.engine",
                     ["src/core/parser.pyx",],
                     language='c++',
                     include_dirs= ["/include", ],
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
    self.path_to_build = 'src/core'

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
      with Pwd('src/core/build') as shell:
        shell.run(["conan", "install", "..", "--build=missing"],  stdout=open(os.devnull, 'wb'))
        shell.run(["cmake", ".."],
        stdout=open(os.devnull, 'wb'))
        log.info("Building static library...",)
       
        shell.run(["make", "install"],  
                stdout=open(os.devnull, 'wb')) 
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
  packages=setuptools.find_packages(exclude=[ "tests/*"]),
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