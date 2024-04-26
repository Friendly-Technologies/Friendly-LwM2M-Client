#include "ResourceConatiner.h"

namespace wpp {

ResourceContainer::ResourceContainer(std::vector<Resource> resources): _resources(resources) {}
ResourceContainer::ResourceContainer(std::vector<Resource> &&resources): _resources(std::move(resources)) {}
ResourceContainer::~ResourceContainer() {}

bool ResourceContainer::isSingle(ID_T resId) const {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return false;
    }
    return res->isSingle();
}

bool ResourceContainer::isMultiple(ID_T resId) const {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return false;
    }
    return res->isMultiple();
}

bool ResourceContainer::isExist(ID_T resId) {
    return resource(resId) != NULL;
}

bool ResourceContainer::isExist(ID_T resId, ID_T resInstId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return false;
    }
    return res->isExist(resInstId);

}

size_t ResourceContainer::instCount(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return 0;
    }
    return res->instCount();
}

std::vector<ID_T> ResourceContainer::instIds(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return std::vector<ID_T>();
    }
    return res->instIds();
}

ID_T ResourceContainer::newInstId(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return ID_T_MAX_VAL;
    }
    return res->newInstId();
}

bool ResourceContainer::remove(ID_T resId, ID_T resInstId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return false;
    }
    if (!res->remove(resInstId)) return false;
    operationNotifier(ItemOp::DELETE, resId, resInstId);
    return true;
}

bool ResourceContainer::clear(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGE(TAG_WPP_RES_CON, "Resource with ID %d not found", resId);
        return false;
    }
    if (!res->clear()) return false;
    operationNotifier(ItemOp::DELETE, resId, ID_T_MAX_VAL);
    return true;
}

Resource * resource(ID_T resId) {
    auto finder = [&resId](const Resource &res) -> bool { return res.getId() == resId; };
	auto res = std::find_if(_resources.begin(), _resources.end(), finder);
	return res != _resources.end()? &(*res) : NULL;
}

std::vector<Resource> & resources() { 
    return _resources; 
}

}