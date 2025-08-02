#include <WavlTree.hpp>
#include <catch2/catch_amalgamated.hpp>
#include <optional>

namespace {
namespace proj4 = shindler::ics46::project4;

/* Remember, these are prerequisites to having us grade
    the related part of the assignment.  These are not
    worth points on their own, nor are they comprehensive.

    REMEMBER TO TEST YOUR OWN CODE COMPREHENSIVELY.

*/

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,
// readability-magic-numbers)
TEST_CASE("Tree:SimpleInsertAndFind:ExpectSingleInsertFound",
          "[Required][Basic][Insert][Contains]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");

    REQUIRE(tree.contains(5));
}

TEST_CASE("Tree:DoubleInsert:ExpectFindSecondInsert",
          "[Required][Basic][Insert][Contains]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(10, "bar");

    REQUIRE(tree.contains(10));
}

TEST_CASE("Tree:QuadInsert:ExpectFindFindTwoHopsAway",
          "[Required][Basic][Insert][Contains]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    tree.insert(12, "twelve");

    REQUIRE(tree.contains(12));
}

TEST_CASE("Tree:5Inserts:ExpectSize5", "[Required][Basic][Insert][Size]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    tree.insert(12, "twelve");
    tree.insert(15, "fifteen");

    REQUIRE(tree.size() == 5);
}

TEST_CASE("Tree:DoAPostOrder:ExpectCorrectPostorderTraversal",
          "[Required][Basic][Insert][PostOrder]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    auto trav = tree.postOrder();
    static const std::vector<int> expected = {3, 10, 5};
    REQUIRE(trav == expected);
}

// TEST_CASE("Tree:RemoveDoesARemove:ExpectElementsNotInTreeAndRanksCorrect",
//           "[Required][WavlTree][Insert][Erase][Contains][Rank]") {
//     proj4::WavlTree<int, std::string> tree;
//     tree.insert(20, "are");
//     tree.insert(19, "you");
//     tree.insert(18, "following");
//     tree.insert(12, "from");
//     tree.insert(9, "lecture?");
//     REQUIRE(tree.contains(20));
//     REQUIRE(tree.contains(19));
//     REQUIRE(tree.contains(18));
//     REQUIRE(tree.contains(12));
//     REQUIRE(tree.contains(9));
//     tree.remove(9);
//     tree.remove(18);
//     // What happens?  12 is now a leaf and must fall.  But 19 is a (2,2) and
//     // remains at rank 3.
//     REQUIRE(!tree.contains(9));
//     REQUIRE(!tree.contains(18));
//     REQUIRE(tree.rank(20) == 1);
//     REQUIRE(tree.rank(12) == 1);
//     REQUIRE(tree.rank(19) == 3);
// }
// // NOLINTEND

}  // namespace
