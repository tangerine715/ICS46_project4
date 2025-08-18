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

TEST_CASE("Tree:ExpectDeleteOneElementRootEmpty",
          "[Required][Basic][Remove][Empty]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.remove(5);

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

TEST_CASE("Tree:ExpectDeleteOneElementRootOne",
          "[Required][Basic][Insert][Contains]") {
    proj4::WavlTree<int, std::string> tree;
    tree.insert(5, "foo");
    tree.insert(3, "eee");
    tree.remove(5);

    REQUIRE(tree.contains(3));
    REQUIRE(!tree.contains(5));
    REQUIRE(tree.rank(3) == 1);
    REQUIRE(tree.size() == 1);
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

    SECTION("Insert multiple elements below root Case 1 both right") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(12, "twelve");
        tree.insert(15, "fifteen");

        REQUIRE(tree.size() == 5);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));

        REQUIRE(tree.rank(5) == 3);
        REQUIRE(tree.rank(3) == 1);
        REQUIRE(tree.rank(10) == 1);
        REQUIRE(tree.rank(15) == 1);
        REQUIRE(tree.rank(12) == 2);
    }

    SECTION("Insert multiple elements below root Case 3 both right") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(2, "eee");
        tree.insert(12, "twelve");
        tree.insert(8, "eight");
        tree.insert(15, "fifteen");
        tree.remove(8);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(2));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));
        REQUIRE(!tree.contains(8));

        REQUIRE(tree.rank(5) == 4);
        REQUIRE(tree.rank(3) == 2);
        REQUIRE(tree.rank(2) == 1);
        REQUIRE(tree.rank(10) == 1);
        REQUIRE(tree.rank(15) == 1);
        REQUIRE(tree.rank(12) == 3);
    }

    SECTION("Insert multiple elements at root with kids Case 3 both left") {
        tree.insert(20, "foo");
        tree.insert(10, "sna");
        tree.insert(30, "bar");
        tree.insert(5, "eee");
        tree.insert(28, "twelve");
        tree.insert(15, "eight");
        tree.insert(3, "fifteen");
        tree.remove(30);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(20));
        REQUIRE(tree.contains(28));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(15));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(5));
        REQUIRE(!tree.contains(30));

        REQUIRE(tree.rank(10) == 4);
        REQUIRE(tree.rank(20) == 3);
        REQUIRE(tree.rank(5) == 2);
        REQUIRE(tree.rank(3) == 1);
        REQUIRE(tree.rank(15) == 1);
        REQUIRE(tree.rank(28) == 1);
    }

    SECTION("Insert multiple elements at root with kids Case 3 both left, char version") {
        tree.insert('t', "foo");
        tree.insert('j', "sna");
        tree.insert('z', "bar");
        tree.insert('e', "eee");
        tree.insert('x', "twelve");
        tree.insert('o', "eight");
        tree.insert('c', "fifteen");
        tree.remove('z');

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains('t'));
        REQUIRE(tree.contains('x'));
        REQUIRE(tree.contains('j'));
        REQUIRE(tree.contains('o'));
        REQUIRE(tree.contains('c'));
        REQUIRE(tree.contains('e'));
        REQUIRE(!tree.contains('z'));

        REQUIRE(tree.rank('j') == 4);
        REQUIRE(tree.rank('t') == 3);
        REQUIRE(tree.rank('e') == 2);
        REQUIRE(tree.rank('c') == 1);
        REQUIRE(tree.rank('o') == 1);
        REQUIRE(tree.rank('x') == 1);
    }

    SECTION("Insert multiple elements below root Case 4 top right bottom left") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(2, "eee");
        tree.insert(12, "twelve");
        tree.insert(8, "eight");
        tree.insert(9, "fifteen");
        tree.remove(12);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(2));
        REQUIRE(tree.contains(9));
        REQUIRE(tree.contains(8));
        REQUIRE(!tree.contains(12));

        REQUIRE(tree.rank(5) == 4);
        REQUIRE(tree.rank(3) == 2);
        REQUIRE(tree.rank(2) == 1);
        REQUIRE(tree.rank(10) == 1);
        REQUIRE(tree.rank(9) == 3);
        REQUIRE(tree.rank(8) == 1);
    }

    SECTION("Insert multiple elements below root Case 4 top left bottom right") {
        tree.insert(5, "foo");
        tree.insert(4, "sna");
        tree.insert(10, "bar");
        tree.insert(1, "eee");
        tree.insert(2, "eee");
        tree.insert(8, "twelve");
        tree.insert(3, "eight");
        tree.remove(1);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(4));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(2));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(8));
        REQUIRE(!tree.contains(1));

        REQUIRE(tree.rank(5) == 4);
        REQUIRE(tree.rank(4) == 1);
        REQUIRE(tree.rank(10) == 2);
        REQUIRE(tree.rank(2) == 1);
        REQUIRE(tree.rank(3) == 3);
        REQUIRE(tree.rank(8) == 1);
    }

    SECTION("Delete root large case") {
        tree.insert(5, "foo");
        tree.insert(4, "sna");
        tree.insert(10, "bar");
        tree.insert(1, "eee");
        tree.insert(2, "eee");
        tree.insert(8, "twelve");
        tree.insert(3, "eight");
        tree.remove(5);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(1));
        REQUIRE(tree.contains(4));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(2));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(8));
        REQUIRE(!tree.contains(5));

        REQUIRE(tree.rank(4) == 4);
        REQUIRE(tree.rank(1) == 1);
        REQUIRE(tree.rank(10) == 1);
        REQUIRE(tree.rank(2) == 2);
        REQUIRE(tree.rank(3) == 1);
        REQUIRE(tree.rank(8) == 2);
    }

    SECTION("Insert multiple elements below root Case 1 both right") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(12, "twelve");
        tree.insert(15, "fifteen");

        REQUIRE(tree.size() == 5);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));

        REQUIRE(tree.rank(5) == 3);
        REQUIRE(tree.rank(3) == 1);
        REQUIRE(tree.rank(10) == 1);
        REQUIRE(tree.rank(15) == 1);
        REQUIRE(tree.rank(12) == 2);
    }

    SECTION("Insert multiple elements above root Case 3 both right") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(2, "eee");
        tree.insert(12, "twelve");
        tree.insert(8, "eight");
        tree.insert(15, "fifteen");
        tree.remove(3);

        REQUIRE(tree.size() == 6);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(8));
        REQUIRE(tree.contains(10));
        REQUIRE(tree.contains(2));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));
        REQUIRE(!tree.contains(3));

        REQUIRE(tree.rank(10) == 4);
        REQUIRE(tree.rank(5) == 3);
        REQUIRE(tree.rank(2) == 1);
        REQUIRE(tree.rank(8) == 1);
        REQUIRE(tree.rank(12) == 2);
        REQUIRE(tree.rank(15) == 1);
    }

    SECTION("Random double case one") {
        tree.insert(5, "foo");
        tree.insert(3, "sna");
        tree.insert(10, "bar");
        tree.insert(12, "twelve");
        tree.insert(15, "fifteen");
        tree.insert(17, "fifteen");
        tree.remove(10);

        REQUIRE(tree.size() == 5);
        REQUIRE(tree.contains(5));
        REQUIRE(tree.contains(3));
        REQUIRE(tree.contains(12));
        REQUIRE(tree.contains(15));
        REQUIRE(tree.contains(17));
        REQUIRE(!tree.contains(10));

        REQUIRE(tree.rank(3) == 1);
        REQUIRE(tree.rank(17) == 1);
        REQUIRE(tree.rank(15) == 2);
        REQUIRE(tree.rank(12) == 3);
        REQUIRE(tree.rank(5) == 2);
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
