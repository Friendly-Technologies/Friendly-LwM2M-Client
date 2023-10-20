import object_xml_parser

import os
from datetime import datetime
from prettytable import PrettyTable
from prettytable import PLAIN_COLUMNS
from optparse import OptionParser
import json


TYPE_REGISTRY = "WppRegistry"
TYPE_OPERATION = "ResOp"
TYPE_OBJECT = "Object"
TYPE_OBJECT_INFO = "ObjectInfo"
TYPE_RESOURCE = "Resource"
TYPE_INSTANCE = "Instance"
TYPE_I_SUBJECT = "InstSubject"

TYPE_1 = f"{TYPE_OPERATION}::TYPE"
TYPE_2 = "const ResLink"

LOG_FUNC_CUSTOM = "WPP_LOGD_ARG"

FOLDER_TEMPLATES = "./file_templates"
FILE_IMPL_H = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_H.txt"
FILE_IMPL_CPP = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_CPP.txt"
FILE_INFO = f"{FOLDER_TEMPLATES}/FILE_OBJ_INFO.txt"
FILE_CONFIG = f"{FOLDER_TEMPLATES}/FILE_OBJ_CONFIG.txt"
FILE_CMAKE = f"{FOLDER_TEMPLATES}/FILE_OBJ_CMAKE.txt"



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
        return self.object_names["obj_name_folder"]

    def parse_operation(self, xml_operation):
        operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::"
        match xml_operation:
            case "E":
                operation += "EXECUTE"
            case "R":
                operation += "READ"
            case "W":
                operation += "WRITE"
            case "RW":
                operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
            case default:
                operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
        operation += "),"
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
                resource_type += "FLOAT"  # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK"
            case "TIME":
                resource_type += "TIME"
            case "?":
                resource_type += "CORE_LINK"  # TODO: check case
            case default:
                resource_type += "EXECUTE"  # TODO: check case

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
                resource_type += "FLOAT_T"  # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK_T"
            case "TIME":
                resource_type += "TIME_T"
            case "CORE_LINK":
                resource_type += "CORE_LINK_T"  # TODO: check case
            case default:
                resource_type += "NONE"  # TODO: check case

        return resource_type

    def get_map_of_resources(self, resources_list_xml):
        resources_enum = ""
        x = PrettyTable()
        x.set_style(PLAIN_COLUMNS)
        x.header = False
        x.align = "l"

        for resource_xml in resources_list_xml:
            resource_name = resource_xml['Name']
            resource_id = resource_xml['ID']
            # postfix = "M" if resource_xml['Mandatory'] == "MANDATORY" else "O"

            # fill the Resources' enum:
            resource_define = resource_xml['Define']
            resource = f"{resource_name}_{resource_id}"

            if resource_xml['Mandatory'] != "MANDATORY":
                resources_enum += f"\t\t#if {resource_define}\n"
            resources_enum += f"\t\t{resource} = {resource_xml['ID']},\n"
            if resource_xml['Mandatory'] != "MANDATORY":
                resources_enum += f"\t\t#endif\n"

            # fill the unordered_map<ID_T, Resource> table:
            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#if {resource_define}", "", "", "", "", ""])

            x.add_row([f"*TAB*{{{resource},",
                       f"{{{resource},",
                       self.parse_operation(resource_xml['Operations']),
                       f"IS_SINGLE::{resource_xml['MultipleInstances']},",
                       f"IS_MANDATORY::{resource_xml['Mandatory']},",
                       f"{self.parse_resource_data_type(resource_xml['Type'])} }}}},"])

            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#endif", "", "", "", "", ""])

        resources_map = str(x).replace("*TAB*", "\t\t")
        # TODO: fix next line by PrettyTable features!
        resources_map = resources_map.replace(",        ", ",  ")

        return resources_enum, resources_map

    def get_content_resourcesInit_f(self, resources_list_xml):
        content = f"""void __CLASS_NAME__::resourcesInit() {{\n""" \
                  f"""\t/* --------------- Code_cpp block 9 start --------------- */\n"""
        for resource in resources_list_xml:
            if resource["Mandatory"] == "MANDATORY":
                # content += f"""\t\t#if {resource["Name"]}_{resource["Mandatory"]}\n\t"""
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].set( /* TODO */ );\n"
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].setDataVerifier( /* TODO */ );\n\n"
                # content += f"\t\t#endif\n\n"
            if resource["Mandatory"] == "OPTIONAL":
                content += f"""\t#if {resource["Define"]}\n"""
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].set( /* TODO */ );\n"
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].setDataVerifier( /* TODO */ );\n"
                content += f"\t#endif\n\n"
        content += f"""\t/* --------------- Code_cpp block 9 end --------------- */\n}}"""

        return content

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
            f"""void __CLASS_NAME__::serverOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 6 start --------------- */\n""" \
            f"""\t/* --------------- Code_cpp block 6 end --------------- */\n""" \
            f"""\n\tobserverNotify(*this, resId, type);\n\n""" \
            f"""\t/* --------------- Code_cpp block 7 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            base += f"""case {TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"Server {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        return f"""{base}default: break;\n\t}}\n\t""" \
               f"""/* --------------- Code_cpp block 7 end --------------- */\n}}"""

    def get_content_userOperationNotifier(self, param_1, param_2):
        cases = ["READ", "WRITE", "DELETE"]
        prefix = \
            f"""void __CLASS_NAME__::userOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 8 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            prefix += f"""case {TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"User {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        postfix = f"""default: break;\n\t}}\n""" \
                  f"""\t/* --------------- Code_cpp block 8 end --------------- */\n}}"""
        return prefix + postfix

    def generate_content_header(self):
        data_str_h = self.read_file(FILE_PATH_TYPE_OBJ_IMPL_H)
        resources_enum, resources_map = self.get_map_of_resources(self.meta_resources)
        data_str_h = data_str_h.replace("__IF_NOT_DEFINED_DEFINE__", self.object_names["obj_name_folder"].upper())
        data_str_h = data_str_h.replace("__CLASS_NAME__", self.object_names["obj_name_class"])
        data_str_h = data_str_h.replace("__ID_ENUM__", resources_enum)
        data_str_h = data_str_h.replace("__RESOURCES_MAP__", resources_map)
        data_str_h = data_str_h.replace("<<IF_DEF_DIRECTIVE>>", self.object_names["obj_name_folder"].upper())

        return data_str_h

    def generate_content_cpp(self):
        data_str_cpp = self.read_file(FILE_PATH_TYPE_OBJ_IMPL_CPP)
        data_str_cpp = data_str_cpp.replace("__OBJ_FOLDER__", self.object_names["obj_name_folder"])
        data_str_cpp = data_str_cpp.replace("__F_SERVER_OPERATION_NOTIFIER__",
                                            self.get_content_serverOperationNotifier(TYPE_1, TYPE_2))
        data_str_cpp = data_str_cpp.replace("__F_USER_OPERATION_NOTIFIER__",
                                            self.get_content_userOperationNotifier(TYPE_1, TYPE_2))
        data_str_cpp = data_str_cpp.replace("__F_RESOURCE_INIT__",
                                            self.get_content_resourcesInit_f(self.meta_resources))
        data_str_cpp = data_str_cpp.replace("__CLASS_NAME__", self.object_names["obj_name_class"])

        return data_str_cpp

    def generate_content_cmake_list(self):
        name_class = self.object_names["obj_name_class"]
        name_def = self.object_names["obj_name_define"]

        main_line = \
            f"""set(WPP_SOURCES ${{WPP_SOURCES}} ${{CMAKE_CURRENT_SOURCE_DIR}}/{name_class}.cpp)\n\n""" \
            f"""\t# --------------- CMake block 0 start ---------------\n""" \
            f"""\t# --------------- CMake block 0 end ---------------\n\n""" \
            f"""\t# Update WPP_SOURCES variable from parent scope.\n""" \
            f"""\tset(WPP_SOURCES ${{WPP_SOURCES}} PARENT_SCOPE)\n"""

        content = f"""{MAIN_COMMENT}""" \
                  f"""if({name_def})\n\t""" \
                  f"""# Update SOURCES variable from parent scope.\n\t""" \
                  f"""{main_line}\nendif()"""

        return content

    def generate_content_info_header(self):
        if_not_def = f"""WPP_{self.object_names["obj_name_up_underline"]}_INFO_H"""
        is_multiple = "MULTIPLE" if self.meta_object["is_multiple"] else "SINGLE"
        is_mandatory = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"

        content = \
            INFO_COMMENT + \
            f"""#ifndef {if_not_def}\n""" \
            f"""#define {if_not_def}\n\n""" \
            f"""#include "{TYPE_OBJECT_INFO}.h"\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""namespace wpp {{\n\n""" \
            f"""static const {TYPE_OBJECT_INFO} {self.object_names["obj_name_up_underline"]}_OBJ_INFO = {{\n""" \
            f"""\t/* Name */\n\t"{self.meta_object["object_name"]}",\n""" \
            f"""\t/* Object ID */\n\tOBJ_ID::{self.object_names["obj_name_up_underline"]},\n""" \
            f"""\t/* URN */\n\t"{self.meta_object["object_urn"]}",\n""" \
            f"""\t/* Object version */\n\t{{{self.meta_object["object_version"].replace('.', ',')}}},\n""" \
            f"""\t/* Lwm2m version */\n\t{{{self.meta_object["object_lwm2m_version"].replace('.', ',')}}},\n""" \
            f"""\t/* Is single */\n\tIS_SINGLE::{is_multiple},\n""" \
            f"""\t/* Is Mandatory */\n\tIS_MANDATORY::{is_mandatory},\n""" \
            f"""\t/* Object supported operations */\n""" \
            f"""\t/* --------------- Info block 0 start --------------- */\n""" \
            f"""\tInstOp(\tInstOp::CREATE |\n""" \
            f"""\t\t\tInstOp::DELETE),\n""" \
            f"""\t{TYPE_OPERATION}(\t{TYPE_OPERATION}::READ|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::WRITE|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::DISCOVER|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::EXECUTE|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::DELETE),\n""" \
            f"""\t/* --------------- Info block 0 end --------------- */\n""" \
            f"""}};\n\n""" \
            f"""}} /* namespace wpp */\n\n""" \
            f"""#endif /* {self.object_names["obj_name_define"]} */\n""" \
            f"""#endif // {if_not_def}\n"""

        return content

    def generate_content_config(self):
        if_not_def = f"""WPP_{self.object_names["obj_name_up_underline"]}_CONFIG_H"""
        defines = ""
        for resource in self.meta_resources:
            if resource["Mandatory"] == "MANDATORY":
                continue
            defines += f"""#define {resource['Define']} 0\n"""
        content = \
            CONFIG_COMMENT + \
            f"""#ifndef {if_not_def}\n""" \
            f"""#define {if_not_def}\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""/* --------------- Config block 0 start --------------- */\n""" \
            f"""/* --------------- Config block 0 end --------------- */\n\n""" \
            f"""#endif // {if_not_def}\n""" \
            f"""#endif // {self.object_names["obj_name_define"]}\n"""
        return content

    def generate_obj_integration_data(self):
        dictionary = {}

        obj_dict = {"is_mandatory": "True" if self.meta_object["is_mandatory"] else "False",
                    "name": self.meta_object["object_name"],
                    "id": self.meta_object["object_id"],
                    "lwm2m_version": self.meta_object["object_lwm2m_version"],
                    "version": self.meta_object["object_version"]}

        obj_names = {"class": self.object_names["obj_name_class"],
                     "camelcase": self.object_names["obj_name_camelcase"],
                     "define": self.object_names["obj_name_define"],
                     "up_underline": self.object_names["obj_name_up_underline"]}

        obj_files = {"impl_h": f"{self.object_names['obj_name_class']}.h",
                     "impl_cpp": f"{self.object_names['obj_name_class']}.cpp",
                     "config": f"{self.object_names['obj_name_class']}Config.h",
                     "info": f"{self.object_names['obj_name_class']}Info.h",
                     "cmake": f"CMakeLists.txt"}

        dictionary["object_data"] = obj_dict
        dictionary["object_names"] = obj_names
        dictionary["object_files"] = obj_files

        return json.dumps(dictionary, indent=4)

    def create_folder(self):
        try:
            os.mkdir(self.object_names['obj_name_folder'])
        except FileExistsError:
            pass

    def read_file(self, file):
        with open(file, 'r') as f:
            return f.read()

    def create_file(self, filename, filetype, content):
        f = open(f"./{self.object_names['obj_name_folder']}/{filename}.{filetype}", "w+")
        f.write(content)
        f.close()

    def object_code_generate(self):
        generated_header = self.generate_content_header()
        generated_cpp_file = self.generate_content_cpp()
        generated_cmake_list = self.generate_content_cmake_list()
        generated_info_header = self.generate_content_info_header()
        generated_config = self.generate_content_config()
        generated_obj_integration_data = self.generate_obj_integration_data()

        name_class = self.object_names["obj_name_class"]

        self.create_folder()

        self.create_file(f"{name_class}",       "h",    generated_header)
        self.create_file(f"{name_class}Info",   "h",    generated_info_header)
        self.create_file(f"{name_class}Config", "h",    generated_config)
        self.create_file(f"{name_class}",       "cpp",  generated_cpp_file)
        self.create_file(f"CMakeLists",         "txt",  generated_cmake_list)

        self.create_file(f"obj_integration_data", "json", generated_obj_integration_data)


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
