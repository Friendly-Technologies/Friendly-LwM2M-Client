#include "catch_amalgamated.hpp"
#include "WppRegistry.h"

using namespace wpp;

TEST_CASE("WppRegistry constructor creates objects", "[WppRegistry]")
{
    // Mock the context or provide a real one if needed
    lwm2m_context_t context;
    // Create a WppRegistry instance
    WppRegistry WppRegistryInstance(context);

    SECTION("objID")
    {
        REQUIRE_FALSE(WppRegistryInstance.isExist(OBJ_ID(-1)));
        REQUIRE_FALSE(WppRegistryInstance.object(OBJ_ID(-1)) != NULL);
        WppRegistryInstance.objects(); // TODO
    }

#ifdef OBJ_M_3_DEVICE
    SECTION("DEVICE")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::DEVICE));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::DEVICE) != NULL);

        Object &obj = WppRegistryInstance.device();
        WppRegistryInstance.objects(); // TODO
        WppRegistryInstance.audioClip(); // TODO
        REQUIRE(obj.getObjectID() == OBJ_ID(3));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(3)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_M_1_LWM2M_SERVER
    SECTION("SERVER")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::LWM2M_SERVER));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::LWM2M_SERVER) != NULL);

        Object &obj = WppRegistryInstance.lwm2mServer();

        REQUIRE(obj.getObjectID() == OBJ_ID(1));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(1)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_M_0_LWM2M_SECURITY
    SECTION("SECURITY")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::LWM2M_SECURITY));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::LWM2M_SECURITY) != NULL);

        Object &obj = WppRegistryInstance.lwm2mSecurity();

        REQUIRE(obj.getObjectID() == OBJ_ID(0));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(0)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
    SECTION("CONNECTIVITY")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::CONNECTIVITY_MONITORING));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::CONNECTIVITY_MONITORING) != NULL);

        Object &obj = WppRegistryInstance.connectivityMonitoring();

        REQUIRE(obj.getObjectID() == OBJ_ID(4));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(4)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
    SECTION("ACCESS_CONTROL")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::LWM2M_ACCESS_CONTROL));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::LWM2M_ACCESS_CONTROL) != NULL);

        Object &obj = WppRegistryInstance.lwm2mAccessControl();

        REQUIRE(obj.getObjectID() == OBJ_ID(2));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(2)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_5_FIRMWARE_UPDATE
    SECTION("FIRMWARE_UPDATE")
    {
        REQUIRE(WppRegistryInstance.isExist(OBJ_ID::FIRMWARE_UPDATE));
        REQUIRE(WppRegistryInstance.object(OBJ_ID::FIRMWARE_UPDATE) != NULL);

        Object &obj = WppRegistryInstance.firmwareUpdate();

        REQUIRE(obj.getObjectID() == OBJ_ID(5));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(5)) == &obj);
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif
}
