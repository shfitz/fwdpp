Common mutation types
----------------------------------

.. code-block:: cpp

    #include <fwdpp/popgenmut.hpp>

:class:`fwdpp::popgenmut` represents a mutation with a single effect size/selection coeffient, dominance coefficient,
and it tracks the generation when it arose (:member:`fwdpp::popgenmut::g`).

.. doxygenstruct:: fwdpp::popgenmut
   :project: fwdpp
   :members:

fwdpp also defines the following type, which is simpler, but less useful, as it doesn't track its origin time:

.. doxygenstruct:: fwdpp::mutation
   :members:
