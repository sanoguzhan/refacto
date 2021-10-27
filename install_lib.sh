rm -rf .eggs
rm -rf build
rm -rf dist
rm -rf parsing.egg-info
rm -rf parsing/core/usr
rm -rf parsing/core/build
rm -rf parsing/parser.cpp
rm -rf parsing/utils/__pycache__
rm parsing/core/parser.cpp
# .git


CFLAGS="-Wno-everything" python3 setup.py install_library build_ext sdist bdist_wheel
yes | pip uninstall parsing
pip install dist/parsing*.whl