Score-P Plugin C++ wrapper
==========================

This code provides a wrapper for C++ of the Score-P plugin interface.
The wrapper hides most of the low-level C details, such as memory handling, object lifetimes, etc.,
from the plugin developer. To do so, the plugin developer only has to write a class, which inherits
from the plugin base class.

Supported features
------------------

The wrapper should support all possible combinations of plugins, which the original Score-P plugin
interface supports.

Usage
-----

### Prerequisites

To compile this wrapper and thus your plugin, you need:

*   a C++14 compiler
*   Score-P

### Build settings

The whole wrapper makes heavily use of C++14 features, so you'll need to set the used standard to
`-std=c++14`.

As the wrapper is header-only, you have to add the folder `include` to the include path of your
plugin, but you don't need special linker settings.

### Getting started

In order to write a simple plugin with the wrapper, you have to:

```c++
#include <scorep/plugin/plugin.hpp>
```

Then you create a `class async_plugin`, which inherits from `scorep::plugin::base`. The base class
needs several template arguments. The first argument is your plugin class itself. The following
template arguments are the used policies.

```c++
class my_first_plugin
  : public scorep::plugin::base<my_first_plugin, Policies...>
```

Then you have to define the - for your set of policies required - member functions in your plugin
class.

Finally, you insert the following macro at the bottom of your file. The first argument is again
your plugin class and the second argument is a name for the plugin. The name should match the
name of the `.so` library for your plugin, e.g., `libmy_first_plugin.so` matches to
`my_first`.

```c++
SCOREP_METRIC_PLUGIN_CLASS(my_first_plugin, "my_first")
```

### If anything fails

1.  Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`.
2.  Check, whether the include path is set corretly.
3.  Open an [issue](https://github.com/bmario/scorep_plugin_cxx_wrapper/issues).
