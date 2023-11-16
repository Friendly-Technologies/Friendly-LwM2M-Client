import constants as const
import functions as func

import sys
import shutil
from optparse import OptionParser


class ObjectIntegrator:
    """Add some comments here"""

    def __init__(self, folder_name):
        self.log_tag = f"[{self.__class__.__name__}]:"
        self.folder_name = folder_name

    def update_file(self, stop_string, content, path_to_file):
        is_stop_string_present = False
        new_content = ''
        errcode, old_content = func.get_content_from_file(path_to_file)

        if not errcode:
            print(f'{self.log_tag} The file "{path_to_file}" not found')
            return 

        if old_content.find(content) != -1:
            print(f'{self.log_tag} The file "{path_to_file}" is already updated')
            return

        for line in old_content.split("\n"):
            if " ".join(line.split()) == stop_string:
                is_stop_string_present = True
                new_content += content
            new_content += line + "\n"

        if not is_stop_string_present:
            print(f'{self.log_tag} The "{path_to_file}" file was not updated')
            return

        func.write_to_file(path_to_file, new_content[:-1])

    def insert_additional_data(self):
        file_path = f"{self.folder_name}/{const.FILE_OBJ_METADATA}"

        errcode, data_dict = func.get_json_from_file(file_path)
        if not errcode:
            func.LOG(self.log_tag,
                     self.set_relations.__name__,
                     f'the "{file_path}" file not found. Operation interrupted.')
            return False
        
        type_obj = const.TYPE_OBJECT        

        dict_obj_meta = data_dict[const.KEY_DICT_OBJ_META]
        dict_obj_names = data_dict[const.KEY_DICT_OBJ_NAMES]

        obj_is_mandatory = dict_obj_meta[const.KEY_IS_MANDATORY]
        obj_id = dict_obj_meta[const.KEY_ID]

        obj_name_class = dict_obj_names[const.KEY_NAME_CLASS]
        obj_name_define = dict_obj_names[const.KEY_NAME_DEFINE]
        obj_name_camelcase = dict_obj_names[const.KEY_NAME_CAMELCASE]
        obj_name_underline = dict_obj_names[const.KEY_NAME_UNDERLINE]

        stop_string_obj_id = const.STOP_STRING_OBJ_ID[0] if obj_is_mandatory else const.STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = const.STOP_STRING_CNFG_CMK[0] if obj_is_mandatory else const.STOP_STRING_CNFG_CMK[1]
        stop_string_reg_cpp_init = const.STOP_STRING_REG_CPP_INIT[0] if obj_is_mandatory else const.STOP_STRING_REG_CPP_INIT[1]
        stop_string_reg_cpp_method = const.STOP_STRING_REG_CPP_METHOD[0] if obj_is_mandatory else const.STOP_STRING_REG_CPP_METHOD[1]
        stop_string_reg_incl = const.STOP_STRING_REG_INCL[0] if obj_is_mandatory else const.STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = const.STOP_STRING_REG_PRT[0] if obj_is_mandatory else const.STOP_STRING_REG_PRT[1]

        content_obj_id = \
            f"\t#ifdef {obj_name_define}\n" \
            f"\t{obj_name_underline} = {obj_id},\n" \
            f"\t#endif\n"

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

        content_reg_cpp_init = \
            f"""\t# if {obj_name_define}\n""" \
            f"""\t_objects.push_back(new {type_obj}<{obj_name_class}>(_context, {obj_name_underline}_OBJ_INFO));\n""" \
            f"""\t#endif\n"""

        content_reg_cpp_method = \
            f"""# if {obj_name_define}\n""" \
            f"""{type_obj}<{obj_name_class}> & WppRegistry::{obj_name_camelcase}() {{\n\t""" \
            f"""return *static_cast<{type_obj}<{obj_name_class}>*>(object(OBJ_ID::{obj_name_underline}));\n}}\n#endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, const.FILE_OBJECT_ID)
        self.update_file(stop_string_cfg_cmk, content_cfg_cmake, const.FILE_CONFIG_CMAKE)
        self.update_file(stop_string_reg_cpp_init, content_reg_cpp_init, const.FILE_REGISTRY_CPP)
        self.update_file(stop_string_reg_cpp_method, content_reg_cpp_method, const.FILE_REGISTRY_CPP)
        self.update_file(stop_string_reg_incl, content_reg_h_include, const.FILE_REGISTRY_H)
        self.update_file(stop_string_reg_prot, content_reg_h_prototype, const.FILE_REGISTRY_H)
        return True

    def copy_main_files(self):
        file_path = f"{const.FOLDER_OBJECTS}/{self.folder_name}"
        try:
            shutil.copytree(self.folder_name, file_path)
            return True
        except FileExistsError:
            print(f'{self.log_tag} The folder "{file_path}" already exists. Operation interrupted.')
            return False

    def update_files(self):
        if not self.copy_main_files():
            return False
        if not self.insert_additional_data():
            return False
        func.remove_folder(self.folder_name)
        return True


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--folder", dest="obj_folder", help="the folder name of the Object")

    options, args = parser.parse_args()

    if not options.obj_folder:
        parser.error("the folder of the Object is not provided")

    oi = ObjectIntegrator(options.obj_folder)
    if not oi.update_files():
        sys.exit(1)
