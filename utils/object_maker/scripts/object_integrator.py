import constants as const
import functions as func

import shutil
from optparse import OptionParser


class ObjectIntegrator:
    """Add some comments here"""

    def __init__(self, folder_name):
        self.folder_name = folder_name

    def update_file(self, stop_string, content, path_to_file):
        is_stop_string_present = False
        new_content = ''
        errcode, old_content = func.get_file_content(path_to_file)

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

        func.write_to_file(path_to_file, new_content[:-1])

    def insert_additional_data(self):
        errcode, data_str = func.get_file_content(f"{self.folder_name}/{const.FILE_OBJ_METADATA}")
        if not errcode:
            return False

        type_obj = const.TYPE_OBJECT

        data_dict = eval(data_str)
        dict_obj_meta = data_dict[const.KEY_DICT_OBJ_META]
        dict_obj_names = data_dict[const.KEY_DICT_OBJ_NAMES]

        obj_is_mandatory = dict_obj_meta[const.KEY_DICT_OBJ_META_MANDAT]
        obj_id = dict_obj_meta[const.KEY_DICT_OBJ_META_ID]

        obj_name_class = dict_obj_names[const.KEY_DICT_OBJ_NAMES_CLASS]
        obj_name_define = dict_obj_names[const.KEY_DICT_OBJ_NAMES_DEFINE]
        obj_name_camelcase = dict_obj_names[const.KEY_DICT_OBJ_NAMES_CAMELC]
        obj_name_underline = dict_obj_names[const.KEY_DICT_OBJ_NAMES_UNDERL]

        stop_string_obj_id = const.STOP_STRING_OBJ_ID[0] if obj_is_mandatory else const.STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = const.STOP_STRING_CNFG_CMK[0] if obj_is_mandatory else const.STOP_STRING_CNFG_CMK[1]
        stop_string_reg_cpp = const.STOP_STRING_REG_PRT[0] if obj_is_mandatory else const.STOP_STRING_REG_PRT[1]
        stop_string_reg_incl = const.STOP_STRING_REG_INCL[0] if obj_is_mandatory else const.STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = const.STOP_STRING_REG_PRT[0] if obj_is_mandatory else const.STOP_STRING_REG_PRT[1]

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
            f"""{type_obj}<{obj_name_class}> & {obj_name_camelcase}();\n\t""" \
            f"""#endif\n"""

        content_reg_cpp = \
            f"""# if {obj_name_define}\n""" \
            f"""{type_obj} <{obj_name_class}> & WppRegistry::{obj_name_camelcase}() {{\n\t""" \
            f"""if (!{type_obj} <{obj_name_class}>::isCreated()) {type_obj} <{obj_name_class}>::""" \
            f"""create(_context, {obj_name_underline}_OBJ_INFO);\n\t""" \
            f"""return *{type_obj} <{obj_name_class}>::object();\n}}\n# endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, const.FILE_OBJECT_ID)
        self.update_file(stop_string_cfg_cmk, content_cfg_cmake, const.FILE_CONFIG_CMAKE)
        self.update_file(stop_string_reg_cpp, content_reg_cpp, const.FILE_REGISTRY_CPP)
        self.update_file(stop_string_reg_incl, content_reg_h_include, const.FILE_REGISTRY_H)
        self.update_file(stop_string_reg_prot, content_reg_h_prototype, const.FILE_REGISTRY_H)

    def copy_main_files(self):
        try:
            shutil.copytree(self.folder_name, f"{const.FOLDER_OBJECTS}/{self.folder_name}")
            return True
        except FileExistsError:
            print(f"The folder \"{const.FOLDER_OBJECTS}/{self.folder_name}\" is already exists")
            return False

    def update_files(self):
        if not self.copy_main_files():
            return False
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
