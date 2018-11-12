#ifndef FWDPP_TS_GENERATE_DATA_MATRIX_HPP
#define FWDPP_TS_DATA_GENERATE_MATRIX_HPP

#include <vector>
#include <type_traits>
#include <cstdint>
#include <stdexcept>
#include <fwdpp/data_matrix.hpp>
#include "table_collection.hpp"
#include "tree_visitor.hpp"
#include "detail/generate_data_matrix_details.hpp"

namespace fwdpp
{
    namespace ts
    {
        template <typename mcont_t>
        data_matrix
        generate_data_matrix(const table_collection& tables,
                             const std::vector<TS_NODE_INT>& samples,
                             const mcont_t& mutations,
                             const bool record_neutral,
                             const bool record_selected)
        /// \todo Document
        /// \version 0.7.0 Added to library
        /// \version 0.7.1 Change behavior to skip sites fixed in the sample
        {
            auto mut = tables.mutation_table.cbegin();
            const auto mut_end = tables.mutation_table.cend();
            tree_visitor tv(tables, samples);
            std::vector<std::int8_t> genotypes(samples.size(), 0);
            data_matrix rv(samples.size());
            while (tv(std::true_type(), std::true_type()))
                {
                    // Advance the mutation table records until we are
                    // in the current tree
                    const auto& tree = tv.tree();
                    while (mut < mut_end
                           && mutations[mut->key].pos < tree.left)
                        {
                            ++mut;
                        }
                    // Process mutations on this tree
                    for (;
                         mut < mut_end && mutations[mut->key].pos < tree.right;
                         ++mut)
                        {
                            auto tc = tree.leaf_counts[mut->node]
                                      + tree.preserved_leaf_counts[mut->node];
                            if (tc < tree.sample_size)
                                {
                                    // Mutation leads to a polymorphism
                                    bool is_neutral
                                        = mutations[mut->key].neutral;
                                    if ((is_neutral && record_neutral)
                                        || (!is_neutral && record_selected))
                                        {
                                            auto index
                                                = tree.left_sample[mut->node];
                                            // Check if mutation leads to a sample
                                            if (index != TS_NULL_NODE)
                                                {
                                                    detail::process_samples(
                                                        tree, mut->node, index,
                                                        genotypes);
                                                    // Update our return value
                                                    detail::update_data_matrix(
                                                        mutations, mut->key,
                                                        genotypes, rv);
                                                }
                                        }
                                }
                        }
                    // Terminate early if we're lucky
                    if (!(mut < mut_end))
                        {
                            break;
                        }
                }
            return rv;
        }
    } // namespace ts
} // namespace fwdpp

#endif
