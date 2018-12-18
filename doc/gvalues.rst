Calculation of genetic values
-----------------------------------------------------------

To do anything interesting, we need to calculate the "fitness" of an individual, meaning we have to process the data in
its gametes and return a `double`.  We need to separate out two different concepts:

1. The gamete data may lead to a *direct* calculation of fitness.  This is the typical case in population genetics,
   where a mutation has a "selection coefficient" which is its **direct** effect on fitness
2. The gamete data may refer to effect sizes on a trait (phenotype), and then that phenotype is subject to fitness.

I refer to the result of processing the gamete data as the *genetic value* of a diploid.  In some modeling situations,
that is fitness.  In others, it is a phenotype that must be further mapped to fitness.

Fitnesses should have an interpretation along the lines of "the mean number of offspring".

The library provides the several types for calculating genetic values.

.. note::

    All of the types discussed here require that a mutation have fields `s` and `h` representing 
    effect size and dominance, respectively.  This notation follows traditions from population genetics.
    :class:`fwdpp::popgenmut` is an example of a mutation type compatible with the function objects described
    below.  Types that you define should follow this tradition, else you have to reinvent the wheel.

To include them:

.. code-block:: cpp

    // NOTE: this may be renamed genetic_values.hpp in the future
    #include <fwdpp/fitness_models.hpp>

First, we have to strong types that help us construct instances of our classes:

.. doxygentypedef:: fwdpp::trait

.. doxygentypedef:: fwdpp::fitness

Next, we have three function objects that may be used to calculate the genetic values for a *single-locus* diploid genotype:

.. doxygenclass:: fwdpp::multiplicative_diploid

.. doxygenclass:: fwdpp::additive_diploid

The above two classes may be used to calculate either fitness or trait values:

.. code-block:: cpp

    auto multi_fitness = fwdpp::multiplicative_diploid(fwdpp::fitness(1.0));

    auto additive_trait = fwdpp::additive_trait(fwdpp::trait(2.0));

The first object, `multi_fitness`, calculates fitness as :math:`max(0,\prod_i (1+G_i))`, where :math:`G_i` is 0, `sh`, or `s`
for genotypes `AA`, `Aa`, and `aa`, respectively, with `a` representing the mutation.

The second object, `additive_trait`, returns a value representing a trait value, :math:`\sum_i G_i`, where :math:`G_i`
is 0, `sh`, or `2s` for each of the same three genotypes.

Thus, the floating-point values passed to :type:`fwdpp::trait` and :type:`fwdpp::fitness` represent the "scaling" of the
model, which allows you to handle, for example, the various different ways that population genetics treats the `aa`
genotype.

The above discussion implies the following:

1. Fitness values are :math:`\geq 0` and, typically, calculated with respect to a value of one, meaning that, in the
   absence of mutations, the expected fecundity of the individual is one offspring on average.
2. Trait values do not have the requirement of being :math:`\geq 0`.  Thus, if the mutation model results in effect
   sizes that may be positive or negative, trait values can be greater or less than zero.

Example: Gaussian stabilizing selection
======================================================

We need to calculate fitness.  Thus, if we calculate a trait value, as with `additive_trait` above, we have more work to
do.  Let's look at how to map trait value to fitness, with a concrete example, which is Gaussian stabilzing selection
around an optimal trait value of zero:

.. code-block:: cpp

    #include <cmath>

    struct gss
    {
        const double vs;
        const double opt;
        const fwdpp::additive_diploid trait_value;
        gss(doube v, double o) : vs(v), opt(o), trait_value(fwdpp::trait(2.0)) {}

        template<typename diploid_type,
                 typename gamete_container,
                 typename mutation_container>
        inline double operator()(const diploid_type & dip,
                                 const gamete_container & gametes,
                                 const mutation_container & mutations) const
        {
            auto tv = trait_value(dip, gametes, mutations);
            return std::exp(-std::pow(tv-opt, 2.0)/(2.*vs));
        }
    };

.. note::

    There are a few different call operator overloads provided by the built-in classes.
    The above signature is the most commonly-used.

:class:`fwdpp::additive_diploid` and `fwdpp::multiplicative_diploid` are implemented in terms of the following struct:

.. doxygenstruct:: fwdpp::site_dependent_genetic_value

The call operators of this struct take functions that process `Aa` and `aa` genotypes as they are seen.  If you want to
write your own boutique calculator, take a look at the implementation of, say, :class:`fwdpp::multiplicative_diploid`,
for more details.

Example: multiplicative fitness for a multi-locus simulation
==============================================================================

The library provides no functions for multi-locus simulations.  Trust us, this is a feature, not a bug!  We envision
situations where different loci may have different functions applied to them, so we leave anything concrete out.

Here is an example of a fitness calculator for a multi-locus system:

.. code-block:: cpp

    struct multilocus_multiplicative
    {
        const fwdpp::multiplicative_diploid m;
        //NOTE: older compilers may not accept initialization
        //of this type into fwdpp::simparams unless this default
        //constructor is defined:
        multilocus_multiplicative() : m(fwpp::fitness(2.)) {}

        template<typename diploid,
                 typename gamete_container,
                 typename mutation_container>
        inline double
        operator()(const diploid &diploid,
                   const gamete_container &gametes,
                   const mutation_container &mutations) const
        {
            double rv = 1.;
            for (auto &&genotype : diploid)
                {
                    rv *= m(genotype, gametes, mutations);
                }
            return std::max(0., rv);
        }
    };
        
