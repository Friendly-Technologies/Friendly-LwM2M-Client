/*
 * FirmwareUpdate
 * Generated on: 2024-03-19 13:06:08
 * Created by: SinaiR&D
 */

#include "o_5_firmware_update/FirmwareUpdate.h"

#include "Resource.h"
#include "ResOp.h"
#include "WppTypes.h"
#include "WppLogs.h"

/* --------------- Code_cpp block 0 start --------------- */
#include <cstring>
#include "WppPlatform.h"

#define SCHEME_DIVIDER 	"://"
#define COAP_SCHEME 	"coap"
#define COAPS_SCHEME 	"coaps"
#define HTTP_SCHEME 	"http"
#define HTTPS_SCHEME 	"https"
#define COAP_TCP_SCHEME "coap+tcp"
#define COAP_TLS_SCHEME "coaps+tcp"
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "FirmwareUpdate"

namespace wpp {

FirmwareUpdate::FirmwareUpdate(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	_pkgUpdater = NULL;
	_uriDownloader = NULL;
	_autoDownloader = NULL;
	_uriDownloaderTaskId = WPP_ERR_TASK_ID;
	_updaterTaskId = WPP_ERR_TASK_ID;
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

FirmwareUpdate::~FirmwareUpdate() {
	/* --------------- Code_cpp block 3 start --------------- */
	WppTaskQueue::requestToRemoveTask(_uriDownloaderTaskId);
	WppTaskQueue::requestToRemoveTask(_updaterTaskId);
	/* --------------- Code_cpp block 3 end --------------- */
}

void FirmwareUpdate::setDefaultState() {
	/* --------------- Code_cpp block 4 start --------------- */
	_pkgUpdater = NULL;
	_uriDownloader = NULL;
	_autoDownloader = NULL;
	
	WppTaskQueue::requestToRemoveTask(_uriDownloaderTaskId);
	WppTaskQueue::requestToRemoveTask(_updaterTaskId);
	_uriDownloaderTaskId = WPP_ERR_TASK_ID;
	_updaterTaskId = WPP_ERR_TASK_ID;
	/* --------------- Code_cpp block 4 end --------------- */

	_resources.clear();
	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void FirmwareUpdate::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 6 start --------------- */
	WPP_LOGD(TAG, "Server operation -> type: %d, resId: %d, resInstId: %d", type, resId.resId, resId.resInstId);
	switch (type) {
	case ResOp::WRITE_UPD:
	case ResOp::WRITE_REPLACE_RES: {
		if (resId.resId == PACKAGE_0) autoDownloaderHandler();
		else if (resId.resId == PACKAGE_URI_1) uriDownloaderHandler();
		break;
	}
	case ResOp::EXECUTE: {
		if (resId.resId == UPDATE_2) pkgUpdaterHandler();
		break;
	}
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */

	operationNotify(*this, resId, type);

	/* --------------- Code_cpp block 7 start --------------- */
	/* --------------- Code_cpp block 7 end --------------- */
}

void FirmwareUpdate::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {
	/* --------------- Code_cpp block 8 start --------------- */
	WPP_LOGD(TAG, "User operation -> type: %d, resId: %d, resInstId: %d", type, resId.resId, resId.resInstId);
	/* --------------- Code_cpp block 8 end --------------- */
}

/* --------------- Code_cpp block 9 start --------------- */
/* --------------- Code_cpp block 9 end --------------- */

void FirmwareUpdate::resourcesCreate() {
	std::vector<Resource> resources = {
		{PACKAGE_0,                          ResOp(ResOp::WRITE),             IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::OPAQUE },  
		{PACKAGE_URI_1,                      ResOp(ResOp::READ|ResOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },  
		{UPDATE_2,                           ResOp(ResOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE }, 
		{STATE_3,                            ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		{UPDATE_RESULT_5,                    ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
		#if RES_5_6                                                                                                                                                                
		{PKGNAME_6,                          ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_7                                                                                                                                                                
		{PKGVERSION_7,                       ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                     
		#if RES_5_8                                                                                                                                                                
		{FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8, ResOp(ResOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                     
		{FIRMWARE_UPDATE_DELIVERY_METHOD_9,  ResOp(ResOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::INT },     
	};
	_resources = std::move(resources);
}

void FirmwareUpdate::resourcesInit() {
	/* --------------- Code_cpp block 10 start --------------- */
	resource(PACKAGE_0)->set(OPAQUE_T());
	resource(PACKAGE_0)->setDataVerifier((VERIFY_OPAQUE_T)[this](const OPAQUE_T& value) { 
		if (value.empty() || isDeliveryTypeSupported(PUSH)) return true;
		return false;
	});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	resource(PACKAGE_URI_1)->setDataVerifier((VERIFY_STRING_T)[this](const STRING_T& value) { return isUriValid(value); });
	resource(UPDATE_2)->set((EXECUTE_T)[](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	resource(STATE_3)->set(INT_T(S_IDLE));
	resource(STATE_3)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { 
		if (S_IDLE > value || value >= STATE_MAX) return false;
		return true;
	});
	resource(UPDATE_RESULT_5)->set(INT_T(R_INITIAL));
	resource(UPDATE_RESULT_5)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { 
		if (R_INITIAL > value || value >= UPD_RES_MAX) return false;
		return true; 
	});
	#if RES_5_6
	resource(PKGNAME_6)->set(STRING_T(""));
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set(STRING_T(""));
	#endif
	#if RES_5_8
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set(INT_T(COAP));
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return COAP <= value && value < FW_UPD_PROTOCOL_MAX; });
	#endif
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return PULL <= value && value < FW_UPD_DELIVERY_MAX; });
	/* --------------- Code_cpp block 10 end --------------- */
}

/* --------------- Code_cpp block 11 start --------------- */
bool FirmwareUpdate::setFwUpdater(FwUpdater &updater) {
	resetStateMachine();

	_pkgUpdater = &updater;

	// Set last update result
	resource(UPDATE_RESULT_5)->set(INT_T(updater.lastUpdateResult()));
	notifyServerResChanged({UPDATE_RESULT_5,});

	#if RES_5_6
	resource(PKGNAME_6)->set(updater.pkgName());
	notifyServerResChanged({PKGNAME_6,});
	#endif
	#if RES_5_7
	resource(PKGVERSION_7)->set(updater.pkgVersion());
	notifyServerResChanged({PKGVERSION_7,});
	#endif

	return true;
}

#if RES_5_8
std::vector<FwUpdProtocol> FirmwareUpdate::supportedProtocols() {
	std::vector<FwUpdProtocol> supportedProtocols;
	for (auto id : resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->getInstIds()) {
		INT_T protocol;
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get(protocol, id);
		supportedProtocols.push_back(FwUpdProtocol(protocol));
	}

	return supportedProtocols;
}

bool FirmwareUpdate::setFwExternalDownloader(FwExternalDl &downloader) {
	resetStateMachine();

	_uriDownloader = &downloader;

	std::vector<FwUpdProtocol> dlSupportedProtocols = _uriDownloader->supportedProtocols();
	if (dlSupportedProtocols.empty()) {
		_uriDownloader = NULL;
		return false;
	}

	// Setup delivery type
	if (_autoDownloader) resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(BOTH));
	else resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PULL));
	notifyServerResChanged({FIRMWARE_UPDATE_DELIVERY_METHOD_9,});

	// Setup supported protocols
	ID_T instId = 0;
	resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->clear();
	for (auto prot : dlSupportedProtocols) {
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->set(INT_T(prot), instId);
		instId++;
	}
	notifyServerResChanged({FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8,});

	return true;
}
#endif

bool FirmwareUpdate::setFwAutoDownloader(FwAutoDl &downloader) {
	// TODO: Update the implementation of this method after creating an
	// interface for downloading firmware via uri using the wpp library.
	// Currently, FwAutoDl only supports loading through the PACKAGE_0 resource.
	resetStateMachine();

	_autoDownloader = &downloader;

	// Setup delivery type
	if (_uriDownloader) resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(BOTH));
	else resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->set(INT_T(PUSH));
	notifyServerResChanged({FIRMWARE_UPDATE_DELIVERY_METHOD_9,});

	return true;
}

void FirmwareUpdate::pkgUpdaterHandler() {
	if (!_pkgUpdater) return;

	INT_T state;
	resource(STATE_3)->get(state);
	if (state != S_DOWNLOADED) return;

	changeState(S_UPDATING);
	_pkgUpdater->startUpdating();

	_updaterTaskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
		if (!_pkgUpdater->isUpdated()) return false;

		changeState(S_IDLE);
		changeUpdRes(_pkgUpdater->lastUpdateResult());

		#if RES_5_6
		resource(PKGNAME_6)->set(_pkgUpdater->pkgName());
		notifyServerResChanged({PKGNAME_6,});
		#endif
		#if RES_5_7
		resource(PKGVERSION_7)->set(_pkgUpdater->pkgVersion());
		notifyServerResChanged({PKGVERSION_7,});
		#endif
		#if RES_3_3
        client.registry().device().instance()->set(Device::FIRMWARE_VERSION_3, updater.pkgVersion());
        #endif

		return true;
	});
}

void FirmwareUpdate::uriDownloaderHandler() {
	if (!_uriDownloader) return;

	STRING_T pkgUri;
	resource(PACKAGE_URI_1)->get(pkgUri);
	if (pkgUri.empty()) {
		WPP_LOGD(TAG, "Server reset state machine through PACKAGE_URI_1");
		resetStateMachine();
		_uriDownloader->reset();
		return;
	}

	changeState(S_DOWNLOADING);
	// TODO: Should be used correct Lwm2mSecurity object
	Lwm2mSecurity security(getContext(), OBJ_LINK_T{0, 0});
	_uriDownloader->startDownloading(pkgUri, security);

	_uriDownloaderTaskId = WppTaskQueue::addTask(WPP_TASK_DEF_DELAY_S, [this](WppClient &client, void *ctx) -> bool {
		if (!_uriDownloader->isDownloaded()) return false;

		if (_uriDownloader->downloadResult() != R_INITIAL) changeState(S_IDLE);
		else changeState(S_DOWNLOADED);

		changeUpdRes(_autoDownloader->downloadResult());

		return true;
	});
}

void FirmwareUpdate::autoDownloaderHandler() {
	// TODO: Update the implementation of this method after creating an
	// interface for downloading firmware via uri using the wpp library.
	// Currently, FwAutoDl only supports loading through the PACKAGE_0 resource.
	if (!_autoDownloader) return;

	OPAQUE_T *pkg;
	resource(PACKAGE_0)->ptr(&pkg);	
	if (pkg->empty()) {
		WPP_LOGD(TAG, "Server reset state machine through PACKAGE_0");
		resetStateMachine();
		_autoDownloader->reset();
		return;
	} 
	
	changeState(S_DOWNLOADING);
	_autoDownloader->downloadIsStarted();
	_autoDownloader->saveDownloadedBlock(*pkg);
	_autoDownloader->downloadIsCompleted();
	if (_autoDownloader->downloadResult() != R_INITIAL) changeState(S_IDLE);
	else changeState(S_DOWNLOADED);

	changeUpdRes(_autoDownloader->downloadResult());
}

void FirmwareUpdate::changeUpdRes(FwUpdRes res) {
	resource(UPDATE_RESULT_5)->set(INT_T(res));
	notifyServerResChanged({UPDATE_RESULT_5,});
}

void FirmwareUpdate::changeState(FwUpdState state) {
	resource(STATE_3)->set(INT_T(state));
	notifyServerResChanged({STATE_3,});
}

void FirmwareUpdate::resetStateMachine() {
	resource(PACKAGE_0)->set(OPAQUE_T());
	notifyServerResChanged({PACKAGE_0,});
	resource(PACKAGE_URI_1)->set(STRING_T(""));
	notifyServerResChanged({PACKAGE_URI_1,});
	changeState(S_IDLE);
	changeUpdRes(R_INITIAL);
}

bool FirmwareUpdate::isUriValid(STRING_T uri) {
	if (uri.empty()) return true;
	if (!isDeliveryTypeSupported(PULL)) return false;

	STRING_T scheme = extractSchemeFromUri(uri);
	if (!isSchemeValid(scheme)) {
		changeUpdRes(R_INVALID_URI);
		return false;
	}

	#if RES_5_8
	if (!isSchemeSupported(scheme)) {
		changeUpdRes(R_UNSUPPORTED_PROTOCOL);
		return false;
	}
	#endif

	return true;
}

STRING_T FirmwareUpdate::extractSchemeFromUri(STRING_T uri) {
	size_t startsAt = uri.find(SCHEME_DIVIDER);
	if (startsAt == std::string::npos) return STRING_T("");
	return uri.substr(0, startsAt);
}

bool FirmwareUpdate::isSchemeValid(STRING_T scheme) {
	if (scheme.empty()) return false;

	const char *validSchemes[] = {COAP_SCHEME, COAPS_SCHEME, HTTP_SCHEME, HTTPS_SCHEME, COAP_TCP_SCHEME, COAP_TLS_SCHEME};
	for (auto s : validSchemes) {
		if (!std::strcmp(scheme.c_str(), s)) return true;
	}
	return false;
}

#if RES_5_8
bool FirmwareUpdate::isSchemeSupported(STRING_T scheme) {
	FwUpdProtocol requiredProt = schemeToProtId(scheme);
	for (auto id : resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->getInstIds()) {
		INT_T suppProt;
		resource(FIRMWARE_UPDATE_PROTOCOL_SUPPORT_8)->get(suppProt, id);
		if (requiredProt == suppProt) return true;
	}
	return false;
}

FwUpdProtocol FirmwareUpdate::schemeToProtId(STRING_T scheme) {
	if (!std::strcmp(scheme.c_str(), COAP_SCHEME)) return COAP;
	else if (!std::strcmp(scheme.c_str(), COAPS_SCHEME)) return COAPS;
	else if (!std::strcmp(scheme.c_str(), HTTP_SCHEME)) return HTTP;
	else if (!std::strcmp(scheme.c_str(), HTTPS_SCHEME)) return HTTPS;
	else if (!std::strcmp(scheme.c_str(), COAP_TCP_SCHEME)) return COAP_TCP;
	else if (!std::strcmp(scheme.c_str(), COAP_TLS_SCHEME)) return COAP_TLS;
	else return FW_UPD_PROTOCOL_MAX;
}
#endif

bool FirmwareUpdate::isDeliveryTypeSupported(FwUpdDelivery type) {
	INT_T deliveryType = FW_UPD_DELIVERY_MAX;
	resource(FIRMWARE_UPDATE_DELIVERY_METHOD_9)->get(deliveryType);
	if (deliveryType == type || deliveryType == BOTH) return true;
	return false;
}
/* --------------- Code_cpp block 11 end --------------- */

} /* namespace wpp */
