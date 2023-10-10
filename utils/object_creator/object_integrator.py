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

    def __init__(self, xml_file, xml_url):
        xp = object_xml_parser.ObjectXmlParser(xml_file=xml_file, xml_url=xml_url)
        obj_dict, res_list = xp.parse_xml()
        self.obj_meta = obj_dict
        self.obj_names = xp.create_metadata()

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
        is_obj_mandatory = self.obj_meta["is_mandatory"]

        obj_name_class = self.obj_names['obj_name_class']
        obj_name_define = self.obj_names['obj_name_define']
        obj_name_camelcase = self.obj_names["obj_name_camelcase"]

        stop_string_obj_id = STOP_STRING_OBJ_ID[0] if is_obj_mandatory else STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = STOP_STRING_CNFG_CMK[0] if is_obj_mandatory else STOP_STRING_CNFG_CMK[1]
        stop_string_reg_cpp = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]
        stop_string_reg_incl = STOP_STRING_REG_INCL[0] if is_obj_mandatory else STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]

        content_obj_id = \
            f"#ifdef {obj_name_define}\n" \
            f"\t{obj_name_class.upper()} = {self.obj_meta['object_id']},\n" \
            f"#endif /* {obj_name_define} */\n"

        content_cnfg_cmk = \
            f"""option({obj_name_define} """ \
            f""""Include {"mandatory" if is_obj_mandatory else "optional"} """ \
            f"""{obj_name_class} object in the build" {"ON" if is_obj_mandatory else "OFF"})\n""" \
            f"""if ({obj_name_define})\n\tset(WPP_DEFINITIONS ${{WPP_DEFINITIONS}} {obj_name_define}=1)""" \
            f"""\nendif()\n"""

        content_reg_h_incl = \
            f"""#if {obj_name_define}\n""" \
            f"""#include "{"mandatory" if is_obj_mandatory else "optional"}/{self.obj_names["obj_name_folder"]}/{obj_name_class}.h"\n""" \
            f"""#endif\n"""

        content_reg_h_prt = \
            f"""\t#if {obj_name_define}\n\t""" \
            f"""{TYPE_OBJECT} <{obj_name_class}> & {obj_name_camelcase}();\n\t""" \
            f"""#endif\n"""

        content_reg_cpp = \
            f"""# if {obj_name_define}\n""" \
            f"""{TYPE_OBJECT} <{obj_name_class}> & {TYPE_REGISTRY}::{obj_name_camelcase}() {{\n\t""" \
            f"""if (!{TYPE_OBJECT} <{obj_name_class}>::isCreated()) {TYPE_OBJECT} <{obj_name_class}>::""" \
            f"""create(_context, {self.obj_names["obj_name_object_info"]});\n\t""" \
            f"""return *{TYPE_OBJECT} <{obj_name_class}>::object();\n""" \
            f"""}}\n""" \
            f"""# endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, "../../wpp/registry/ObjectID.h")
        self.update_file(stop_string_cfg_cmk, content_cnfg_cmk, "../../wpp/config/config.cmake")

        self.update_file(stop_string_reg_incl, content_reg_h_incl, "../../wpp/registry/WppRegistry.h")
        self.update_file(stop_string_reg_prot, content_reg_h_prt, "../../wpp/registry/WppRegistry.h")

        self.update_file(stop_string_reg_cpp, content_reg_cpp, "../../wpp/registry/WppRegistry.cpp")


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--file", dest="xml_file", help="the path to the xml file of the Object")
    parser.add_option("-u", "--url", dest="xml_url", help="the url to the xml file of the Object")
    options, args = parser.parse_args()

    if options.xml_file or options.xml_url:
        oi = ObjectIntegrator(options.xml_file, options.xml_url)
        oi.update_files()
    else:
        parser.error("the path or url to the XML-file of the Object is not provided")
