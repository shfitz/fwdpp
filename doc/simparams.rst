Encapsulating the genetic parameters 
-----------------------------------------------------

Simulations need to pass the functions for various functions related to the genetics
of the model.  The relatively large number of widgets needed to model the genetics 
can result in long argument lists for functions.  :class:`fwdpp::genetic_parameters`
helps encapsulate them into one object.

The class is able to hold the parameters for single- and multi- locus simulations.
For a single-locus simulation, the type representing "inter-locus recombination"
is expected to be `std::nullptr_t`.

.. doxygenstruct:: fwdpp::genetic_parameters
   :members:

In practice, it is very tedious to fully-specify the template parameter names for
:class:`fwdpp::genetic_parameters`.  We offer the following two functions to help.

For single-locus simulations: 

.. cpp:function:: template <typename genetic_value, typename mutation_function, \
                  typename recombination_function> \
                  inline genetic_parameters< \
                  typename std::remove_reference<genetic_value>::type, \
                  typename std::remove_reference<mutation_function>::type, \
                  typename std::remove_reference<recombination_function>::type, \
                  std::nullptr_t, mendel> \
                  make_genetic_parameters( \
                  genetic_value&& gvalue_param, \
                  mutation_function&& generate_mutations_param, \
                  recombination_function&& generate_breakpoints_param)

For multi-locus simulations: 

.. cpp:function:: template <typename genetic_value, typename mutation_function, \
                            typename recombination_function, \
                            typename interlocus_recombination> \
                  inline genetic_parameters< \
                      typename std::remove_reference<genetic_value>::type, \
                      typename std::remove_reference<mutation_function>::type, \
                      typename std::remove_reference<recombination_function>::type, \
                      typename std::remove_reference<interlocus_recombination>::type, mendel> \
                  make_genetic_parameters( \
                      genetic_value&& gvalue_param, \
                      mutation_function&& generate_mutations_param, \
                      recombination_function&& generate_breakpoints_param, \
                      interlocus_recombination&& interlocus_rec)

Yikes, those prototypes are horrid!  They rely on C++11's "perfect-forwarding" mechanism to get 
the input parameters into a :class:`fwdpp::genetic_parameters` object. We need to do a lot of 
type traits nonsense (all the `std::remove_reference<...>` stuff) to make sure that the decayed input type
(meaning the type name after removing all reference labels, etc.) is used to parameterize the return value.

In spite of the complexity, the parameter names are intended to map to the public members of 
:class:`fwdpp::genetic_parameters`.
    
