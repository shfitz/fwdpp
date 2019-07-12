#ifndef FWDPP_TS_GENERATE_DATA_MATRIX_DETAILS_HPP
#define FWDPP_TS_GENERATE_DATA_MATRIX_DETAILS_HPP

#include <cstdint>
#include <vector>
#include <algorithm>
#include <fwdpp/data_matrix.hpp>
#include "../marginal_tree.hpp"

namespace fwdpp
{
    namespace ts
    {
        namespace detail
        {
            inline void
            process_samples(const marginal_tree& marginal,
                            const TS_NODE_INT node, TS_NODE_INT index,
                            std::vector<std::int8_t>& genotypes)
            {
                auto right = marginal.right_sample[node];
                // Set all genotypes to ancestral state
                std::fill(genotypes.begin(), genotypes.end(), 0);
                while (true)
                    {
                        if (genotypes[index] == 1)
                            {
                                throw std::runtime_error("inconsist"
                                                         "ent "
                                                         "samples "
                                                         "list");
                            }
                        genotypes[index] = 1;
                        if (index == right)
                            {
                                break;
                            }
                        index = marginal.next_sample[index];
                    }
            }

            inline void
            update_data_matrix(const std::size_t key, const double position,
                               const bool neutral,
                               const std::vector<std::int8_t>& genotypes,
                               data_matrix& rv)
            {
                auto& sm = (neutral) ? rv.neutral : rv.selected;
                auto& k = (neutral) ? rv.neutral_keys : rv.selected_keys;
                sm.positions.push_back(position);
                sm.data.insert(sm.data.end(), genotypes.begin(),
                               genotypes.end());
                k.push_back(key);
            }
        } // namespace detail
    }     // namespace ts
} // namespace fwdpp

#endif
