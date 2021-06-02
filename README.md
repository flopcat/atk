# The A Toolkit

A lightweight toolkit for simple GUI applications that wraps Win32 functions.
Features lazy widget creation and event callbacks.

The toolkit is generally designed to have similar concepts as the leading GUI
toolkits, but the code you write will likely will not be the same as when using
them because our scope is not anywhere near as grandiose.  In particular, while
we hide most Win32-isms you may still have to use Win32 constants and the like.

We are leveraging the c++ standards (mostly templates and std::functional) for
our design, and our object system only goes as far as object ownership with
automatic child destruction of heap objects.  However, it is hoped that your
use of this library will still be an enjoyable one.

Compile with mingw32 and -DUNICODE.  Some efforts are made to ensure this runs
similarly in Windows and in wine.

Example program included.  Sources may include testing/tracing statements, so
feel free to comment those out before redistribution.

## Coding style

We try to stick to best practice, but may diverge here and there.  Here are
some good reads which may inspire you if you wish to extend this toolkit, but
we don't stick to them.

* [C++ style](http://geosoft.no/development/cppstyle.html)
* [The Little Manual of API Design](https://people.mpi-inf.mpg.de/~jblanche/api-design.pdf)
* [Digia API design](https://doc.qt.io/archives/qq/qq13-apis.html)
* [Qt coding style](https://wiki.qt.io/Qt_Coding_Style)

