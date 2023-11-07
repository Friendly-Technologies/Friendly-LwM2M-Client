import os
import json
import sys

import functions as func
import constants as const


class ObjectInitializer:
    def __init__(self, path_json_file):
        self.objects = None
        self.register_data = None
        self.log_tag = self.__class__.__name__
        self.path_json_file = path_json_file

    def get_init_data(self):
        """
        Reads data from provided json file. Save the list of the Objects to field of class.
        """
        errcode, data_str = func.get_file_content(self.path_json_file)
        if not errcode:
            print(f'{self.log_tag} There is no file with the metadata of the Object'
                  f'"{self.path_json_file}". Operation interrupted.')
            return False
        try:
            data_dict = json.loads(data_str)
        except json.decoder.JSONDecodeError:
            print(f'{self.log_tag} Unable to pars provided json-file. Operation interrupted.')
            return False
        self.objects = [obj for obj in data_dict["objects"] if obj != {}]
        return True

    def search_object(self, required_id, required_version):
        """
        Search necessary Object by id and version by checking files "object_metadata.json".
        """
        # create the list of existing Objects in the system:
        registers_folder = const.FOLDER_OBJECTS
        list_of_objects = [f for f in os.listdir(registers_folder) if not os.path.isfile(f"{registers_folder}/{f}")]
        # search required Object from existing Objects:
        for obj in list_of_objects:
            errcode, data_str = func.get_file_content(f"{registers_folder}/{obj}/{const.FILE_OBJ_METADATA}")
            if not errcode:
                # func.print_log(self.log_tag, "There is no file with the metadata of the Object.")
                continue
            # TODO: create separated function in FUNCTIONS to open and parse json
            try:
                data_dict = json.loads(data_str)
            except json.decoder.JSONDecodeError:
                print(f'{self.log_tag} Unable to pars provided json-file. Operation interrupted.')
                return False
            # TODO ~
            # TODO: do not convert type here but create json with this types:
            obtained_id = int(data_dict[const.KEY_DICT_OBJ_META][const.KEY_JSON_ID])
            obtained_version = float(data_dict[const.KEY_DICT_OBJ_META][const.KEY_VER])
            # TODO ~
            if obtained_id == required_id and obtained_version == required_version:
                func.LOG(self.log_tag,
                         self.search_registry.__name__,
                         f"the {data_dict[const.KEY_DICT_OBJ_NAMES]['class']} meet the requirements ")
                self.register_data = data_dict
                return True

        func.LOG(self.log_tag,
                 self.search_registry.__name__,
                 f"unable to found the Object appropriate to required id {required_id} and version {required_version}")
        return False

    def generate_header(self):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_H)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES]['class']
        register_name_up = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_UNDERLINE]
        content = content.replace("__CLASS_NAME__", register_name)
        content = content.replace("<<IF_DEF_DIRECTIVE>>", register_name_up)
        func.create_file(f"{register_name}/{register_name}.h", content)
        return True

    def generate_cpp(self):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_CPP)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES]['class']
        content = content.replace("__CLASS_NAME__", register_name)
        func.create_file(f"{register_name}/{register_name}.cpp", content)
        return True

    def generate_cmake_lists(self):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_CMAKE)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES]['class']
        content = content.replace("__CLASS_NAME__", register_name)
        func.create_file(f"{register_name}/{const.FILE_CMAKE_LISTS}", content)
        return True

    def create_registers(self):
        pass

    def create_instances(self):
        pass

    def initialize(self):
        """
        Returns True if initialized example is created successful.
        """
        if not self.get_init_data():
            return False
        for obj in self.objects:
            if not self.search_object(obj[const.KEY_JSON_ID], obj[const.KEY_VER]):
                continue
            func.create_folder(self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS])
            if not self.generate_header() or not self.generate_cpp() or not self.generate_cmake_lists():
                return False
        return True
