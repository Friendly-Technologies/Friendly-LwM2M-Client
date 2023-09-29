from tabulate import tabulate
from optparse import OptionParser
import object_xml_parser

TYPE_REGISTRY = "WppRegistry"
TYPE_OBJECT = "Object"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]


class ObjectIntegrator:
    """Add some comments here"""

    def __init__(self, object_dict):
        # meta data:
        self.meta_object = object_dict
        # names:
        plain_name = object_dict["object_name"]                                         # LwM2M Server
        plain_name_no_space = plain_name.replace(' ', '')                               # LwM2MServer
        plain_name_underline = plain_name.replace(' ', '_')                             # LwM2M_Server
        # create camelcase:
        plain_name_list = plain_name.split(" ")
        plain_name_list[0] = plain_name_list[0][0].lower() + plain_name_list[0][1:]
        self.name_camelcase = ''.join(plain_name_list)                                 # lwM2MServer
        # names of class, folder, define:
        self.name_obj_class = f"Wpp{plain_name_no_space}"                               # WppLwM2MServer
        mandatory_or_optional = "mandatory" if object_dict["is_mandatory"] else "optional"
        self.name_obj_folder = plain_name_underline.lower()                             # lwm2m_server
        self.name_obj_define = f"{mandatory_or_optional}_{plain_name_no_space}_OBJ".upper()     # MANDATORY_LWM2MSERVER_OBJ

    def update_file(self, stop_string, content, path_to_file):
        is_stop_string_present = False
        new_content = ''
        old_content = ''
        with open(path_to_file, 'r') as f:
            for i in f:
                old_content += i
        f.close()

        if old_content.find(content) != -1:
            # print(f"File {path_to_file} is already updated")
            return

        for line in old_content.split("\n"):
            if " ".join(line.split()) == stop_string:
                is_stop_string_present = True
                new_content += content
            new_content += line + "\n"

        if not is_stop_string_present:
            # print(f"The {path_to_file} was not updated!")
            return

        with open(path_to_file, 'w') as f:
            f.write(new_content[:-1])
        f.close()

    def update_files(self):
        is_obj_mandatory = self.meta_object["is_mandatory"]
        stop_string_obj_id = STOP_STRING_OBJ_ID[0] if is_obj_mandatory else STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = STOP_STRING_CNFG_CMK[0] if is_obj_mandatory else STOP_STRING_CNFG_CMK[1]

        stop_string_reg_cpp = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]
        stop_string_reg_incl = STOP_STRING_REG_INCL[0] if is_obj_mandatory else STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]

        content_obj_id = \
            f"#ifdef {self.name_obj_define}\n" \
            f"\t{self.name_obj_class.upper()} = {self.meta_object['object_id']},\n" \
            f"#endif /* {self.name_obj_define} */\n\n"

        content_cnfg_cmk = \
            f"""\noption({self.name_obj_define} """ \
            f""""Include {"mandatory" if is_obj_mandatory else "optional"} """ \
            f"""{self.name_obj_class} object in the build" {"ON" if is_obj_mandatory else "OFF"})\n""" \
            f"""if ({self.name_obj_define})\n\tset(WPP_DEFINITIONS ${{WPP_DEFINITIONS}} {self.name_obj_define}=1)""" \
            f"""\nendif()\n\n"""

        content_reg_h_incl = \
            f"""#if {self.name_obj_define}\n""" \
            f"""#include "mandatory/{self.name_obj_folder}/{self.name_obj_class}.h"\n""" \
            f"""#endif\n\n"""

        content_reg_h_prt = \
            f"""\t#if {self.name_obj_define}\n\t""" \
            f"""{TYPE_OBJECT} <{self.name_obj_class}> & {self.name_camelcase}();\n\t""" \
            f"""#endif\n\n"""

        content_reg_cpp = \
            f"""# if {self.name_obj_define}\n""" \
            f"""{TYPE_OBJECT} <{self.name_obj_class}> & {TYPE_REGISTRY}::{self.name_camelcase}() {{\n\t""" \
            f"""if (!{TYPE_OBJECT} <{self.name_obj_class}>::isCreated()) {TYPE_OBJECT} <{self.name_obj_class}>::create(_client, {self.name_obj_class.replace(' ', '').upper()}_OBJ_INFO);\n\t""" \
            f"""return *{TYPE_OBJECT} <{self.name_obj_class}>::object();\n""" \
            f"""}}\n""" \
            f"""# endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, "../wpp/registry/ObjectID.h")
        self.update_file(stop_string_cfg_cmk, content_cnfg_cmk, "../wpp/config/config.cmake")

        self.update_file(stop_string_reg_incl, content_reg_h_incl, "../wpp/registry/WppRegistry.h")
        self.update_file(stop_string_reg_prot, content_reg_h_prt, "../wpp/registry/WppRegistry.h")

        self.update_file(stop_string_reg_cpp, content_reg_cpp, "../wpp/registry/WppRegistry.cpp")


xp = object_xml_parser.ObjectXmlParser()
obj_dict, res_list = xp.parse_xml("./1-1_1.xml")
oi = ObjectIntegrator(obj_dict)
oi.update_files()
