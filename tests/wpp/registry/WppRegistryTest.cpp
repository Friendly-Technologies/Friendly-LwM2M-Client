#include "catch_amalgamated.hpp"
#include "WppRegistry.h"

using namespace wpp;

TEST_CASE("WppRegistry constructor creates objects", "[WppRegistry]")
{
    // Mock the context or provide a real one if needed
    lwm2m_context_t context;
    // Create a WppRegistry instance
    WppRegistry WppRegistryInstance(context);
    // Retrieve the objects created by the constructor
    OBJ_ID objID;

    SECTION("objID")
    {
        REQUIRE(WppRegistryInstance.isObjExist(objID));
        REQUIRE_FALSE(WppRegistryInstance.isObjExist(OBJ_ID(-1)));

        REQUIRE(WppRegistryInstance.object(objID));
        REQUIRE_FALSE(WppRegistryInstance.object(OBJ_ID(-1)));
    }

#ifdef OBJ_M_3_DEVICE_V12
    SECTION("DEVICE")
    {
        ObjectSpec<Device> &obj = WppRegistryInstance.device();

        REQUIRE(obj.getObjectID() == OBJ_ID(3));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(3)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_M_1_LWM2M_SERVER_V11
    SECTION("SERVER")
    {
        ObjectSpec<Lwm2mServer> &obj = WppRegistryInstance.lwm2mServer();

        REQUIRE(obj.getObjectID() == OBJ_ID(1));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(1)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_M_0_LWM2M_SECURITY_V11
    SECTION("SECURITY")
    {
        ObjectSpec<Lwm2mSecurity> &obj = WppRegistryInstance.lwm2mSecurity();

        REQUIRE(obj.getObjectID() == OBJ_ID(0));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(0)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING_V13
    SECTION("CONNECTIVITY")
    {
        ObjectSpec<ConnectivityMonitoring> &obj = WppRegistryInstance.connectivityMonitoring();

        REQUIRE(obj.getObjectID() == OBJ_ID(4));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(4)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL_V11
    SECTION("ACCESS_CONTROL")
    {
        ObjectSpec<Lwm2mAccessControl> &obj = WppRegistryInstance.lwm2mAccessControl();

        REQUIRE(obj.getObjectID() == OBJ_ID(2));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(2)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif

#ifdef OBJ_O_5_FIRMWARE_UPDATE_V11
    SECTION("FIRMWARE_UPDATE")
    {
        ObjectSpec<FirmwareUpdate> &obj = WppRegistryInstance.firmwareUpdate();

        REQUIRE(obj.getObjectID() == OBJ_ID(5));
        REQUIRE(WppRegistryInstance.object(OBJ_ID(5)));
        REQUIRE(WppRegistryInstance.registerObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == true);
        REQUIRE(WppRegistryInstance.deregisterObj(obj) == true);
        REQUIRE(WppRegistryInstance.isObjRegistered(obj) == false);
    }
#endif
}
