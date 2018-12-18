Basic types
-----------------------------

Header files
=========================

The types described on this page are found in the following header:

.. code-block:: cpp

    #include <fwdpp/forward_types.hpp>


Unsigned integer type
=========================

A lot of fwdpp relies on an unsigned, 32-bit integer, which you will see a lot in function prototypes, etc.:

.. doxygentypedef:: fwdpp::uint_t

Mutations
=========================

Fundementally, a mutation is a type that publicly inherits from :class:`fwdpp::mutation_base`, which is defined as
follows:

.. doxygenstruct:: fwdpp::mutation_base
   :project: fwdpp
   :members:

As you can see, this class provides a position, :member:`fwdpp::mutation_base::pos`, which is a C++ double.  A mutation
also contains a boolean flag, :member:`fwdpp::mutation_base::neutral`, which is `true` if the mutation affects trait
value/fitness, and `false` otherwise.   Finally, the field :member:`fwdpp::mutation_base::xtra` is a 16-bit integer that
exists simply because it fills in space that would otherwise be wasted by structure packing.  You may use this type to
label different classes of mutations.  For example, the deme they arose in, "methylation status", etc.

This type is not especially useful on its own.  There is no support for effect sizes, dominance terms, etc.. The library
provides :class:`fwdpp::popgenmut` to cover the common use case of a mutation with a single effect size and heterozygous effect
modifier ("dominance coefficient").

Gametes
=========================

A gamete type represents some section of a genome.  Essentially, it is a holder of "keys" to mutations, where a key is
:cpp:type:`fwdpp::uint_t`.

The keys things to realize about a gamete are:

1. They track how many times they exist via :member:`fwdpp::gamete_base::n`
2. Keys to *neutral* variants are stored in :member:`fwdpp::gamete_base::mutations`
3. Keys to *selected* variants are stored in :member:`fwdpp::gamete_base::smutations`

A gamete is represented by the following struct:

.. doxygenstruct:: fwdpp::gamete_base
   :members:

In practice, simulations are written using the following type:

.. doxygentypedef:: fwdpp::gamete


