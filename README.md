Score-P Plugin C++ wrapper
==========================

This code provides a wrapper for C++ of the Score-P plugin interface.
The wrapper hides most of the low-level C details, such as memory handling, object lifetimes, etc., from the plugin developer.
To do so, the plugin developer only has to write a class, which inherits from the plugin base class.

Supported features
------------------

The wrapper should support all possible combinations of plugins, which the original Score-P plugin interface supports.

Usage
-----

### Prerequisites

To compile this wrapper and thus your plugin, you need:

* a C++11 compiler

* Score-P

### Create a plugin with the wrapper

As the wrapper is header-only, you have to add the folder *include* to the include path of your plugin.

### If anything fails:

1. Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`.

2. Check, whether the include path is set corretly.

3. Write a mail to the author.

Authors
-------

* Mario Bielert (mario.bielert at tu-dresden dot de)
