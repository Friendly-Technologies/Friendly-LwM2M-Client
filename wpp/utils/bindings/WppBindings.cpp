#include "WppBindings.h"
#include <vector>

namespace wpp {

bool wppBindingValidate(const STRING_T& binding) {
	 std::vector<STRING_T> supportedBuindings = {WPP_BINDING_UDP, WPP_BINDING_TCP, WPP_BINDING_SMS, WPP_BINDING_NON_IP};
	for (auto &item : binding) {
		if (std::find(supportedBuindings.begin(), supportedBuindings.end(), STRING_T(1, item)) == supportedBuindings.end()) return false;
	}
	return true;
}

} // wpp