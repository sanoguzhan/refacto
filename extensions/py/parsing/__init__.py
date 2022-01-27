if __package__ is None or __package__ == '':
    pass
else:
    import importlib
    # modules = ['engine.CSVParser', 'XMLParser', 'Decompressdir', 'Cleandir', 'Compressdir'] 
    globals().update(importlib.import_module('parsing.engine').__dict__) 
    imports = map(__import__, ['parsing.engine'])
