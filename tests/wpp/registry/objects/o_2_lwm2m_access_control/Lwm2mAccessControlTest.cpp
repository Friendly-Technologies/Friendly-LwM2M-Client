#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/o_2_lwm2m_access_control/Lwm2mAccessControl.h"

#define AC_OBJ_ID_MIN 1
#define AC_OBJ_ID_MAX (ID_T_MAX_VAL-1)
#define AC_OBJ_INST_ID_MIN 0
#define AC_OBJ_INST_ID_MAX ID_T_MAX_VAL
#define AC_OWNER_MIN 0
#define AC_OWNER_MAX ID_T_MAX_VAL

using namespace wpp;

TEST_CASE("objectLwm2mAccessControl", "[objectLwm2mAccessControl]")
{
    SECTION("objectLwm2mAccessControl")
    {
        class Lwm2mAccessControlMock : public Lwm2mAccessControl
        {
        public:
            Lwm2mAccessControlMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mAccessControl(context, id) {}

            void setDefaultState()
            {
                // Lwm2mAccessControl::setDefaultState(); TODO

                // return AC_OBJ_ID_MIN <= value && value <= AC_OBJ_ID_MAX
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MAX)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MIN - 1)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_ID_0)->set(INT_T(AC_OBJ_ID_MAX + 1)));

                // return AC_OBJ_INST_ID_MIN <= value && value <= AC_OBJ_INST_ID_MAX
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MAX)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MIN - 1)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(OBJECT_INSTANCE_ID_1)->set(INT_T(AC_OBJ_INST_ID_MAX + 1)));

                #if RES_2_2
                REQUIRE(resource(ACL_2)->set(INT_T(0)));
                REQUIRE(resource(ACL_2)->set(INT_T(3)));
                REQUIRE_FALSE(resource(ACL_2)->set(INT_T(200)));
                #endif

                // return AC_OWNER_MIN <= value && value <= AC_OWNER_MAX
                REQUIRE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MIN)));
                REQUIRE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MAX)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MIN - 1)));
                REQUIRE_FALSE(Lwm2mAccessControl::resource(ACCESS_CONTROL_OWNER_3)->set(INT_T(AC_OWNER_MAX + 1)));
            }

            void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { Lwm2mAccessControl::serverOperationNotifier(securityInst, type, resId); }
            void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { Lwm2mAccessControl::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of Lwm2mAccessControlMock
        Lwm2mAccessControlMock accesscontrolMock(mockContext, mockId);

        accesscontrolMock.setDefaultState();

        accesscontrolMock.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
        accesscontrolMock.userOperationNotifier(ItemOp::TYPE::WRITE, {10, 10});
    }
}