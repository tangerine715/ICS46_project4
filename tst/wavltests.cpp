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
    REQUIRE(tree.size() == 4);
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

TEST_CASE("Tree:ExpectFindThrowsInEmpty", "[Required][Basic][Find]") {
    const proj4::WavlTree<int, std::string> tree;
    REQUIRE_THROWS_AS(tree.find(999), std::runtime_error);
}
TEST_CASE("Tree:DoAPreOrder:ExpectCorrectPreorderTraversal",
          "[Required][Basic][Insert][PreOrder]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    auto trav = tree.preOrder();
    static const std::vector<int> expected = {5, 3, 10};
    REQUIRE(trav == expected);
}

TEST_CASE("Tree:DoAPostOrder:ExpectCorrectInorderTraversal",
          "[Required][Basic][Insert][InOrder]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "sna");
    tree.insert(10, "bar");
    auto trav = tree.postOrder();
    static const std::vector<int> expected = {3, 10, 5};
    REQUIRE(trav == expected);
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

TEST_CASE("WAVL Tree: Basic Operations", "[wavl][basic]") {
    proj4::WavlTree<int, std::string> tree;

    SECTION("Empty tree") {
        REQUIRE(tree.size() == 0);
    }

    SECTION("Insert single element") {
        tree.insert(10, "ten");
        REQUIRE(tree.size() == 1);
        REQUIRE(tree.contains(10));
        REQUIRE(tree.find(10) == "ten");
        REQUIRE(tree.rank(10) == 1);
    }

    SECTION("Insert multiple elements Case 1 both right") {
        tree.insert(5, "foo");
        tree.insert(10, "bar");
        tree.insert(15, "str");

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(15));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(10) == 2);
        REQUIRE(tree.rank(15) == 1);
    }

    SECTION("Insert multiple elements Case 1 both left") {
        tree.insert(5, "foo");
        tree.insert(3, "bar");
        tree.insert(1, "str");

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(1));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(3) == 2);
        REQUIRE(tree.rank(1) == 1);
    }

    SECTION("Insert multiple elements Case 2 top right bottom left") {
        tree.insert(5, "foo");
        tree.insert(10, "bar");
        tree.insert(7, "str");

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(7));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(7) == 2);
        REQUIRE(tree.rank(10) == 1);
    }

    SECTION("Insert multiple elements Case 2 top left bottom right") {
        tree.insert(5, "foo");
        tree.insert(1, "bar");
        tree.insert(3, "str");

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(1));
        REQUIRE(tree.contains(3));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(3) == 2);
        REQUIRE(tree.rank(1) == 1);
    }

    SECTION("Insert multiple elements Case 3 both right") {
        tree.insert(5, "foo");
        tree.insert(3, "bar");
        tree.insert(12, "str");
        tree.insert(15, "zzz");
        tree.remove(3);

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));
        REQUIRE(!tree.contains(3));

        REQUIRE(tree.rank(5) == 1); //bruh, demoted bcuz 2-2 node...
        REQUIRE(tree.rank(12) == 3);
        REQUIRE(tree.rank(15) == 1);
    }

    SECTION("Insert multiple elements Case 3 both left") {
        tree.insert(5, "foo");
        tree.insert(3, "bar");
        tree.insert(12, "str");
        tree.insert(1, "zzz");
        tree.remove(12);

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(1));
        REQUIRE(!tree.contains(12));

        REQUIRE(tree.rank(5) == 1); //once again, 2-2 node demoted ;-;
        REQUIRE(tree.rank(3) == 3);
        REQUIRE(tree.rank(1) == 1);
    }

    SECTION("Insert multiple elements Case 4 top right bottom left") {
        tree.insert(5, "foo");
        tree.insert(3, "bar");
        tree.insert(12, "str");
        tree.insert(10, "zzz");
        tree.remove(3);

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(10));
        REQUIRE(!tree.contains(3));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(10) == 3);
        REQUIRE(tree.rank(12) == 1);
    }

    SECTION("Insert multiple elements Case 4 top left bottom right") {
        tree.insert(5, "foo");
        tree.insert(3, "bar");
        tree.insert(12, "str");
        tree.insert(4, "zzz");
        tree.remove(12);

        REQUIRE(tree.size() == 3);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(4));
        REQUIRE(!tree.contains(12));

        REQUIRE(tree.rank(5) == 1);
        REQUIRE(tree.rank(4) == 3);
        REQUIRE(tree.rank(3) == 1);
    }

    SECTION("Find throws for missing key") {
        REQUIRE_THROWS_AS(tree.find(999), std::runtime_error);
    }

    SECTION("Insert and remove elements") {
        tree.insert(5, "foo");
        tree.insert(10, "bar");
        tree.insert(15, "str");
        tree.remove(10);
        tree.remove(5);

        REQUIRE(tree.size() == 1);
        REQUIRE_FALSE(tree.contains(5));
        REQUIRE_FALSE(tree.contains(10));
        REQUIRE(tree.contains(15));

        REQUIRE(tree.rank(15) == 1);
    }
}

}  // namespace
