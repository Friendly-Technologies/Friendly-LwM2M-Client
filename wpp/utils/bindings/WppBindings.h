#ifndef WPP_BINDINGS_H
#define WPP_BINDINGS_H

#include "WppTypes.h"

#define WPP_BINDING_UDP	STRING_T("U")
#define WPP_BINDING_TCP	STRING_T("T")
#define WPP_BINDING_SMS	STRING_T("S")
#define WPP_BINDING_NON_IP STRING_T("N")

namespace wpp {

bool wppBindingValidate(const STRING_T& binding);

} // wpp

#endif //WPP_BINDINGS_H