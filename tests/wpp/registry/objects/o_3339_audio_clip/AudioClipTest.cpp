#include "catch_amalgamated.hpp"
#include "o_3339_audio_clip/AudioClip.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppClient.h"

using namespace wpp;

TEST_CASE("AudioClip", "[clip][audio]")
{
    class AudioClipMock : public AudioClip
    {
    public:
        AudioClipMock(lwm2m_context_t &context, const OBJ_LINK_T &id) : AudioClip(context, id) {}
        void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resId) { AudioClip::serverOperationNotifier(securityInst, type, resId); }
        void userOperationNotifier(ItemOp::TYPE type, const ResLink &resId) { AudioClip::userOperationNotifier(type, resId); }
    };

    class WppConnectionMock : public WppConnection
    {
    public:
        SESSION_T connect(Lwm2mSecurity &security) override { return this; }
        void disconnect(SESSION_T session) override {}
        bool sessionCmp(SESSION_T session1, SESSION_T session2) override { return true; }
        bool sendPacket(const Packet &packet) override { return true; }
    };

    WppClient::ClientInfo clientInfo;
    OBJ_LINK_T mockId = {0, 0};

    WppConnectionMock conmock;
    lwm2m_context_t contextmock;
    WppClient *defclient = WppClient::takeOwnership();

    AudioClip(defclient->getContext(), mockId);
    AudioClipMock aClip(contextmock, mockId);
    defclient->giveOwnership();
    AudioClip::object(*defclient);

    REQUIRE(AudioClip::instance(*defclient) == nullptr);
    REQUIRE(AudioClip::instance(*defclient, 0) == nullptr);
    REQUIRE(AudioClip::instance(*defclient, 1) == nullptr);

    REQUIRE(AudioClip::instance(*defclient, ID_T_MAX_VAL) == nullptr);

    REQUIRE(AudioClip::createInst(*defclient));
    REQUIRE_FALSE(AudioClip::createInst(*defclient, 0));
    REQUIRE(AudioClip::createInst(*defclient, 1));
    REQUIRE(AudioClip::createInst(*defclient, ID_T_MAX_VAL));

    REQUIRE(AudioClip::instance(*defclient) == AudioClip::instance(*defclient, ID_T_MAX_VAL));
    REQUIRE_FALSE(AudioClip::instance(*defclient, 0) == nullptr);
    REQUIRE_FALSE(AudioClip::instance(*defclient, 1) == nullptr);
    REQUIRE_FALSE(AudioClip::instance(*defclient, ID_T_MAX_VAL) == nullptr);

    REQUIRE(AudioClip::removeInst(*defclient, 0));
    REQUIRE(AudioClip::removeInst(*defclient, 1));
    REQUIRE_FALSE(AudioClip::removeInst(*defclient, 100));
    REQUIRE_FALSE(AudioClip::removeInst(*defclient, ID_T_MAX_VAL));

    REQUIRE(AudioClip::instance(*defclient) == AudioClip::instance(*defclient, ID_T_MAX_VAL));
    REQUIRE(AudioClip::instance(*defclient, 0) == nullptr);
    REQUIRE(AudioClip::instance(*defclient, 1) == nullptr);
    REQUIRE_FALSE(AudioClip::instance(*defclient, ID_T_MAX_VAL) == nullptr);
    REQUIRE(AudioClip::removeInst(*defclient, 2));
    REQUIRE(AudioClip::instance(*defclient, ID_T_MAX_VAL) == nullptr);

    aClip.userOperationNotifier(ItemOp::TYPE::READ, {0, 0});
    aClip.userOperationNotifier(ItemOp::TYPE::WRITE, {0, 0});
    aClip.userOperationNotifier(ItemOp::TYPE::DELETE, {0, 0});

    aClip.serverOperationNotifier(0, ItemOp::TYPE::READ, {0, 0});
    aClip.serverOperationNotifier(0, ItemOp::TYPE::WRITE, {0, 0});
    aClip.serverOperationNotifier(0, ItemOp::TYPE::EXECUTE, {0, 0});
}