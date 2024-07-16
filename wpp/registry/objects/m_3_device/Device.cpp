/*
 * Device
 * Generated on: 2023-11-02 16:38:01
 * Created by: Sinai RnD
 */

#include "m_3_device/Device.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
#if RES_3_9
#define BAT_LVL_MIN	0
#define BAT_LVL_MAX	100
#endif
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Device"

namespace wpp {

Device::Device(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	#if RES_3_13
	_currentTimeTaskId = WPP_ERR_TASK_ID;
	#endif
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Device::~Device() {
	/* --------------- Code_cpp block 3 start --------------- */
	#if RES_3_13
	WppTaskQueue::requestToRemoveTask(_currentTimeTaskId);
	#endif
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Device::object(WppClient &ctx) {
	return ctx.registry().device();
}

Device * Device::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().device().instance(instId);
	if (!inst) return NULL;
	return static_cast<Device*>(inst);
}

Device * Device::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().device().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Device*>(inst);
}

bool Device::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().device().remove(instId);
}

void Device::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 5 start --------------- */
	/* --------------- Code_cpp block 5 end --------------- */
}

void Device::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
	if (type == ItemOp::WRITE || type == ItemOp::DELETE) notifyResChanged(resLink.resId, resLink.resInstId);

	/* --------------- Code_cpp block 6 start --------------- */
	/* --------------- Code_cpp block 6 end --------------- */
}

void Device::resourcesCreate() {
	std::vector<Resource> resources = {
		#if RES_3_0                                                                                                                                                             
		{MANUFACTURER_0,                 ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_1                                                                                                                                                             
		{MODEL_NUMBER_1,                 ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_2                                                                                                                                                             
		{SERIAL_NUMBER_2,                ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_3                                                                                                                                                             
		{FIRMWARE_VERSION_3,             ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		{REBOOT_4,                       ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE },  
		#if RES_3_5                                                                                                                                                             
		{FACTORY_RESET_5,                ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                  
		#if RES_3_6                                                                                                                                                             
		{AVAILABLE_POWER_SOURCES_6,      ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_7                                                                                                                                                             
		{POWER_SOURCE_VOLTAGE_7,         ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_8                                                                                                                                                             
		{POWER_SOURCE_CURRENT_8,         ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_9                                                                                                                                                             
		{BATTERY_LEVEL_9,                ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_10                                                                                                                                                            
		{MEMORY_FREE_10,                 ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		{ERROR_CODE_11,                  ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT },      
		#if RES_3_12                                                                                                                                                            
		{RESET_ERROR_CODE_12,            ItemOp(ItemOp::EXECUTE),           IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                  
		#if RES_3_13                                                                                                                                                            
		{CURRENT_TIME_13,                ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },     
		#endif                                                                                                                                                                  
		#if RES_3_14                                                                                                                                                            
		{UTC_OFFSET_14,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_15                                                                                                                                                            
		{TIMEZONE_15,                    ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		{SUPPORTED_BINDING_AND_MODES_16, ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },   
		#if RES_3_17                                                                                                                                                            
		{DEVICE_TYPE_17,                 ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_18                                                                                                                                                            
		{HARDWARE_VERSION_18,            ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_19                                                                                                                                                            
		{SOFTWARE_VERSION_19,            ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                  
		#if RES_3_20                                                                                                                                                            
		{BATTERY_STATUS_20,              ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_21                                                                                                                                                            
		{MEMORY_TOTAL_21,                ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                  
		#if RES_3_22                                                                                                                                                            
		{EXTDEVINFO_22,                  ItemOp(ItemOp::READ),              IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK }, 
		#endif  
		#if RES_3_99
		{CPU_UTILIZATION_99, 			 ItemOp(ItemOp::READ),              IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },
		#endif                                                                                                                                                                
	};
	setupResources(std::move(resources));
}

void Device::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	#if RES_3_0                                                                                                                                                                                        
	resource(MANUFACTURER_0)->set<STRING_T>("");
	#endif          

	#if RES_3_1  
	resource(MODEL_NUMBER_1)->set<STRING_T>("");                                                                                                                                                                                        
	#endif                                                                                                                                                                                                              
	
	#if RES_3_2                                                                                                                                                                                         
	resource(SERIAL_NUMBER_2)->set<STRING_T>(""); 
	#endif                                                                                                                                                                                                              
	
	#if RES_3_3                                                                                                                                                                                        
	resource(FIRMWARE_VERSION_3)->set<STRING_T>(""); 
	#endif     
 
	resource(REBOOT_4)->set<EXECUTE_T>([](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
                                                                                                                                                                                                                            
	#if RES_3_5
	resource(FACTORY_RESET_5)->set<EXECUTE_T>([](Instance& inst, ID_T resId, const OPAQUE_T& data) { return true; });
	#endif

	#if RES_3_6
	resource(AVAILABLE_POWER_SOURCES_6)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return DC <= value && value < PWR_SRC_MAX; });
	#endif

	#if RES_3_9
	resource(BATTERY_LEVEL_9)->set<INT_T>(BAT_LVL_MIN);
	resource(BATTERY_LEVEL_9)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return BAT_LVL_MIN <= value && value <= BAT_LVL_MAX; });
	#endif

	#if RES_3_10
	resource(MEMORY_FREE_10)->set<INT_T>(0);                                                                                                                                                                                         
	#endif

	resource(ERROR_CODE_11)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return NO_ERROR <= value && value < ERR_CODE_MAX; });
	
	#if RES_3_12
	resource(RESET_ERROR_CODE_12)->set<EXECUTE_T>([this](Instance& inst, ID_T resId, const OPAQUE_T& buff) { 
		resource(ERROR_CODE_11)->clear();
		resource(ERROR_CODE_11)->set<INT_T>(NO_ERROR);
		notifyResChanged(ERROR_CODE_11);
		return true;
	});
	#endif

	#if RES_3_13
	resource(CURRENT_TIME_13)->set<TIME_T>(WppPlatform::getTime());
	_currentTimeTaskId = WppTaskQueue::addTask(1, [this](WppClient &client, void *ctx) -> bool {
		TIME_T currentTime = WppPlatform::getTime();
		resource(CURRENT_TIME_13)->set<TIME_T>(currentTime);
		notifyResChanged(CURRENT_TIME_13);
		return false;
	});
	#endif                                                                                                                                                                                                              
	
	#if RES_3_14
	resource(UTC_OFFSET_14)->set<STRING_T>("");                                                                                                                                                                                             
	#endif                                                                                                                                                                                                              
	
	#if RES_3_15
	resource(TIMEZONE_15)->set<STRING_T>("");                                                                                                                                                                                     
	#endif

	resource(SUPPORTED_BINDING_AND_MODES_16)->set<STRING_T>("");
	resource(SUPPORTED_BINDING_AND_MODES_16)->setDataVerifier((VERIFY_STRING_T)([](const STRING_T& value) { return wppBindingValidate(value); }));

	#if RES_3_17
	resource(DEVICE_TYPE_17)->set<STRING_T>("");                                                                                                                                                                                           
	#endif

	#if RES_3_18
	resource(HARDWARE_VERSION_18)->set<STRING_T>("");                                                                                                                                                                              
	#endif                                                                                                                                                                                                              
	
	#if RES_3_19
	resource(SOFTWARE_VERSION_19)->set<STRING_T>("");                                                                                                                                                                                    
	#endif                 

	#if RES_3_20
	resource(BATTERY_STATUS_20)->set<INT_T>(BAT_STATUS_MAX);
	resource(BATTERY_STATUS_20)->setDataVerifier((VERIFY_INT_T)[](const INT_T& value) { return NORMAL <= value && value < BAT_STATUS_MAX; });
	#endif

	#if RES_3_21                                                                                                                                                                                          
	resource(MEMORY_TOTAL_21)->set<INT_T>(NO_ERROR);
	#endif                                                 

	#if RES_3_99
	resource(CPU_UTILIZATION_99)->set<STRING_T>("");
	#endif                                                                                                                                                                 
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
