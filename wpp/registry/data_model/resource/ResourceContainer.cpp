#include "ResourceContainer.h"

namespace wpp {

bool ResourceContainer::isSingle(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return false;
    }
    return res->isSingle();
}

bool ResourceContainer::isMultiple(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
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
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return false;
    }
    return res->isExist(resInstId);
}

size_t ResourceContainer::instCount(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return 0;
    }
    return res->instCount();
}

std::vector<ID_T> ResourceContainer::instIds(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return std::vector<ID_T>();
    }
    return res->instIds();
}

ID_T ResourceContainer::newInstId(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return ID_T_MAX_VAL;
    }
    return res->newInstId();
}

bool ResourceContainer::removeRes(ID_T resId, ID_T resInstId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return false;
    }
    if (!res->remove(resInstId)) return false;
    resourceOperationNotifier(ItemOp::DELETE, resId, resInstId);
    return true;
}

bool ResourceContainer::clearRes(ID_T resId) {
    auto res = resource(resId);
    if (res == NULL) {
        WPP_LOGW(TAG_WPP_RES_CON, "Resource[%d] not found", resId);
        return false;
    }
    if (!res->clear()) return false;
    resourceOperationNotifier(ItemOp::DELETE, resId, ID_T_MAX_VAL);
    return true;
}

void ResourceContainer::setupResources(const std::vector<Resource> &resources) {
    _resources = resources;
}

void ResourceContainer::setupResources(std::vector<Resource> &&resources) {
    _resources = std::move(resources);
}

Resource * ResourceContainer::resource(ID_T resId) {
    auto finder = [&resId](const Resource &res) -> bool { return res.getId() == resId; };
	auto res = std::find_if(_resources.begin(), _resources.end(), finder);
	return res != _resources.end()? &(*res) : NULL;
}

std::vector<Resource> & ResourceContainer::resources() { 
    return _resources; 
}

} // namespace wpp