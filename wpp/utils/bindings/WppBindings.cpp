#include "WppBindings.h"

namespace wpp {

bool wppBindingValidate(const STRING_T& binding) {
	if(binding == WPP_BINDING_UDP) return true;
	if(binding == WPP_BINDING_TCP) return true;
	if(binding == WPP_BINDING_SMS) return true;
	if(binding == WPP_BINDING_NON_IP) return true;
	return false;
}

} // wpp