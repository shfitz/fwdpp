#ifndef __FWDPP_INTERNAL_MULTILOCUS_REC_HPP__
#define __FWDPP_INTERNAL_MULTILOCUS_REC_HPP__

/*
  The mechanics of crossing over for a multilocus
  simulation
 */
#include <gsl/gsl_rng.h>
#include <fwdpp/mutate_recombine.hpp>

namespace fwdpp
{
    namespace fwdpp_internal
    {

        /*!
          Mechanics of segregation, recombination, and mutation for multi-locus
          API
        */
        template <typename diploid_type,
                  typename recombination_policy_container, typename mqueue_t,
                  typename gqueue_t, typename mcont_t, typename gcont_t,
                  typename mutation_model_container>
        diploid_type
        multilocus_rec_mut(
            const gsl_rng *r, const diploid_type &parent1,
            const diploid_type &parent2, mqueue_t &mutation_recycling_bin,
            gqueue_t &gamete_recycling_bin,
            const recombination_policy_container &rec_pols,
            const std::vector<std::function<unsigned(void)>> &interlocus_rec,
            const int iswitch1, const int iswitch2, gcont_t &gametes,
            mcont_t &mutations,
            typename gcont_t::value_type::mutation_container &neutral,
            typename gcont_t::value_type::mutation_container &selected,
            const double *mu, const mutation_model_container &mmodel)
        {
            diploid_type offspring(parent1.size());
            unsigned s1 = iswitch1, s2 = iswitch2;
            auto NLOOPS = parent1.size();
            auto p1 = parent1.data();
            auto p2 = parent2.data();
            auto o = offspring.data();
            decltype(NLOOPS) i = 0;
            while (i < NLOOPS)
                {
                    if (i)
                        {
                            // between-locus rec, parent 1
                            s1 += interlocus_rec[i - 1]();
                            // between-locus rec, parent 2
                            s2 += interlocus_rec[i - 1]();
                        }
                    auto p1g1 = p1->first;
                    auto p1g2 = p1->second;
                    auto p2g1 = p2->first;
                    auto p2g2 = p2->second;
                    // if ttl # recs before now is odd, swap parental pointers
                    if (s1 % 2 != 0.)
                        {
                            std::swap(p1g1, p1g2);
                        }
                    if (s2 % 2 != 0.)
                        {
                            std::swap(p2g1, p2g2);
                        }

                    auto rm = mutate_recombine_update(
                        r, gametes, mutations,
                        std::make_tuple(p1g1, p1g2, p2g1, p2g2), rec_pols[i],
                        mmodel[i], mu[i], gamete_recycling_bin,
                        mutation_recycling_bin, *o, neutral, selected);
                    s1 += std::get<0>(rm);
                    s2 += std::get<1>(rm);
// mechanics of recombination
// auto xx = recombination(gametes, gamete_recycling_bin,
//                        neutral, selected, rec_pols[i],
//                        p1g1, p1g2, mutations);
// o->first = xx.first;
// s1 += xx.second;
// xx = recombination(gametes, gamete_recycling_bin, neutral,
//                   selected, rec_pols[i], p2g1, p2g2,
//                   mutations);
// o->second = xx.first;
// s2 += xx.second;
#ifndef FWDPP_UNIT_TESTING
// gametes[o->first].n++;
// gametes[o->second].n++;
// o->first = mutate_gamete_recycle(
//    mutation_recycling_bin, gamete_recycling_bin, r, mu[i],
//    gametes, mutations, o->first, mmodel[i], gpolicy_mut);
// o->second = mutate_gamete_recycle(
//    mutation_recycling_bin, gamete_recycling_bin, r, mu[i],
//    gametes, mutations, o->second, mmodel[i], gpolicy_mut);
#endif
                    ++i;
                    ++p1;
                    ++p2;
                    ++o;
                }
            return offspring;
        }
    } // namespace fwdpp_internal
} // namespace fwdpp

#endif
