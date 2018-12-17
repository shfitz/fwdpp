Random number generators
-----------------------------------------------

fwdpp uses the GNU Scientific library for its random-number generation and for sampling from various distributions.  The
library takes a modern C++ approach to managing the allocation/deallocation of the low-level `gsl_rng *`:

.. code-block:: cpp

    #include <fwdpp/GSLrng_t.h>

.. doxygenclass:: fwdpp::GSLrng_t
    :members:

Behind the scenes, the memory management is done via `std::unique_ptr`.  Thus, this class is move-only.

.. note::

    The underlying pointer is accessed via :func:`fwdpp::GSLrng_t::get`.  Compiling at optimization level -O2
    or greater will completely eliminate the function call.

Mersenne twister
++++++++++++++++++++++++++++++

In most cases, you'll want to use a mersene twister in your simulations:

.. doxygentypedef:: fwdpp::GSLrng_mt
    
