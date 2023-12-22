#include "catch_amalgamated.hpp"
#include "WppRegistry.h"

using namespace wpp;

TEST_CASE("WppRegistry constructor creates objects", "[WppRegistry]")
{
    // Mock the context or provide a real one if needed
    lwm2m_context_t context;
    // Create a WppRegistry instance
    WppRegistry WppRegistryInstance(context);

    SECTION("objId_func")
    {
        // Retrieve the objects created by the constructor
        OBJ_ID objId;

        const auto &createdObjects = WppRegistryInstance.isObjExist(objId);
        const auto &objectObjects = WppRegistryInstance.object(objId);
        const auto &objectNoObjects = WppRegistryInstance.object(OBJ_ID(25));

        REQUIRE(createdObjects);
        REQUIRE(objectObjects);
        REQUIRE_FALSE(objectNoObjects);
    }

    SECTION("DEVICE")
    {
        ObjectSpec<Device> &obj = WppRegistryInstance.device();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }

    SECTION("SERVER")
    {
        ObjectSpec<Lwm2mServer> &obj = WppRegistryInstance.lwm2mServer();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }

    SECTION("SECURITY")
    {
        ObjectSpec<Lwm2mSecurity> &obj = WppRegistryInstance.lwm2mSecurity();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }

    SECTION("CONNECTIVITY")
    {
        ObjectSpec<ConnectivityMonitoring> &obj = WppRegistryInstance.connectivityMonitoring();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }

    SECTION("ACCESS_CONTROL")
    {
        ObjectSpec<Lwm2mAccessControl> &obj = WppRegistryInstance.lwm2mAccessControl();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }

    SECTION("FIRMWARE_UPDATE")
    {
        ObjectSpec<FirmwareUpdate> &obj = WppRegistryInstance.firmwareUpdate();

        const auto &registerObjects = WppRegistryInstance.registerObj(obj);
        const auto &isObjRegistered = WppRegistryInstance.isObjRegistered(obj);
        const auto &deregisterObjects = WppRegistryInstance.deregisterObj(obj);
        const auto &isObjNoRegistered = WppRegistryInstance.isObjRegistered(obj);

        REQUIRE(registerObjects);
        REQUIRE(isObjRegistered);
        REQUIRE(deregisterObjects);
        REQUIRE_FALSE(isObjNoRegistered);
    }
}
