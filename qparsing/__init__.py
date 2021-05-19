if __package__ is None or __package__ == '':
    # imports = map(__import__, ['qparsing.engine'])
    # globals().update(imports) 
    pass
else:
    import importlib
#     # modules = ['engine.CSVParser', 'XMLParser', 'Decompressdir', 'Cleandir', 'Compressdir'] 
    # cython_modules = map('engine', modules)
    globals().update(importlib.import_module('qparsing.engine').__dict__) 
    imports = map(__import__, ['qparsing.engine'])
    # globals().update(imports) 
