#include "catch_amalgamated.hpp"
#include "WppBindings.h"

using namespace wpp;

TEST_CASE("Wpp bindings defines") {
    REQUIRE(WPP_BINDING_UDP == STRING_T("U"));
    REQUIRE(WPP_BINDING_TCP == STRING_T("T"));
    REQUIRE(WPP_BINDING_SMS == STRING_T("S"));
    REQUIRE(WPP_BINDING_NON_IP == STRING_T("N"));
}

TEST_CASE("Wpp binding validate") {
    REQUIRE(wpp::wppBindingValidate(WPP_BINDING_UDP));
    REQUIRE(wpp::wppBindingValidate(WPP_BINDING_TCP));
    REQUIRE(wpp::wppBindingValidate(WPP_BINDING_SMS));
    REQUIRE(wpp::wppBindingValidate(WPP_BINDING_NON_IP));

    REQUIRE(wpp::wppBindingValidate(STRING_T("")));
    REQUIRE(wpp::wppBindingValidate(STRING_T("UU")));
    REQUIRE(wpp::wppBindingValidate(STRING_T("TT")));
    REQUIRE(wpp::wppBindingValidate(STRING_T("SS")));
    REQUIRE(wpp::wppBindingValidate(STRING_T("NN")));
    REQUIRE_FALSE(wpp::wppBindingValidate(STRING_T("u")));
    REQUIRE_FALSE(wpp::wppBindingValidate(STRING_T("t")));
    REQUIRE_FALSE(wpp::wppBindingValidate(STRING_T("s")));
    REQUIRE_FALSE(wpp::wppBindingValidate(STRING_T("n")));
}