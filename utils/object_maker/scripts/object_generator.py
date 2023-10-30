import object_xml_parser
import constants as const
import functions as func

import os
from datetime import datetime
from prettytable import PrettyTable
from prettytable import PLAIN_COLUMNS
from optparse import OptionParser
import json

DATETIME = datetime.now().strftime('%Y-%m-%d %H:%M:%S')


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
        type = const.TYPE_OPERATION
        operation = f"{type}({type}::"
        match xml_operation:
            case "E":
                operation += "EXECUTE"
            case "R":
                operation += "READ"
            case "W":
                operation += "WRITE"
            case "RW":
                operation = f"{type}({type}::READ|{type}::WRITE"
            case default:
                operation = f"{type}({type}::READ|{type}::WRITE"
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
            return f"""WPP_LOGD_ARG(TAG, {text});"""

    def get_content_serverOperationNotifier(self):
        cases = ["READ", "WRITE", "EXECUTE", "DISCOVER", "DELETE"]
        base = \
            f"""void __CLASS_NAME__::serverOperationNotifier(ResOp::TYPE type, const ResLink &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 6 start --------------- */\n""" \
            f"""\t/* --------------- Code_cpp block 6 end --------------- */\n""" \
            f"""\n\tobserverNotify(*this, resId, type);\n\n""" \
            f"""\t/* --------------- Code_cpp block 7 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            base += f"""case {const.TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"Server {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        return f"""{base}default: break;\n\t}}\n\t""" \
               f"""/* --------------- Code_cpp block 7 end --------------- */\n}}"""

    def get_content_userOperationNotifier(self):
        cases = ["READ", "WRITE", "DELETE"]
        prefix = \
            f"""void __CLASS_NAME__::userOperationNotifier(ResOp::TYPE type, const ResLink &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 8 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            prefix += f"""case {const.TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"User {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        postfix = f"""default: break;\n\t}}\n""" \
                  f"""\t/* --------------- Code_cpp block 8 end --------------- */\n}}"""
        return prefix + postfix

    def generate_content_header(self):
        data_str_h = func.get_file_content(const.FILE_TMPLT_IMPL_H)[1]
        resources_enum, resources_map = self.get_map_of_resources(self.meta_resources)
        data_str_h = data_str_h.replace("__DATETIME__", DATETIME)
        data_str_h = data_str_h.replace("__IF_NOT_DEFINED_DEFINE__", self.object_names["obj_name_folder"].upper())
        data_str_h = data_str_h.replace("__CLASS_NAME__", self.object_names["obj_name_class"])
        data_str_h = data_str_h.replace("__ID_ENUM__", resources_enum)
        data_str_h = data_str_h.replace("__RESOURCES_MAP__", resources_map)
        data_str_h = data_str_h.replace("<<IF_DEF_DIRECTIVE>>", self.object_names["obj_name_folder"].upper())

        return data_str_h

    def generate_content_cpp(self):
        data_str_cpp = func.get_file_content(const.FILE_TMPLT_IMPL_CPP)[1]
        data_str_cpp = data_str_cpp.replace("__DATETIME__", DATETIME)
        data_str_cpp = data_str_cpp.replace("__OBJ_FOLDER__", self.object_names["obj_name_folder"])
        data_str_cpp = data_str_cpp.replace("__F_SERVER_OPERATION_NOTIFIER__",
                                            self.get_content_serverOperationNotifier())
        data_str_cpp = data_str_cpp.replace("__F_USER_OPERATION_NOTIFIER__",
                                            self.get_content_userOperationNotifier())
        data_str_cpp = data_str_cpp.replace("__F_RESOURCE_INIT__",
                                            self.get_content_resourcesInit_f(self.meta_resources))
        data_str_cpp = data_str_cpp.replace("__CLASS_NAME__", self.object_names["obj_name_class"])

        return data_str_cpp

    def generate_content_cmake_list(self):
        content = func.get_file_content(const.FILE_TMPLT_CMAKE)[1]
        content = content.replace("__DATETIME__", DATETIME)
        content = content.replace("__OBJ_DEFINE__", self.object_names["obj_name_define"])
        content = content.replace("__CLASS_NAME__", self.object_names["obj_name_class"])

        return content

    def generate_content_info_header(self):
        is_multiple = "MULTIPLE" if self.meta_object["is_multiple"] else "SINGLE"
        is_mandatory = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"

        content = func.get_file_content(const.FILE_TMPLT_INFO)[1]
        content = content.replace("__DATETIME__", DATETIME)
        content = content.replace("<<IF_DEF_DIRECTIVE>>", self.object_names["obj_name_up_underline"])
        content = content.replace("__UPNAME__", self.object_names["obj_name_up_underline"])
        content = content.replace("__OBJ_DEFINE__", self.object_names["obj_name_define"])
        content = content.replace("__NAME__", self.meta_object["object_name"])
        content = content.replace("__OBJ_ID__", self.object_names["obj_name_up_underline"])
        content = content.replace("__URN__", self.meta_object["object_urn"])
        content = content.replace("__VERSION__",
                                  f"{{{self.meta_object['object_version'].replace('.', ',')}}}")
        content = content.replace("__LWM2M_VERSION__",
                                  f"{{{self.meta_object['object_lwm2m_version'].replace('.', ',')}}}")
        content = content.replace("__MULTIPLE__", is_multiple)
        content = content.replace("__MANDATORY__", is_mandatory)

        return content

    def generate_content_config(self):
        defines = ""
        for resource in self.meta_resources:
            if resource["Mandatory"] == "OPTIONAL":
                defines += f"""#define {resource['Define']} 0\n"""

        content = func.get_file_content(const.FILE_TMPLT_CONFIG)[1]
        content = content.replace("__DATETIME__", DATETIME)
        content = content.replace("<<IF_DEF_DIRECTIVE>>", self.object_names["obj_name_up_underline"])
        content = content.replace("__OBJ_DEFINE__", self.object_names["obj_name_define"])
        content = content.replace("__RES_DEFINES__", defines)

        return content

    def generate_obj_integration_data(self):
        dictionary = {}

        obj_dict = {const.KEY_DICT_OBJ_META_MANDAT: "True" if self.meta_object["is_mandatory"] else "False",
                    const.KEY_DICT_OBJ_META_NAME: self.meta_object["object_name"],
                    const.KEY_DICT_OBJ_META_ID: self.meta_object["object_id"],
                    const.KEY_DICT_OBJ_META_VER_LWM2M: self.meta_object["object_lwm2m_version"],
                    const.KEY_DICT_OBJ_META_VER: self.meta_object["object_version"]}

        obj_names = {const.KEY_DICT_OBJ_NAMES_CLASS: self.object_names["obj_name_class"],
                     const.KEY_DICT_OBJ_NAMES_CAMELC: self.object_names["obj_name_camelcase"],
                     const.KEY_DICT_OBJ_NAMES_DEFINE: self.object_names["obj_name_define"],
                     const.KEY_DICT_OBJ_NAMES_UNDERL: self.object_names["obj_name_up_underline"]}

        obj_files = {const.KEY_FILE_IMPL_H: f"{self.object_names['obj_name_class']}.h",
                     const.KEY_FILE_IMPL_CPP: f"{self.object_names['obj_name_class']}.cpp",
                     const.KEY_FILE_IMPL_CONFIG: f"{self.object_names['obj_name_class']}Config.h",
                     const.KEY_FILE_IMPL_INFO: f"{self.object_names['obj_name_class']}Info.h",
                     const.KEY_FILE_IMPL_CMAKE: f"CMakeLists.txt"}

        dictionary[const.KEY_DICT_OBJ_META] = obj_dict
        dictionary[const.KEY_DICT_OBJ_NAMES] = obj_names
        dictionary[const.KEY_DICT_OBJ_FILES] = obj_files

        return json.dumps(dictionary, indent=4)

    def create_folder(self):
        try:
            os.mkdir(self.object_names['obj_name_folder'])
        except FileExistsError:
            pass

    def create_file(self, filename, filetype, content):
        file = filename if filetype == "" else f"{filename}.{filetype}"
        func.create_file(f"./{self.object_names['obj_name_folder']}/{file}", content)

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

        self.create_file(const.FILE_OBJ_METADATA, "", generated_obj_integration_data)


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
