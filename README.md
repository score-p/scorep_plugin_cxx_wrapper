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

In order to write a simple plugin with the wrapper, you have to:

```c++
#include <scorep/plugin/plugin.hpp>
```

Then you create a class, e.g., `my_first_plugin`, which inherits from the base class
`scorep::plugin::base`. The base class needs several template arguments. The first argument is your
plugin class itself. The following template arguments are the used policies.

```c++
class my_first_plugin
  : public scorep::plugin::base<my_first_plugin, Policies...>
```

Then you have to define the - for your set of policies required - member functions in your plugin
class.

Finally, you insert the following macro at the bottom of your file. The first argument is again
your plugin class and the second argument is the plugin name. The name should match the name of
the `.so` library for your plugin, e.g., `libmy_first_plugin.so` matches to `my_first`.

```c++
SCOREP_METRIC_PLUGIN_CLASS(my_first_plugin, "my_first")
```

The plugin name determines the output added, when a message is logged, and the prefix for
environment variables.

### Detailed documentation

The detailed documentation for this wrapper can be found in the
[Wiki](https://github.com/score-p/scorep_plugin_cxx_wrapper/wiki).

### If anything fails

1.  Check whether the plugin library can be loaded from the `LD_LIBRARY_PATH`.
2.  Check, whether the include path is set corretly and you are building with `-std=c++14`.
3.  Open an [issue](https://github.com/score-p/scorep_plugin_cxx_wrapper/issues).
