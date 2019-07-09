#include <iostream>
#include <boost/test/unit_test.hpp>
#include "simple_table_collection.hpp"
#include "simple_table_collection_polytomy.hpp"
#include <fwdpp/ts/tree_visitor.hpp>
#include <fwdpp/ts/decapitate.hpp>
#include <fwdpp/ts/marginal_tree_functions.hpp>

BOOST_FIXTURE_TEST_SUITE(test_simple_table_collection_traversal,
                         simple_table_collection)

BOOST_AUTO_TEST_CASE(test_construction)
{
    BOOST_REQUIRE_NO_THROW(
        { fwdpp::ts::node_iterator(tv.tree(), fwdpp::ts::nodes_preorder()); });
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal)
{
    fwdpp::ts::node_iterator ni(tv.tree(), fwdpp::ts::nodes_preorder());
    std::vector<fwdpp::ts::TS_NODE_INT> nodes;
    auto n = ni();
    while (n != fwdpp::ts::TS_NULL_NODE)
        {
            nodes.push_back(n);
            n = ni();
        }
    BOOST_REQUIRE_EQUAL(
        nodes == std::vector<fwdpp::ts::TS_NODE_INT>({ 6, 4, 0, 1, 5, 2, 3 }),
        true);
    auto nodes_from_fxn
        = fwdpp::ts::get_nodes(tv.tree(), fwdpp::ts::nodes_preorder());
    BOOST_REQUIRE_EQUAL(nodes == nodes_from_fxn, true);
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal_subtree)
{
    auto n = fwdpp::ts::get_nodes(tv.tree(), 5, fwdpp::ts::nodes_preorder());
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes{ 5, 2, 3 };
    BOOST_REQUIRE_EQUAL(n.size(), expected_nodes.size());
    BOOST_REQUIRE_EQUAL(n == expected_nodes, true);
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal_after_decaptitation)
{
    fwdpp::ts::decapitate(tables, 1.);
    tv = fwdpp::ts::tree_visitor(tables, samples,
                                 fwdpp::ts::update_samples_list(false));
    tv();
    auto nodes = fwdpp::ts::get_nodes(tv.tree(), fwdpp::ts::nodes_preorder());
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes = { 4, 0, 1, 2, 3 };
    BOOST_REQUIRE_EQUAL(nodes.size(), expected_nodes.size());
    BOOST_REQUIRE_EQUAL(nodes.size() == expected_nodes.size(), true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(test_simple_table_collection_polytomy_traversal,
                         simple_table_collection_polytomy)

BOOST_AUTO_TEST_CASE(test_preorder_traversal)
{
    auto n = fwdpp::ts::get_nodes(tv.tree(), fwdpp::ts::nodes_preorder());
    BOOST_REQUIRE_EQUAL(n.size(), tables.num_nodes());
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes{
        7, 5, 0, 1, 2, 6, 3, 4
    };
    BOOST_REQUIRE_EQUAL(n == expected_nodes, true);
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal_subtree)
{
    auto n = fwdpp::ts::get_nodes(tv.tree(), 5, fwdpp::ts::nodes_preorder());
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes{ 5, 0, 1, 2 };
    BOOST_REQUIRE_EQUAL(n == expected_nodes, true);
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal_other_subtree)
{
    auto n = fwdpp::ts::get_nodes(tv.tree(), 6, fwdpp::ts::nodes_preorder());
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes{ 6, 3, 4 };
    BOOST_REQUIRE_EQUAL(n == expected_nodes, true);
}

BOOST_AUTO_TEST_CASE(test_preorder_traversal_after_decaptitation)
{
    fwdpp::ts::decapitate(tables, 0.);
    tv = fwdpp::ts::tree_visitor(tables, samples,
                                 fwdpp::ts::update_samples_list(false));
    tv();
    auto nodes = fwdpp::ts::get_nodes(tv.tree(), fwdpp::ts::nodes_preorder());
    for (auto n : nodes)
        {
            std::cout << n << ' ';
        }
    std::cout << '\n';
    std::vector<fwdpp::ts::TS_NODE_INT> expected_nodes
        = { 5, 0, 1, 2, 6, 3, 4 };
    BOOST_REQUIRE_EQUAL(nodes.size(), expected_nodes.size());
    BOOST_REQUIRE_EQUAL(nodes.size() == expected_nodes.size(), true);
}

BOOST_AUTO_TEST_SUITE_END()

