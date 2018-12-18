Populations
---------------------------------------------

Simulations using fwdpp will be based on modifying objects representing populations.  Populations are represented as C++
structs encapsulating all the things that we need during a simulation.  The most important of these are diploids,
gametes, and mutations.

Because fwdpp is a generic library, populations are implemented as templates.

Fundamentally, fwdpp supports two types of simulation:

1. Of single contiguous genomic segments
2. Of multiple, partially-linked genomic segments.

These segments, or *loci* have mutations and recombination events occur within them at each time step.  When multiple
segments are being simulated, recombination may also happen *between* them.

Let's call the first type of simulation a "single locus" simulation, and our population type a "single locus
population".  The other type will be a "multi-locus" simulation and population.

Both types inherit from a common base class, described in the next section.

The population base class
==========================================

.. doxygenclass:: fwdpp::poptypes::popbase
   :members:

A single-locus population
==========================================

Simulations of single regions will typically rely on this following typedef:

.. doxygentypedef:: fwdpp::slocuspop

The following is more readable:

.. code-block:: cpp

    template <typename mtype,
              typename diploid_t = std::pair<std::size_t, std::size_t>>
    using slocuspop
        = poptypes::slocuspop<mtype, std::vector<mtype>, std::vector<gamete>,
                           std::vector<diploid_t>, std::vector<mtype>,
                           std::vector<uint_t>,
                           std::unordered_multimap<double, std::uint32_t>>;

The typedef requires that you supply a mutation type (i.e., something derived from :class:`fwdpp::mutation_base`, such
as :class:`fwdpp::popgenmut`), and a diploid type, which defaults to a pair of `std::size_t`.  For this type,
the various containers are all in terms of standard library templates.

The preceding declaration is a template alias for the full class:

.. doxygenclass:: fwdpp::poptypes::slocuspop
   :members:

A multi-locus population
==========================================

Simulations of single regions will typically rely on this following typedef:

.. doxygentypedef:: fwdpp::mlocuspop

The following is more readable:

.. code-block:: cpp

    template <typename mtype,
              typename diploid_t = std::pair<std::size_t, std::size_t>>
    using mlocuspop
        = poptypes::mlocuspop<mtype, std::vector<mtype>, std::vector<gamete>,
                           std::vector<std::vector<diploid_t>>,
                           std::vector<mtype>, std::vector<uint_t>,
                           std::unordered_multimap<double, uint_t>>;

The typedef requires that you supply a mutation type (i.e., something derived from :class:`fwdpp::mutation_base`, such
as :class:`fwdpp::popgenmut`), and a diploid genotype type, which defaults to a pair of `std::size_t`.  For this type,
the various containers are all in terms of standard library templates.

.. note::

    The primary difference to :typedef:`fwdpp::slocuspop` is that a diploid is a vector of pairs of keys to gametes.

The preceding declaration is a template alias for the full class:

.. doxygenclass:: fwdpp::poptypes::mlocuspop
   :members:

API comments
====================================================

When writing functions to act on popluations, you can save yourself API clutter via the use of tag dispatch, which is an
important C++ technique.  For example:

.. code-block:: cpp

    namespace detail
    {
        template<typename poptype>
        void function_detail(const poptype & pop, fwdpp::poptypes::SLOCUSPOP_TAG)
        {
            //slocus-specific implementation
        }

        template<typename poptype>
        void function_detail(const poptype & pop, fwdpp::poptypes::MULTILOC_TAG)
        {
            //mlocus-specific implementation
        }
    }

    template<typename poptype>
    void function(const poptype & pop)
    {
        // Dispatch to correct version
        detail::function_detail(pop, typename poptype::popmodel_t())
    }
    
Note that this dispatch is compile-time.  There is zero run time cost.  It is a bit more code to write, but you end up
with *one* function in your public API.  Of course, if your implementation can be the same for both types of population,
then no dispatch is required.


Other possible population class types
=====================================================

The types :class:`fwdpp::poptypes::slocuspop` and :class:`fwdpp::poptypes::mlocuspop`
are not the only possible way to do things!  In fwdpy11_, we introduce a new class inheriting
from :class:`fwdpp::poptypes::popbase`, which allows us to add a bunch of API requirements
specific to doing things in Python.  Ultimately, the main requirement is that final types 
correctly duck-type the ones shown here.

.. _fwdpy11: https://fwdpy11.readthedocs.io
