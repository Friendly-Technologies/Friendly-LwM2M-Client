import object_xml_parser

import os
from datetime import datetime
from prettytable import PrettyTable
from prettytable import PLAIN_COLUMNS
from optparse import OptionParser

PLACE_CLASS_NAME_UPPER = "<<<0>>>"
PLACE_CLASS_NAME = "<<<1>>>"
PLACE_RESOURCES_ENUM = "<<<2>>>"
PLACE_RESOURCES_MAP = "<<<3>>>"
PLACE_FOLDER = "<<<4>>>"
PLACE_RESOURCES_INIT = "<<<5>>>"

TYPE_REGISTRY = "WppRegistry"
TYPE_OPERATION = "Operation"
TYPE_OBJECT = "Object"
TYPE_OBJECT_INFO = "ObjectInfo"
TYPE_RESOURCE = "Resource"
TYPE_INSTANCE = "Instance"
TYPE_I_SUBJECT = "InstSubject"

TYPE_1 = "Operation::TYPE"
TYPE_2 = "const ResLink"

LOG_FUNC_CUSTOM = "WPP_LOGD_ARG"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]

MAIN_COMMENT = \
    f"""/*\n""" \
    f""" * {PLACE_CLASS_NAME}\n""" \
    f""" * Generated on: {datetime.now()}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

PREFIX_H = \
    f"""#ifndef {PLACE_CLASS_NAME_UPPER}_H\n""" \
    f"""#define {PLACE_CLASS_NAME_UPPER}_H\n\n""" \
    f"""#include "{PLACE_CLASS_NAME}Config.h"\n""" \
    f"""#include "{PLACE_CLASS_NAME}Info.h"\n""" \
    f"""#include "{TYPE_INSTANCE}.h"\n""" \
    f"""#include "{TYPE_I_SUBJECT}.h"\n\n""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_h block 0 start --------------- */\n""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_h block 0 end --------------- */\n\n""" \
    f"""namespace wpp {{\n\nclass {PLACE_CLASS_NAME} : """ \
    f"""public {TYPE_INSTANCE}, public {TYPE_I_SUBJECT}<{PLACE_CLASS_NAME}> {{\n\n"""

PUBLIC_ENUM_H = \
    f"""\tpublic:\n\t\tenum ID: ID_T {{\n{PLACE_RESOURCES_ENUM}\t\t}};\n\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 1 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 1 end --------------- */\n\n""" \

PUBLIC_CONSTRUCTOR_H = \
    f"""\tpublic:\n\t\t{PLACE_CLASS_NAME}(WppClient &client, const OBJ_LINK_T &id);\n\t""" \
    f"""\t~{PLACE_CLASS_NAME}();\n\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 2 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 2 end --------------- */\n\n"""

I_INSTANCE_IMPLEMENTATIONS = \
    f"""\t/* --------------- {TYPE_INSTANCE} implementation part --------------- */\n"""

I_INSTANCE_IMPLEMENTATIONS_H = \
    f"""\tprotected:\n\t{I_INSTANCE_IMPLEMENTATIONS}\t\t/* \n\t""" \
    f"""\t * Returns Resource object if it is exist\n\t\t */\n\t\t{TYPE_RESOURCE} * getResource(ID_T id) override;\n\t\t""" \
    f"""/*\n\t\t * Returns list with available resources\n\t\t */\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getResourcesList() override;\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getResourcesList(const {TYPE_OPERATION}& filter) override;\n\t\t/*\n\t """ \
    f"""\t * Returns list with available instantiated resources\n\t\t */\n\t """ \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList() override;\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) override;\n\t""" \
    f"""\t/*\n\t\t * Reset all resources values and internal state to default.\n\t\t */\n\t""" \
    f"""\tvoid clear() override;\n\t\t/*\n\t""" \
    f"""\t * Handles information about resource operation that made server\n\t\t */\n\t""" \
    f"""\tvoid serverOperationNotifier({TYPE_1} type, {TYPE_2} &resId) override;\n\t\t/*\n\t""" \
    f"""\t * Handles information about resource operation that made user\n\t\t */\n\t""" \
    f"""\tvoid userOperationNotifier({TYPE_1} type, {TYPE_2} &resId) override;"""

CLASS_PRIVATE_METHODS_H = \
    f"""\n\n\tprivate:\n\t\t/* --------------- Class private methods --------------- */\n\t""" \
    f"""\t/*\n\t\t * Initialize resources with default values\n\t""" \
    f"""\t * Resource always must have at least one instance.\n\t\t */\t\n\t""" \
    f"""\tvoid resourcesInit();\n\t\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 3 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 3 end --------------- */\n\n""" \
    f"""\tprivate:\n\t\tstd::unordered_map<ID_T, {TYPE_RESOURCE}> _resources = {{\n\t\t""" \
    f"""\t// KEY   VALUE\n{PLACE_RESOURCES_MAP}\n\t\t}};\n\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 4 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_h block 4 end --------------- */\n""" \
    f"""}};\n\n""" \
    f"""}} /* namespace wpp */\n\n#endif /* {PLACE_CLASS_NAME_UPPER}_H */\n"""

PREFIX_CPP = \
    f"""#include "mandatory/{PLACE_FOLDER}/{PLACE_CLASS_NAME}.h"\n\n""" \
    f"""#include <unordered_map>\n""" \
    f"""#include <iostream>\n\n""" \
    f"""#include "{TYPE_RESOURCE}.h"\n""" \
    f"""#include "{TYPE_OPERATION}.h"\n""" \
    f"""#include "types.h"\n""" \
    f"""#include "WppLogs.h"\n\n""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_cpp block 0 start --------------- */\n""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_cpp block 0 end --------------- */\n\n""" \
    f"""#define TAG "{PLACE_CLASS_NAME}"\n\n""" \
    f"""namespace wpp {{\n\n"""

PUBLIC_CONSTRUCTOR_CPP = \
    f"""\t{PLACE_CLASS_NAME}::{PLACE_CLASS_NAME}(WppClient &client, const OBJ_LINK_T &id): {TYPE_INSTANCE}(client, id) {{\n\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 1 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 1 end --------------- */\n""" \
    f"""\n\t\tresourcesInit();\n\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 2 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 2 end --------------- */\n""" \
    f"""\t}}\n\n"""

PUBLIC_DESTRUCTOR_CPP = \
    f"""\t{PLACE_CLASS_NAME}::~{PLACE_CLASS_NAME}() {{\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 3 start --------------- */\n""" \
    f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 3 end --------------- */\n""" \
    f"""\t}}\n\n"""

CLASS_PRIVATE_METHODS_CPP = \
    f"""\t/* --------------- Class private methods --------------- */\n{PLACE_RESOURCES_INIT}""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_cpp block 10 start --------------- */\n""" \
    f"""/* --------------- {PLACE_CLASS_NAME}_cpp block 10 end --------------- */\n\n"""

FUNC_GET_RESOURCE_T = \
    f"""\t{TYPE_RESOURCE} * {PLACE_CLASS_NAME}::getResource(ID_T id) {{\n\t""" \
    f"""\t// Check if resource ID is valid\n\t""" \
    f"""\tif (_resources.find(id) == _resources.end()) return NULL;\n\t""" \
    f"""\treturn &_resources[id];\n\t}}\n\n"""

FUNC_GET_RESOURCE_LIST = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList() {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tlist.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_RESOURCE_LIST_P = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_INSTANTIATED_LIST = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList() {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (!pair.second.isEmpty()) list.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_INSTANTIATED_LIST_P = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) """ \
    f"""list.push_back(&pair.second);\n\t\t}}\n\t\treturn list;\n\t}}\n\n"""

FUNC_CLEAR = \
    f"""\tvoid {PLACE_CLASS_NAME}::clear() {{\n\t""" \
    f"""\t/* --------------- Code_cpp block 4 start --------------- */\n\t""" \
    f"""\t/* --------------- Code_cpp block 4 end --------------- */\n\n\t""" \
    f"""\tfor (auto &pair : _resources) pair.second.clear();\n\t""" \
    f"""\tresourcesInit();\n\n\t""" \
    f"""\t/* --------------- Code_cpp block 5 start --------------- */\n\t""" \
    f"""\t/* --------------- Code_cpp block 5 end --------------- */\n\t""" \
    f"""}}\n\n"""


class ObjectGenerator:
    """Add some comments here"""

    def __init__(self, xml_file, xml_url):

        xp = object_xml_parser.ObjectXmlParser(xml_file=xml_file, xml_url=xml_url)
        obj_dict, res_list = xp.parse_xml()
        obj_names = xp.create_metadata()

        self.meta_object = obj_dict
        self.meta_resources = res_list
        self.object_names = obj_names

    def get_folder_path(self):
        return self.object_names["obj_name_path_to_folder"]

    def parse_operation(self, xml_operation):
        operation = f"{TYPE_OPERATION}::"
        match xml_operation:
            case "E":
                operation += "EXECUTE"
            case "R":
                operation += "READ"
            case "W":
                operation += "WRITE"
            case "RW":
                operation = f"{TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
            case default:
                operation = f"{TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
        operation += ","
        return operation

    def parse_resource_data_type(self, xml_type):
        resource_type = "TYPE_ID::"
        match xml_type:
            case "INTEGER":
                resource_type += "INT"
            case "UNSIGNED_INTEGER":
                resource_type += "UINT"
            case "BOOLEAN":
                resource_type += "BOOL"
            case "STRING":
                resource_type += "STRING"
            case "EXECUTE":
                resource_type += "EXECUTE"
            case "OPAQUE":
                resource_type += "OPAQUE"
            case "?":
                resource_type += "FLOAT"            # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK"
            case "TIME":
                resource_type += "TIME"
            case "?":
                resource_type += "CORE_LINK"        # TODO: check case
            case default:
                resource_type += "EXECUTE"          # TODO: check case

        return resource_type

    def parse_type(self, xml_type):
        resource_type = ""
        match xml_type:
            case "INTEGER":
                resource_type += "INT_T"
            case "UNSIGNED_INTEGER":
                resource_type += "UINT_T"
            case "BOOLEAN":
                resource_type += "BOOL_T"
            case "STRING":
                resource_type += "STRING_T"
            case "EXECUTE":
                resource_type += "EXECUTE_T"
            case "OPAQUE":
                resource_type += "OPAQUE_T"
            case "FLOAT":
                resource_type += "FLOAT_T"          # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK_T"
            case "TIME":
                resource_type += "TIME_T"
            case "CORE_LINK":
                resource_type += "CORE_LINK_T"      # TODO: check case
            case default:
                resource_type += "NONE"             # TODO: check case

        return resource_type

    def get_map_of_resources(self, resources_list_xml):
        resources_enum = ""
        x = PrettyTable()
        x.set_style(PLAIN_COLUMNS)
        x.header = False
        x.align = "l"

        for resource_xml in resources_list_xml:
            resource_name = resource_xml['Name']
            postfix = "M" if resource_xml['Mandatory'] == "MANDATORY" else "O"

            # fill the Resources' enum:
            resource_define = f"{resource_name}_{postfix}"
            resource_enum = f"\t\t\t{resource_define} = {resource_xml['ID']},\n"
            resources_enum += resource_enum

            # fill the unordered_map<ID_T, Resource> table:

            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#if DEF_{resource_define}", "", "", "", "", ""])

            x.add_row([f"*TAB*{{{resource_define},",
                       f"{{{resource_define},",
                       self.parse_operation(resource_xml['Operations']),
                       f"IS_SINGLE::{resource_xml['MultipleInstances']},",
                       f"IS_MANDATORY::{resource_xml['Mandatory']},",
                       f"{self.parse_resource_data_type(resource_xml['Type'])} }}}},"])

            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#endif", "", "", "", "", ""])

        resources_map = str(x).replace("*TAB*", "\t\t\t")

        return resources_enum, resources_map

    def get_content_resourcesInit_f(self, resources_list_xml):
        content = f"""\tvoid {PLACE_CLASS_NAME}::resourcesInit() {{\n""" \
                  f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 9 start --------------- */\n\n"""
        for resource in resources_list_xml:
            if resource["Mandatory"] == "MANDATORY":
                content += f"""\t\t#if {resource["Name"]}_{resource["Mandatory"]}\n\t"""
                content += f"\t\t_resources[{resource['Name']}].set( /* TODO */ );\n\t"
                content += f"\t\t_resources[{resource['Name']}].setDataVerifier( /* TODO */ );\n"
                content += f"\t\t#endif\n\n"
        return content + f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 9 end --------------- */\n\t}}\n\n"""

    def create_log_string(self, text: str, arguments: list, is_std, pattern: str = "%d"):
        if not isinstance(text, str) or not isinstance(arguments, list):
            raise TypeError

        if len(pattern) != text.count(pattern):
            return ""

        if is_std:
            for argument in arguments:
                text = text.replace(pattern, f"\" << {argument} << \"", 1)
            return f"""std::cout << "{text}" << std::endl;"""

        else:
            text = '"' + text + '"'
            for argument in arguments:
                text += ', ' + argument
            return f"""{LOG_FUNC_CUSTOM}(TAG, {text});"""

    def get_content_serverOperationNotifier(self, param_1, param_2):
        cases = ["READ", "WRITE", "EXECUTE", "DISCOVER", "DELETE"]
        base = \
            f"""\tvoid {PLACE_CLASS_NAME}::serverOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 6 start --------------- */\n""" \
            f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 6 end --------------- */\n""" \
            f"""\n\t\tobserverNotify(*this, resId, type);\n\n""" \
            f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 7 start --------------- */\n""" \
            f"""\t\tswitch (type) {{\n\t"""
        for case in cases:
            base += f"""\t\tcase {TYPE_OPERATION}::{case}:\n\t\t\t\t{self.create_log_string(
                f"Server {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\t\t\tbreak;\n\t"""
        return f"""{base}\t\tdefault: break;\n\t\t}}\n\t""" \
               f"""\t/* --------------- {PLACE_CLASS_NAME}_cpp block 7 end --------------- */\n}}\n\n"""

    def get_content_userOperationNotifier(self, param_1, param_2):
        cases = ["READ", "WRITE", "DELETE"]
        prefix = \
            f"""\tvoid {PLACE_CLASS_NAME}::userOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 8 start --------------- */\n""" \
            f"""\t\tswitch (type) {{\n\t"""
        for case in cases:
            prefix += f"""\t\tcase {TYPE_OPERATION}::{case}:\n\t\t\t\t{self.create_log_string(
                f"User {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\t\t\tbreak;\n\t"""
        postfix = f"""\t\tdefault: break;\n\t\t}}\n""" \
                  f"""\t\t/* --------------- {PLACE_CLASS_NAME}_cpp block 8 end --------------- */\n\t}}\n\n"""
        return prefix + postfix

    def generate_content_header(self):
        resources_enum, resources_map = self.get_map_of_resources(self.meta_resources)
        code_header = (MAIN_COMMENT +
                       PREFIX_H +
                       PUBLIC_ENUM_H +
                       PUBLIC_CONSTRUCTOR_H +
                       I_INSTANCE_IMPLEMENTATIONS_H +
                       CLASS_PRIVATE_METHODS_H +
                       "")
        code_header = code_header.replace(PLACE_CLASS_NAME_UPPER, self.object_names["obj_name_class"].upper())
        code_header = code_header.replace(PLACE_CLASS_NAME, self.object_names["obj_name_class"])
        code_header = code_header.replace(PLACE_RESOURCES_ENUM, resources_enum)
        code_header = code_header.replace(PLACE_RESOURCES_MAP, resources_map)

        return code_header

    def generate_content_cpp(self):
        code_cpp = (MAIN_COMMENT +
                    PREFIX_CPP +
                    PUBLIC_CONSTRUCTOR_CPP +
                    PUBLIC_DESTRUCTOR_CPP +
                    I_INSTANCE_IMPLEMENTATIONS +
                    FUNC_GET_RESOURCE_T +
                    FUNC_GET_RESOURCE_LIST +
                    FUNC_GET_RESOURCE_LIST_P +
                    FUNC_GET_INSTANTIATED_LIST +
                    FUNC_GET_INSTANTIATED_LIST_P +
                    FUNC_CLEAR +
                    self.get_content_serverOperationNotifier(TYPE_1, TYPE_2) +
                    self.get_content_userOperationNotifier(TYPE_1, TYPE_2) +
                    CLASS_PRIVATE_METHODS_CPP +
                    "} /* namespace wpp */\n")
        code_cpp = code_cpp.replace(PLACE_RESOURCES_INIT, self.get_content_resourcesInit_f(self.meta_resources))
        code_cpp = code_cpp.replace(PLACE_CLASS_NAME, self.object_names["obj_name_class"])
        code_cpp = code_cpp.replace(PLACE_FOLDER, self.object_names["obj_name_folder"])

        return code_cpp

    def generate_content_cmake_list(self):
        main_line = f"""set(WPP_SOURCES ${{WPP_SOURCES}} ${{CMAKE_CURRENT_SOURCE_DIR}}/{self.object_names["obj_name_class"]}.cpp PARENT_SCOPE)\n""" \
                    f"""\t# --------------- CMake block 0 start ---------------\n""" \
                    f"""\t# --------------- CMake block 0 end ---------------\n"""

        return f"""if({self.object_names["obj_name_define"]})\n\t""" \
               f"""# Update SOURCES variable from parent scope.\n\t""" \
               f"""{main_line}\nendif()"""

    def generate_content_info_header(self):
        ifnotdef = f"""{ self.object_names["obj_name_define"].upper()}INFO_H"""
        is_multiple = "MULTIPLE" if self.meta_object["is_multiple"] else "SINGLE"
        is_mandatory = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"

        content = \
            f"""#ifndef {ifnotdef}\n""" \
            f"""#define {ifnotdef}\n\n""" \
            f"""#include "{TYPE_OBJECT_INFO}.h"\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""namespace wpp {{\n\n""" \
            f"""static const {TYPE_OBJECT_INFO} {self.object_names["obj_name_object_info"]} = {{\n""" \
            f"""\t/* Name */\n\t"{self.meta_object["object_name"]}",\n\n""" \
            f"""\t/* Object ID */\n\tOBJ_ID::SERVER,\n\n""" \
            f"""\t/* URN */\n\t"{self.meta_object["object_urn"]}",\n\n""" \
            f"""\t/* Object version */\n\t{{{self.meta_object["object_lwm2m_version"].replace('.', ',')}}},\n\n""" \
            f"""\t/* Lwm2m version */\n\t{{{self.meta_object["object_version"].replace('.', ',')}}},\n\n""" \
            f"""\t/* Is single */\n\tIS_SINGLE::{is_multiple},\n\n""" \
            f"""\t/* Is Mandatory */\n\tIS_MANDATORY::{is_mandatory},\n\n""" \
            f"""\t/* Object supported operations */\n""" \
            f"""\t/* --------------- Info block 0 start --------------- */\n""" \
            f"""\t{TYPE_OPERATION}(\t{TYPE_OPERATION}::READ|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::WRITE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::DISCOVER|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::EXECUTE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::CREATE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::DELETE),\n""" \
            f"""\t/* --------------- Info block 0 end --------------- */\n""" \
            f"""}};\n\n""" \
            f"""}} /* namespace wpp */\n\n""" \
            f"""#endif /* {self.object_names["obj_name_define"]} */\n""" \
            f"""#endif // {ifnotdef}\n"""

        return content

    def generate_content_config(self):
        ifnotdef = f"""{self.object_names["obj_name_class"].upper()}CONFIG_H"""
        defines = ""
        for i in self.meta_resources:
            if i["Mandatory"] == "MANDATORY":
                continue
            defines += f"""#define {i["Name"]}_O 0\n"""
        content = \
            f"""#ifndef {ifnotdef}\n""" \
            f"""#define {ifnotdef}\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""/* ---------- Server optional resources start ---------- */\n\n""" \
            f"""{defines}\n""" \
            f"""/* ---------- Server optional resources end ---------- */\n\n""" \
            f"""/* --------------- Config block 0 start --------------- */\n""" \
            f"""/* --------------- Config block 0 end --------------- */\n\n""" \
            f"""#endif // {ifnotdef}\n""" \
            f"""#endif // {self.object_names["obj_name_define"]}\n"""
        return content

    def create_folder(self):
        try:
            os.mkdir(self.object_names['obj_name_path_to_folder'])
        except FileExistsError:
            pass

    def create_file(self, filename, filetype, content):
        f = open(f"{self.object_names['obj_name_path_to_folder']}/{filename}.{filetype}", "w+")
        f.write(content)
        f.close()

    def object_code_generate(self):
        generated_header = self.generate_content_header()
        generated_cpp_file = self.generate_content_cpp()
        generated_cmake_list = self.generate_content_cmake_list()
        generated_info_header = self.generate_content_info_header()
        generated_config = self.generate_content_config()

        name_class = self.object_names["obj_name_class"]

        self.create_folder()

        self.create_file(f"{name_class}",        "h",    generated_header)
        self.create_file(f"{name_class}",        "cpp",  generated_cpp_file)
        self.create_file(f"CMakeLists",          "txt",  generated_cmake_list)
        self.create_file(f"{name_class}Info",    "h",    generated_info_header)
        self.create_file(f"{name_class}Config",  "h",    generated_config)


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--file", dest="xml_file", help="the path to the xml file of the Object")
    parser.add_option("-u", "--url", dest="xml_url", help="the url to the xml file of the Object")
    options, args = parser.parse_args()

    if options.xml_file or options.xml_url:
        og = ObjectGenerator(options.xml_file, options.xml_url)
        og.object_code_generate()
    else:
        parser.error("the path or url to the XML-file of the Object is not provided")
