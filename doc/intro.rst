Introduction
---------------------------------

This document covers the basic concepts and data types used to write simulations using fwdpp.  This manual will **not**
be a thorough reference guide to the library.  Rather, we will focus on the big picture.  Further, we will focus on
concepts related to writing simulations using tree sequence recording, which is a powerful method for improving
simulation performance while also recording the entire genealogy of the simulation.

The primary citation for fwdpp is:

::

    Thornton, Kevin R. 2014. “A C++ Template Library for Efficient Forward-Time Population Genetic Simulation of Large Populations.” Genetics 198 (1): 157–66.

For background on tree sequence recording, see the following paper:

::

    Kelleher, Jerome, Kevin R. Thornton, Jaime Ashander, and Peter L. Ralph. 2018. “Efficient Pedigree Recording for Fast Population Genetics Simulation.” PLoS Computational Biology 14 (11): e1006581.

fwdpp has changed a **lot** since the 2014 paper.  It is much more efficient, and the API is becoming safer and more
stable.  The goal of this document is to describe the current status of the library, and illustrate "best practices", at
least as we see them.

Many of the ideas in this document are used in fwdpy11_, which is a Python package based on fwdpp.

Obtaining the code
==================================

This manual will be most helpful if you have access to the fwdpp code, which is available via github:

.. code-block:: bash

    git clone https://github.com/molpopgen/fwdpp
    cd fwdpp

In there, you will find the examples program in the `examples` sub-directory.  Those examples, and the fwdpy11_ code,
are the best "deep dive" into fwdpp.  Of particular interest here, because they involve tree sequences, are the
following example programs:

* :ref:`wfts` 
* :ref:`wfmlts`
* :ref:`spatialts`

The original source files come with the GitHub repo.

Namespaces
===========================

The primary namespace is fwdpp.

.. _fwdpy11: https://fwdpy11.readthedocs.io

