import shutil
import os
from optparse import OptionParser

FOLDER_OBJECTS = "../../wpp/registry/objects"
FOLDER_CONFIG = "../../wpp/config"
FOLDER_REGISTRY = "../../wpp/registry"

FILE_ADD_DATA = "object_metadata.json"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]

TYPE_REGISTRY = "WppRegistry"
TYPE_OBJECT = "Object"


class ObjectIntegrator:
    """Add some comments here"""

    def __init__(self, folder_name):
        self.folder_name = folder_name

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

    def insert_additional_data(self):
        try:
            with open(f"{self.folder_name}/{FILE_ADD_DATA}", 'r') as f:
                data_str = f.read()
            f.close()
        except FileNotFoundError:
            print(F"No such file \"{FILE_ADD_DATA}\". Stopped")
            return False

        data_dict = eval(data_str)
        dict_obj_meta = data_dict["object_data"]
        dict_obj_names = data_dict["object_names"]

        obj_is_mandatory = dict_obj_meta["is_mandatory"]
        obj_id = dict_obj_meta["id"]

        obj_name_class = dict_obj_names["class"]
        obj_name_define = dict_obj_names["define"]
        obj_name_camelcase = dict_obj_names["camelcase"]
        obj_name_underline = dict_obj_names["up_underline"]

        stop_string_obj_id = STOP_STRING_OBJ_ID[0] if obj_is_mandatory else STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = STOP_STRING_CNFG_CMK[0] if obj_is_mandatory else STOP_STRING_CNFG_CMK[1]
        stop_string_reg_cpp = STOP_STRING_REG_PRT[0] if obj_is_mandatory else STOP_STRING_REG_PRT[1]
        stop_string_reg_incl = STOP_STRING_REG_INCL[0] if obj_is_mandatory else STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = STOP_STRING_REG_PRT[0] if obj_is_mandatory else STOP_STRING_REG_PRT[1]

        content_obj_id = \
            f"#ifdef {obj_name_define}\n" \
            f"\t{obj_name_underline} = {obj_id},\n" \
            f"#endif /* {obj_name_define} */\n"

        content_cfg_cmake = \
            f"""option({obj_name_define} """ \
            f""""Include {"mandatory" if obj_is_mandatory else "optional"} """ \
            f"""{obj_name_class} object in the build" {"ON" if obj_is_mandatory else "OFF"})\n""" \
            f"""if ({obj_name_define})\n\tset(WPP_DEFINITIONS ${{WPP_DEFINITIONS}} {obj_name_define}=1)""" \
            f"""\nendif()\n"""

        content_reg_h_include = \
            f"""#if {obj_name_define}\n""" \
            f"""#include "{self.folder_name}/{obj_name_class}.h"\n""" \
            f"""#endif\n"""

        content_reg_h_prototype = \
            f"""\t#if {obj_name_define}\n\t""" \
            f"""{TYPE_OBJECT}<{obj_name_class}> & {obj_name_camelcase}();\n\t""" \
            f"""#endif\n"""

        content_reg_cpp = \
            f"""# if {obj_name_define}\n""" \
            f"""{TYPE_OBJECT} <{obj_name_class}> & {TYPE_REGISTRY}::{obj_name_camelcase}() {{\n\t""" \
            f"""if (!{TYPE_OBJECT} <{obj_name_class}>::isCreated()) {TYPE_OBJECT} <{obj_name_class}>::""" \
            f"""create(_context, {obj_name_underline}_OBJ_INFO);\n\t""" \
            f"""return *{TYPE_OBJECT} <{obj_name_class}>::object();\n}}\n# endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, "../../wpp/registry/ObjectID.h")
        self.update_file(stop_string_cfg_cmk, content_cfg_cmake, "../../wpp/config/config.cmake")
        self.update_file(stop_string_reg_cpp, content_reg_cpp, "../../wpp/registry/WppRegistry.cpp")
        self.update_file(stop_string_reg_incl, content_reg_h_include, "../../wpp/registry/WppRegistry.h")
        self.update_file(stop_string_reg_prot, content_reg_h_prototype, "../../wpp/registry/WppRegistry.h")

    def remove_unused_files(self, path, file):
        try:
            os.remove(f"{path}/{file}")
        except FileNotFoundError:
            print(f"No such file \"{path}/{file}\"")

    def copy_main_files(self):
        try:
            shutil.copytree(self.folder_name, f"{FOLDER_OBJECTS}/{self.folder_name}")
            return True
        except FileExistsError:
            print(f"The folder \"{FOLDER_OBJECTS}/{self.folder_name}\" is already exists")
            return False

    def update_files(self):
        if not self.copy_main_files():
            return False
        # self.remove_unused_files(f"{FOLDER_OBJECTS}/{self.folder_name}", FILE_ADD_DATA)
        self.insert_additional_data()


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--folder", dest="obj_folder", help="the folder name of the Object")

    options, args = parser.parse_args()

    if options.obj_folder:
        oi = ObjectIntegrator(options.obj_folder)
        oi.update_files()
    else:
        parser.error("the folder of the Object is not provided")
