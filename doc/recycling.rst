Recycling
------------------------------

During a simulation, mutations and gametes come and go.  Rather than remove them, fwdpp has machinery to "flag" the
locations of such variants into FIFO queues.  The types of these queues are:

.. doxygentypedef:: fwdpp::flagged_mutation_queue

.. doxygentypedef:: fwdpp::flagged_gamete_queue

These two types are examples of **strong types**.  Fundamentally, both are `std::queue<std::size_t>`.
