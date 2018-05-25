#ifndef __FWDPP_SUGAR_SINGLEPOP_SINGLEPOP_HPP__
#define __FWDPP_SUGAR_SINGLEPOP_SINGLEPOP_HPP__

/*
  A structure representing a single-locus population.
  The user initizializes it with a population size, N
*/

#include <fwdpp/sugar/poptypes/tags.hpp>
#include <fwdpp/sugar/poptypes/popbase.hpp>
namespace fwdpp
{
    namespace sugar
    {
        /*!
          \brief Abstraction of what is needed to simulate a
          single-locus population.

          All that is missing is the mutation_type and the container types.

          See @ref md_md_sugar for rationale, etc.

          \ingroup sugar
        */
        template <typename mutation_type, typename mcont, typename gcont,
                  typename dipvector, typename mvector, typename ftvector,
                  typename lookup_table_type>
        class slocuspop : public popbase<mutation_type, mcont, gcont, mvector,
                                         ftvector, lookup_table_type>
        {
          private:
            void
            process_individual_input()
            {
                std::vector<uint_t> gcounts(this->gametes.size(), 0);
                for (auto &&dip : diploids)
                    {
                        this->validate_individual_keys(dip.first);
                        this->validate_individual_keys(dip.second);
                        gcounts[dip.first]++;
                        gcounts[dip.second]++;
                    }
                this->validate_gamete_counts(gcounts);
            }

          public:
            virtual ~slocuspop() = default;
            slocuspop(slocuspop &&) = default;
            slocuspop(const slocuspop &) = default;
            slocuspop &operator=(slocuspop &&) = default;
            slocuspop &operator=(const slocuspop &) = default;
            //! Population size
            uint_t N;

            using dipvector_t = dipvector;
            using diploid_t = typename dipvector::value_type;
            //! Typedef for base class
            using popbase_t = popbase<mutation_type, mcont, gcont, mvector,
                                      ftvector, lookup_table_type>;
            //! Dispatch tag for other parts of sugar layer
            using popmodel_t = sugar::SINGLELOC_TAG;
            //! Fitness function signature compatible with this type
            using fitness_t
                = fwdpp::traits::fitness_fxn_t<dipvector_t,
                                               typename popbase_t::gcont_t,
                                               typename popbase_t::mcont_t>;

            //! Container of diploids
            dipvector_t diploids;

            //! Constructor
            explicit slocuspop(
                const uint_t &popsize,
                typename popbase_t::gamete_t::mutation_container::size_type
                    reserve_size
                = 100)
                : popbase_t(2 * popsize, reserve_size), N(popsize),
                  // All N diploids contain the only gamete in the pop
                  diploids(dipvector_t(popsize, diploid_t(0, 0)))
            {
            }

            template <typename diploids_input, typename gametes_input,
                      typename mutations_input>
            explicit slocuspop(diploids_input &&d, gametes_input &&g,
                               mutations_input &&m)
                : popbase_t(std::forward<gametes_input>(g),
                            std::forward<mutations_input>(m), 100),
                  N{ static_cast<decltype(N)>(d.size()) },
                  diploids(std::forward<diploids_input>(d))
            //! Constructor for pre-determined population status
            {
                this->process_individual_input();
            }

            bool
            operator==(const slocuspop &rhs) const
            {
                return this->diploids == rhs.diploids
                       && popbase_t::is_equal(rhs);
            }

            //! Empty all the containers
            void
            clear()
            {
                diploids.clear();
                popbase_t::clear_containers();
            }
        };
    } // namespace sugar
} // namespace fwdpp
#endif
