rm -rf .eggs
rm -rf build
rm -rf dist
rm -rf qparsing.egg-info
rm -rf qparsing/core/usr
rm -rf qparsing/core/build
rm -rf qparsing/parser.cpp
rm -rf qparsing/utils/__pycache__
rm qparsing/core/parser.cpp
# .git


CFLAGS="-Wno-everything" python3 setup.py install_library build_ext sdist bdist_wheel
yes | pip uninstall qparsing
pip install dist/qparsing*.whl