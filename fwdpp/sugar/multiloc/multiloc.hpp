#ifndef __FWDPP_SUGAR_MULTILOC_MULTILOC_HPP__
#define __FWDPP_SUGAR_MULTILOC_MULTILOC_HPP__

#include <type_traits>
#include <vector>
#include <fwdpp/forward_types.hpp>
#include <fwdpp/sugar/serialization.hpp>
#include <fwdpp/sugar/poptypes/tags.hpp>
namespace KTfwd {
  namespace sugar {
    /*!
      \brief Abstraction of what is needed to simulate a multilocus
      simulation using an individual-based sampler from fwdpp.
      
      All that is missing is the mutation_type and the container types.
      
      Does not allow copy construction/assignment!

      See @ref md_md_sugar for rationale, etc.

      \ingroup sugar
    */
    template<typename mutation_type,
	     typename mlist,
	     typename glist,
	     typename dipvector,
	     typename vglist,
	     typename mvector,
	     typename ftvector,
	     typename lookup_table_type>
    struct multiloc
    {
      //! Dispatch tags for other parts of sugar layer
      using popmodel_t = sugar::MULTILOCPOP_TAG;
      
      //! Mutation type
      using mutation_t = mutation_type;
      //! Gamete type
      using gamete_t = typename glist::value_type;
      //! Vector of diploids
      using dipvector_t = dipvector;
      //! Diploid type (vector<std::pair<glist_t::iterator,glist_t::iterator>)
      using diploid_t = typename dipvector_t::value_type;
      //! Linked list of mutations
      using mlist_t = mlist;
      //! Linked list of gametes
      using glist_t = glist;
      //! Container of glist_t (gametes for each locus)
      using vglist_t = vglist;
            //! Lookup table type for recording mutation positions, etc.
      using lookup_table_t = lookup_table_type;
      
      //! Population size
      unsigned N;
      mlist_t mutations;
      vglist_t gametes;
      dipvector_t diploids;
      /*!
	\brief Can be used to track positions of segregating mutations.
	\note Must have interface like std::map or std::unordered_set
      */
      lookup_table_type mut_lookup;
      //! Vector of mutation_t to track fixations
      mvector fixations;
      /*! \brief vector<unsigned> records times when mutation_ts 
	were added to mut_lookup
      */
      ftvector fixation_times;

      //! Construct with population size and number of loci
      multiloc(const unsigned & __N, const unsigned & __nloci) : N(__N),
								 mutations(mlist_t()),
								 gametes(vglist_t(__nloci,glist_t(1,gamete_t(2*__N)))),
								 diploids(dipvector_t()),
								 mut_lookup(lookup_table_t()),
								 fixations(mvector()),
								 fixation_times(ftvector())
      {
	diploid_t idip;
	for( auto gitr = gametes.begin() ; gitr != gametes.end() ; ++gitr )
	  {
	    idip.emplace_back(std::make_pair(gitr->begin(),gitr->begin()));
	  }
	diploids = dipvector_t(N,idip);
      }

      //! Move constructor
      multiloc(multiloc &&) = default;

      //! Deleted
      multiloc( multiloc & ) = delete;
      //! Deleted
      multiloc( const multiloc & ) = delete;
      //! Deleted
      multiloc & operator=(multiloc &) = delete;
      //! Deleted
      multiloc & operator=(const multiloc &) = delete;

      //! Empty all containers
      void clear() 
      {
	mutations.clear();
	gametes.clear();
	diploids.clear();
	mut_lookup.clear();
	fixations.clear();
	fixation_times.clear();
      }
    };

    /*!
      \brief Abstraction of what is needed to simulate a multilocus
      simulation using an individual-based sampler from fwdpp.
      
      Allows copy/construction/assignment via deep copy.

      See @ref md_md_sugar for rationale, etc.

      \ingroup sugar
    */
    template<typename mutation_type,
	     typename mwriter,
	     typename mreader,
	     typename mlist,
	     typename glist,
	     typename dipvector,
	     typename vglist,
	     typename mvector,
	     typename ftvector,
	     typename lookup_table_type>
    struct multiloc_serialized
    {
      //Dispatch tags for other parts of sugar layer
      using popmodel_t = sugar::MULTILOCPOP_TAG;

      //! Mutation type
      using mutation_t = mutation_type;
      //! Mutation serialization type
      using mwriter_t = mwriter;
      //! Mutation deserialization type
      using mreader_t = mreader;
      //! Gamete type
      using gamete_t = typename glist::value_type;
      //! Vector of diploids
      using dipvector_t = dipvector;
      //! Diploid type (vector<std::pair<glist_t::iterator,glist_t::iterator>)
      using diploid_t = typename dipvector_t::value_type;
      //! Linked list of mutations
      using mlist_t = mlist;
      //! Linked list of gametes
      using glist_t = glist;
      //! Container of glist_t (gametes for each locus)
      using vglist_t = vglist;
      //! Lookup table type for recording mutation positions, etc.
      using lookup_table_t = lookup_table_type;
      
      //! Population size
      unsigned N;
      mlist_t mutations;
      vglist_t gametes;
      dipvector_t diploids;
      /*!
	\brief Can be used to track positions of segregating mutations.
	\note Must have interface like std::map or std::unordered_set
      */
      lookup_table_type mut_lookup;
      //! Vector of mutation_t to track fixations
      mvector fixations;
      /*! \brief vector<unsigned> records times when mutation_ts 
	were added to mut_lookup
      */
      ftvector fixation_times;
     
      //! Construct with population size and number of loci
      multiloc_serialized(const unsigned & __N, const unsigned & __nloci) : N(__N),
									    mutations(mlist_t()),
									    gametes(vglist_t(__nloci,glist_t(1,gamete_t(2*__N)))),
									    diploids(dipvector_t()),
									    mut_lookup(lookup_table_t()),
									    fixations(mvector()),
									    fixation_times(ftvector())
      {
	diploid_t idip;
	for( auto gitr = gametes.begin() ; gitr != gametes.end() ; ++gitr )
	  {
	    idip.emplace_back(std::make_pair(gitr->begin(),gitr->begin()));
	  }
	diploids = dipvector_t(N,idip);
      }

      //! Move constructor
      multiloc_serialized(multiloc_serialized &&) = default;

      //! Copy constructor
      multiloc_serialized( const multiloc_serialized & __m ) : N(0u), mutations(mlist_t()),
							       gametes(vglist_t()),
							       diploids(dipvector_t()),
							       mut_lookup(lookup_table_t()),
							       fixations(mvector()),
							       fixation_times(ftvector())
      {
	serialize s;
	s(__m,mwriter_t());
	deserialize()(*this,s,mreader_t());
      }

      //! Assignment operator
      multiloc_serialized & operator=(const multiloc_serialized & __m)
      {
	serialize s;
	s(__m,mwriter_t());
	deserialize()(*this,s,mreader_t());
	return *this;
      }

      //! Clear all containers
      void clear() 
      {
	mutations.clear();
	gametes.clear();
	diploids.clear();
	mut_lookup.clear();
	fixations.clear();
	fixation_times.clear();
      }
    };
  }
}

#endif