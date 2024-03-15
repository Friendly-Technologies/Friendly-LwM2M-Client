#include "catch_amalgamated.hpp"
#include "./../../../../../wpp/registry/objects/m_0_lwm2m_security_v11/Lwm2mSecurity.h"

using namespace wpp;

TEST_CASE("objectLwm2mSecurity", "[objectLwm2mSecurity]")
{
    SECTION("objectLwm2mSecurity")
    {
        class Lwm2mSecurityMock : public Lwm2mSecurity
        {
        public:
            Lwm2mSecurityMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : Lwm2mSecurity(context, id) {}
            void setDefaultState()
            {
                Lwm2mSecurity::setDefaultState();

                // return value.size() < SERVER_URI_MAX_SIZE
                REQUIRE(Lwm2mSecurity::resource(LWM2M_SERVER_URI_0)->set(STRING_T("")));
                REQUIRE_FALSE(Lwm2mSecurity::resource(LWM2M_SERVER_URI_0)->set(STRING_T("want-more-and-more-more-more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312more1231231231231231231231231231231231231231231231231231231231231312312312312312312312312312312")));

                // return LWM2M_SECURITY_MODE_PRE_SHARED_KEY <= value && value <= LWM2M_SECURITY_MODE_NONE
                REQUIRE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_PRE_SHARED_KEY)));
                REQUIRE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_PRE_SHARED_KEY - 1)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SECURITY_MODE_2)->set(INT_T(LWM2M_SECURITY_MODE_NONE + 1)));

                // return SINGLE_INSTANCE_ID < value && value < ID_T_MAX_VA
                REQUIRE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(1)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(SINGLE_INSTANCE_ID)));
                REQUIRE_FALSE(Lwm2mSecurity::resource(SHORT_SERVER_ID_10)->set(INT_T(ID_T_MAX_VAL)));
            }

            void serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mSecurity::serverOperationNotifier(type, resId); }
            void userOperationNotifier(ResOp::TYPE type, const ResLink &resId) { Lwm2mSecurity::userOperationNotifier(type, resId); }
        };

        lwm2m_context_t mockContext;
        OBJ_LINK_T mockId = {0, 1};
        // Create an instance of SecurityMock
        Lwm2mSecurityMock securityMock(mockContext, mockId);

        securityMock.setDefaultState();

        securityMock.serverOperationNotifier(ResOp::TYPE::READ, {0, 0});
        securityMock.userOperationNotifier(ResOp::TYPE::WRITE, {10, 10});
    }
}
