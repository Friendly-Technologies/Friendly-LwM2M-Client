#include "catch_amalgamated.hpp"
#include "ItemOp.h"

using namespace wpp;

TEST_CASE("ItemOp", "[ItemOp]")
{
    SECTION("Default Constructor")
    {
        ItemOp op;

        REQUIRE(op.getFlags() == 0);
        REQUIRE(op.isCompatible(ItemOp::TYPE::CREATE));

        REQUIRE_FALSE(op.isCreate());
        REQUIRE_FALSE(op.isDelete());
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::CREATE));
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DELETE));
    }

    SECTION("NONE Function")
    {
        ItemOp op(ItemOp::TYPE::NONE);

        REQUIRE(op.getFlags() == 0);
        REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
    }

    SECTION("isCreate Function")
    {
        ItemOp op(ItemOp::TYPE::CREATE);

        REQUIRE(op.isSupported(ItemOp::TYPE::CREATE));
        REQUIRE(op.isCreate());
        REQUIRE(op.getFlags() == 16);

        REQUIRE_FALSE(op.isDelete());
    }

    SECTION("isDelete Function")
    {
        ItemOp op(ItemOp::TYPE::DELETE);

        REQUIRE(op.isSupported(ItemOp::TYPE::DELETE));
        REQUIRE(op.isDelete());
        REQUIRE(op.getFlags() == ' ');

        REQUIRE_FALSE(op.isCreate());
    }

    SECTION("isCompatible Function")
    {
        ItemOp op1(ItemOp::TYPE::NONE | ItemOp::TYPE::CREATE);
        ItemOp op2(ItemOp::TYPE::CREATE | ItemOp::TYPE::DELETE);

        REQUIRE(op1.isCompatible(op2));
        REQUIRE_FALSE(op2.isCompatible(op1));

        SECTION("asVector Function")
        {
            ItemOp op(
                ItemOp::TYPE::NONE |
                ItemOp::TYPE::CREATE |
                ItemOp::TYPE::DELETE);
            std::vector<ItemOp::TYPE> result = op.asVector();
            REQUIRE(result.size() == 2);
            REQUIRE(result[0] == ItemOp::TYPE::CREATE);
            REQUIRE(result[1] == ItemOp::TYPE::DELETE);
        }
    }
}