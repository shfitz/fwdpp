//Copied from fwdpy11_arg_example.
//Author: KRT
//License: GPL3+
#ifndef FWDPP_TS_EDGE_HPP
#define FWDPP_TS_EDGE_HPP

#include <tuple>
#include "definitions.hpp"

namespace fwdpp
{
    namespace ts
    {
        struct edge
        /// \brief An edge in a tree sequence
        ///
        /// Edges define a transmission event
        /// of the genomic interval [left,right)
        /// from parent to child.
        ///
        ///  \version 0.7.0 Added to fwdpp
        {
            /// Left edge of interval, inclusive
            double left;
            /// Right edge of interval, exclusive
            double right;
            /// Parent of interval
            TS_NODE_INT parent;
            /// Child of interval
            TS_NODE_IN Tchild;
        };
        inline bool
        operator==(const edge& a, const edge& b)
        /// Comparison operator
        {
            return std::tie(a.parent, a.child, a.left, a.right)
                   == std::tie(b.parent, b.child, b.left, b.right);
        }
    } // namespace ts
} // namespace fwdpp

#endif
