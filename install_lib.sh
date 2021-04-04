rm -rf .eggs
rm -rf build
rm -rf dist
rm -rf parsing.egg-info
rm -rf src/core/usr
rm -rf src/core/build
rm -rf src/parser.cpp
rm -rf src/utils/__pycache__
rm src/core/parser.cpp
# .git

python3 src/setup.py install_library build_ext sdist bdist_wheel
pip install dist/parsing*.whl