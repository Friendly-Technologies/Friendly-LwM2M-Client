#include "catch_amalgamated.hpp"
#include "ItemOp.h"

using namespace wpp;

TEST_CASE("ItemOp2", "[ItemOp2]")
{
    SECTION("Default Constructor")
    {
        ItemOp op;
        REQUIRE(op.getFlags() == 0);

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());

        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::READ));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::WRITE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DELETE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::EXECUTE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DISCOVER));
    }

    SECTION("isRead Function")
    {
        ItemOp op(ItemOp::TYPE::READ);

        REQUIRE(op.isRead());
        REQUIRE(op.getFlags() == 1);
        REQUIRE(op.isSupported(ItemOp::TYPE::READ));

        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
    }

    SECTION("isWrite Function")
    {
        ItemOp op(ItemOp::TYPE::WRITE);

        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 2);
        REQUIRE(op.isSupported(ItemOp::TYPE::WRITE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
        REQUIRE_FALSE((op.getFlags() == 1));
        REQUIRE_FALSE((op.getFlags() == 8));
    }

    SECTION("isExecute Function")
    {
        ItemOp op(ItemOp::TYPE::EXECUTE);

        REQUIRE(op.isExecute());
        REQUIRE(op.getFlags() == 4);
        REQUIRE(op.isSupported(ItemOp::TYPE::EXECUTE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isDiscover());
    }

    SECTION("isDiscover Function")
    {
        ItemOp op(ItemOp::TYPE::DISCOVER);

        REQUIRE(op.isDiscover());
        REQUIRE(op.getFlags() == 8);
        REQUIRE(op.isSupported(ItemOp::TYPE::DISCOVER));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDelete());
    }

    SECTION("isCreate Function")
    {
        ItemOp op(ItemOp::TYPE::CREATE);

        REQUIRE_FALSE(op.isDiscover());
        REQUIRE(op.getFlags() == 16);
        REQUIRE(op.isSupported(ItemOp::TYPE::CREATE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDelete());
    }

    SECTION("isDelete Function")
    {
        ItemOp op(ItemOp::TYPE::DELETE);

        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 32);
        REQUIRE(op.isSupported(ItemOp::TYPE::DELETE));

        REQUIRE_FALSE(op.isRead());
        REQUIRE_FALSE(op.isWrite());
        REQUIRE_FALSE(op.isExecute());
        REQUIRE_FALSE(op.isDiscover());
    }

    SECTION("EXECUTE|WRITE Function")
    {
        ItemOp op(ItemOp::TYPE::EXECUTE | ItemOp::TYPE::WRITE);

        REQUIRE(op.isWrite());
        REQUIRE(op.isExecute());
        REQUIRE(op.getFlags() == 6);
    }

    SECTION("READ|WRITE_REPLACE_RES Function")
    {
        ItemOp op(ItemOp::TYPE::READ | ItemOp::TYPE::WRITE);

        REQUIRE(op.isRead());
        REQUIRE(op.isWrite());
        REQUIRE(op.getFlags() == 3);
    }

    SECTION("DISCOVER|EXECUTE Function")
    {
        ItemOp op(ItemOp::TYPE::DISCOVER | ItemOp::TYPE::CREATE);

        REQUIRE(op.isDiscover());
        REQUIRE(op.isCreate());
        REQUIRE(op.getFlags() == 24);
    }
    SECTION("WRITE|DELETE Function")
    {
        ItemOp op(ItemOp::TYPE::WRITE | ItemOp::TYPE::DELETE);

        REQUIRE(op.isWrite());
        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == 34);
    }
    SECTION("DELETE with duplicates TYPE Function")
    {
        ItemOp op(
            ItemOp::TYPE::DELETE |
            ItemOp::TYPE::DELETE |
            ItemOp::TYPE::DELETE);
        REQUIRE(op.getFlags() == 32);
    }

    SECTION("isCompatible Function")
    {
        ItemOp op1(ItemOp::TYPE::NONE | ItemOp::TYPE::WRITE);
        ItemOp op2(ItemOp::TYPE::WRITE | ItemOp::TYPE::EXECUTE);
        ItemOp op3(ItemOp::TYPE::DISCOVER | ItemOp::TYPE::DELETE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op2.isCompatible(op1));
        REQUIRE_FALSE(op1.isCompatible(op3));
        REQUIRE_FALSE(op2.isCompatible(op3));
    }

    SECTION("asVector Function")
    {
        ItemOp op(
            ItemOp::TYPE::NONE |
            ItemOp::TYPE::READ |
            ItemOp::TYPE::WRITE |
            ItemOp::TYPE::EXECUTE |
            ItemOp::TYPE::DISCOVER |
            ItemOp::TYPE::DELETE);
        std::vector<ItemOp::TYPE> result = op.asVector();
        REQUIRE(result.size() == 5);
        REQUIRE(result[0] == ItemOp::TYPE::READ);
        REQUIRE(result[1] == ItemOp::TYPE::WRITE);
        REQUIRE(result[2] == ItemOp::TYPE::EXECUTE);
        REQUIRE(result[3] == ItemOp::TYPE::DISCOVER);
        REQUIRE(result[4] == ItemOp::TYPE::DELETE);
    }

    SECTION("asVector2 Function")
    {
        ItemOp op(
            ItemOp::TYPE::READ |
            ItemOp::TYPE::WRITE |
            ItemOp::TYPE::WRITE);
        std::vector<ItemOp::TYPE> result = op.asVector();
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == ItemOp::TYPE::READ);
        REQUIRE(result[1] == ItemOp::TYPE::WRITE);
        // REQUIRE(result[2] == ItemOp::TYPE::EXECUTE);
    }
}
