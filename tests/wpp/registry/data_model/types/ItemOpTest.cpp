// #include "catch_amalgamated.hpp"
// #include "ItemOp.h"

// using namespace wpp;

// TEST_CASE("ItemOp2", "[ItemOp2]") {
//     SECTION("Default Constructor") {
//         ItemOp op;
//         REQUIRE(op.getFlags() == 0);

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isDelete());        
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::NONE));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::READ));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::WRITE));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DELETE));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::EXECUTE));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::DISCOVER));
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::BLOCK_WRITE));
//         REQUIRE_FALSE(op.isSupported(ItemOp::TYPE::BLOCK_EXECUTE));
// #endif
//     }

//     SECTION("isRead Function") {
//         ItemOp op(ItemOp::TYPE::READ);
        
//         REQUIRE(op.isRead());
//         REQUIRE(op.getFlags() == 1);
//         REQUIRE(op.isSupported(ItemOp::TYPE::READ));

//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isDelete());
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//     }

//     SECTION("isWrite(WRITE_UPD) Function") {
//         ItemOp op(ItemOp::TYPE::WRITE);

//         REQUIRE(op.isWrite());
//         REQUIRE(op.getFlags() == 2);
//         // REQUIRE(op.isSupported(ItemOp::TYPE::WRITE_UPD)); 

//         REQUIRE_FALSE(op.getFlags() == 4);
//         REQUIRE_FALSE(op.getFlags() == 8);
//     }
//     SECTION("isWrite(WRITE_REPLACE_RES) Function") {
//         ItemOp op(ItemOp::TYPE::WRITE);

//         REQUIRE(op.isWrite());
//         REQUIRE(op.getFlags() == 4);
//         // REQUIRE(op.isSupported(ItemOp::TYPE::WRITE_REPLACE_RES));

//         REQUIRE_FALSE(op.getFlags() == 2);
//         REQUIRE_FALSE(op.getFlags() == 8);
//     }
//     SECTION("isWrite(WRITE_REPLACE_INST) Function") {
//         ItemOp op(ItemOp::TYPE::WRITE);

//         REQUIRE(op.isWrite());
//         REQUIRE(op.getFlags() == 8);
//         // REQUIRE(op.isSupported(ItemOp::TYPE::WRITE_REPLACE_INST));

//         REQUIRE_FALSE(op.getFlags() == 2);
//         REQUIRE_FALSE(op.getFlags() == 4);
//     }

//     SECTION("isWrite Function") {
//         ItemOp op(ItemOp::TYPE::WRITE);
        
//         REQUIRE(op.isWrite());
//         REQUIRE(op.getFlags() == 14);
//         REQUIRE(op.isSupported(ItemOp::TYPE::WRITE));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isDelete());
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//         REQUIRE_FALSE((op.getFlags() == 2));
//         REQUIRE_FALSE((op.getFlags() == 8));
//     }
//     SECTION("isExecute Function") {
//         ItemOp op(ItemOp::TYPE::EXECUTE);
        
//         REQUIRE(op.isExecute());
//         REQUIRE(op.getFlags() == 16);
//         REQUIRE(op.isSupported(ItemOp::TYPE::EXECUTE));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isDelete());
//         REQUIRE_FALSE(op.isDiscover());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//     }
//     SECTION("isDiscover Function") {
//         ItemOp op(ItemOp::TYPE::DISCOVER);

//         REQUIRE(op.isDiscover());
//         REQUIRE(op.getFlags() == 32);
//         REQUIRE(op.isSupported(ItemOp::TYPE::DISCOVER));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE( op.isDelete());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//     }
//     SECTION("isDelete Function") {
//         ItemOp op(ItemOp::TYPE::DELETE);

//         REQUIRE(op.isDelete());
//         REQUIRE(op.getFlags() == 64);
//         REQUIRE(op.isSupported(ItemOp::TYPE::DELETE));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE_FALSE(op.isBlockWrite());
//         REQUIRE_FALSE(op.isBlockExecute());
// #endif
//     }

// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//    SECTION("isBlockWrite Function") {
//         ItemOp op(ItemOp::TYPE::BLOCK_WRITE);

//         REQUIRE(op.isBlockWrite());
//         REQUIRE(op.getFlags() == 128);
//         REQUIRE(op.isSupported(ItemOp::TYPE::BLOCK_WRITE));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isDelete());
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
//         REQUIRE_FALSE(op.isBlockExecute());
//     }
//    SECTION("isBlockExecute Function") {
//         ItemOp op(ItemOp::TYPE::BLOCK_EXECUTE);

//         REQUIRE(op.isBlockExecute());
//         REQUIRE(op.getFlags() == 256);
//         REQUIRE(op.isSupported(ItemOp::TYPE::BLOCK_EXECUTE));

//         REQUIRE_FALSE(op.isRead());
//         REQUIRE_FALSE(op.isWrite());
//         REQUIRE_FALSE(op.isDelete());        
//         REQUIRE_FALSE(op.isExecute());
//         REQUIRE_FALSE(op.isDiscover());
//         REQUIRE_FALSE(op.isBlockWrite());
//     }
// #endif
//     SECTION("EXECUTE|WRITE Function") {
//         ItemOp op(ItemOp::TYPE::EXECUTE | ItemOp::TYPE::WRITE);

//         REQUIRE(op.isWrite());
//         REQUIRE(op.isExecute());
//         REQUIRE(op.getFlags() == 30);
//     }
//     SECTION("READ|WRITE_REPLACE_RES Function") {
//         ItemOp op(ItemOp::TYPE::READ | ItemOp::TYPE::WRITE);

//         REQUIRE(op.isRead());
//         REQUIRE(op.isWrite());
//         REQUIRE(op.getFlags() == 5);
//     }
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//     SECTION("DISCOVER|BLOCK_EXECUTE Function") {
//         ItemOp op(ItemOp::TYPE::DISCOVER | ItemOp::TYPE::BLOCK_EXECUTE);

//         REQUIRE(op.isDiscover());
//         REQUIRE(op.isBlockExecute());
//         REQUIRE(op.getFlags() == 288);
//     }
//     SECTION("BLOCK_WRITE|DELETE Function") {
//         ItemOp op(ItemOp::TYPE::BLOCK_WRITE | ItemOp::TYPE::DELETE | ItemOp::TYPE::WRITE_REPLACE_INST);
        
//         REQUIRE(op.isBlockWrite());
//         REQUIRE(op.isDelete());
//         REQUIRE(op.getFlags() == 200);
//     }
// #endif
//     SECTION("DELETE with duplicates TYPE Function") {
//         ItemOp op(ItemOp::TYPE::DELETE | ItemOp::TYPE::DELETE | ItemOp::TYPE::DELETE);
//                 REQUIRE(op.getFlags() == 64);
//     }

//     SECTION("isCompatible Function") {
//         ItemOp op1(ItemOp::TYPE::NONE        | ItemOp::TYPE::WRITE);
//         ItemOp op2(ItemOp::TYPE::WRITE       | ItemOp::TYPE::EXECUTE);
//         ItemOp op3(ItemOp::TYPE::DISCOVER    | ItemOp::TYPE::DELETE);

//         REQUIRE(op1.isCompatible(op2));
//         REQUIRE_FALSE(op2.isCompatible(op1));
//         REQUIRE_FALSE(op1.isCompatible(op3));
//         REQUIRE_FALSE(op2.isCompatible(op3));
//     }

//     SECTION("asVector Function") {
//         ItemOp op(
//             ItemOp::TYPE::NONE           |
//             ItemOp::TYPE::READ           |
//             ItemOp::TYPE::WRITE          |
//             ItemOp::TYPE::EXECUTE        |
//             ItemOp::TYPE::DISCOVER       |
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//             ItemOp::TYPE::BLOCK_EXECUTE  |
//             ItemOp::TYPE::BLOCK_WRITE    |
// #endif
//             ItemOp::TYPE::DELETE);
//         std::vector<ItemOp::TYPE> result = op.asVector();
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE(result.size() == 9);
// #else
//         REQUIRE(result.size() == 7);
// #endif
//         REQUIRE(result[0] == ItemOp::TYPE::READ);
//         REQUIRE(result[1] == ItemOp::TYPE::WRITE);
//         // REQUIRE(result[2] == ItemOp::TYPE::WRITE_REPLACE_RES);
//         // REQUIRE(result[3] == ItemOp::TYPE::WRITE_REPLACE_INST);
//         REQUIRE(result[4] == ItemOp::TYPE::EXECUTE);
//         REQUIRE(result[5] == ItemOp::TYPE::DISCOVER);
//         REQUIRE(result[6] == ItemOp::TYPE::DELETE);
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE(result[7] == ItemOp::TYPE::BLOCK_WRITE);
//         REQUIRE(result[8] == ItemOp::TYPE::BLOCK_EXECUTE);
// #endif
//     }

//     SECTION("asVector2 Function") {
//         ItemOp op(
//             ItemOp::TYPE::READ               |
//             ItemOp::TYPE::WRITE          |
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//             ItemOp::TYPE::BLOCK_WRITE        |
// #endif
//             ItemOp::TYPE::WRITE);
//         std::vector<ItemOp::TYPE> result = op.asVector();
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE(result.size() == 4);
// #else
//         REQUIRE(result.size() == 3);
// #endif
//         REQUIRE(result[0] == ItemOp::TYPE::READ);
//         REQUIRE(result[1] == ItemOp::TYPE::WRITE);
//         // REQUIRE(result[2] == ItemOp::TYPE::WRITE_REPLACE_INST);
// #ifdef LWM2M_RAW_BLOCK1_REQUESTS
//         REQUIRE(result[3] == ItemOp::TYPE::BLOCK_WRITE);
// #endif
//     }
// }