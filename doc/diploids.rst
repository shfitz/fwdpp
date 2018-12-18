Diploids
----------------------------------

Single-locus
================================

A diploid consists of two genomes, each of which is represented by a gamete.  A diploid stores the *indexes* of its two
gametes, and must duck-type the public interface of `std::pair<std::size_t,std::size_t>`.  Thus, for many types of
simulation, the following suffices:

.. code-block:: cpp

    using diploid = std::pair<fwdpp::uint_t, fwdpp::uint_t>;

You may use any integral type that has the correct range for your simulation.

.. note::

    This "duck-typing" of `std::pair` means that a diploid type must have member typedefs first_type and 
    second_type as well as members first and second. Further, first_type and second_type must be
    the same type.


Multi-locus
================================

fwdpp allows genomes to be broken up into discrete "chunks". Each chunk is represented by something pair-like, as
described above.

For example:

.. code-block:: cpp

    using diploid_genotype = std::pair<fwdpp::uint_t, fwdpp::uint_t>;
    using diploid = std::vector<diploid_genotype>;

Meta-data
=================================

It is often nice to track data associated with individuals.  To do so, you have two options:

1. Create a custom diploid type that contains the relevant data.
2. Use a simple diploid type as above and track the meta-data separately in a vector of a custom type.

In general, we prefer the latter approach, as it allows easy tracking of meta-data of ancient samples when recording
them into tree sequences.
