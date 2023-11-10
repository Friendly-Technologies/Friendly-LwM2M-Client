import os
import json
import sys

import functions as func
import constants as const


class ObjectInitializer:
    def __init__(self, path_json_file, list_of_id):
        self.objects = None
        self.register_data = None
        self.log_tag = self.__class__.__name__
        self.path_json_file = path_json_file
        self.list_of_id = list_of_id

    def parse_list_of_id(self):
        list_of_id = self.list_of_id[1:-1]
        try:
            self.list_of_id = [int(obj_id) for obj_id in list_of_id.split(',')]
            return True
        except ValueError:
            func.LOG(self.log_tag, self.parse_list_of_id.__name__, "unable to parse list of Objects' ID.")
            return False

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
                         self.search_object.__name__,
                         f"the {data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]} Object meet the requirements")
                data_dict["object_folder"] = obj
                self.register_data = data_dict
                return True

        func.LOG(self.log_tag,
                 self.search_object.__name__,
                 f"unable to found the Object appropriate to required id {required_id} and version {required_version}")
        return False

    def generate_header(self):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_H)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        register_name_up = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_UNDERLINE]
        content = content.replace("__CLASS_NAME__", register_name)
        content = content.replace("<<IF_DEF_DIRECTIVE>>", register_name_up)
        func.create_file(f"{register_name}/{register_name}.h", content)
        return True

    def generate_cpp(self, instances_txt):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_CPP)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        content = content.replace("__INSTANCES__", instances_txt)
        content = content.replace("__CLASS_NAME__", register_name)
        func.create_file(f"{register_name}/{register_name}.cpp", content)
        return True

    def generate_cmake_lists(self):
        errcode, content = func.get_file_content(const.FILE_TMPLT_INIT_CMAKE)
        if not errcode:
            return False
        register_name = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        content = content.replace("__CLASS_NAME__", register_name)
        func.create_file(f"{register_name}/{const.FILE_CMAKE_LISTS}", content)
        return True

    def get_enum_of_resources(self, file):
        """
        Read header file of the existing Object, extract the enum of the
        resources, create dictionary based on enum and return this dictionary.
        """
        resources = []
        errcode, content = func.get_file_content_arr(file)
        if not errcode:
            func.LOG(self.log_tag,
                     self.get_enum_of_resources.__name__,
                     "unable to extract the enum of the resources from the file")
            return None
        flag_fill = False
        for line in content:
            resources_dict = {}
            if line.find("};") != -1:
                flag_fill = False
            if flag_fill:
                if line.find('#') != -1:
                    continue
                divided_line = line.split('=')                      # "SMS_NUMBER_9 = 9," -> ["SMS_NUMBER_9 ", " 9,"]
                divided_line = [i.strip() for i in divided_line]    # ["SMS_NUMBER_9 ", " 9,"] -> ["SMS_NUMBER_9", "9,"]
                resource = divided_line[0]                          # "SMS_NUMBER_9"
                number = int(divided_line[1][0:-1])                 # "9," -> 9
                resources_dict[const.KEY_NAME] = resource           # {"name: "SMS_NUMBER_9"}
                resources_dict[const.KEY_JSON_ID] = number          # {"name: "SMS_NUMBER_9", "id": 9}
                resources.append(resources_dict)
            if line.find("enum") != -1 and line.find("ID") != -1 and line.find("ID_T") != 1:
                flag_fill = True
        return resources


    def assign_value_to_resources(self, res_names, res_values):
        """
        Creates a new dictionary of the resources.
        The names of keys reach for that obtained from the existing Object (first parameter).
        The values reach for a list of dictionaries obtained from the provided JSON file (second parameter).
        """
        completed_res_dict = {}
        for resource_dict in res_values:
            res_id_1 = resource_dict[const.KEY_JSON_ID]
            try:
                res_1_val = resource_dict["value"]
            except KeyError:
                res_1_val = None
            # correct value in order to its types:
            if res_1_val is None:
                res_1_val = "(EXECUTE_T)[](ID_T id, const OPAQUE_T& data) {\n\t\treturn true;\n\t}"
            elif type(res_1_val) is dict:
                value = ""
                for i in res_1_val.values():
                    value += f"{i},"
                res_1_val = f"OBJ_LINK_T{{{value[0:-1]}}}"
            elif type(res_1_val) is str:
                res_1_val = f'STRING_T("{res_1_val}")'
            elif type(res_1_val) is bool:
                res_1_val = "true" if res_1_val else "false"
            for res_key, res_val in res_names.items():
                if res_id_1 == res_val:
                    completed_res_dict[res_key] = res_1_val
        return completed_res_dict

    def create_instances(self, obj):
        # filter empty structures of the instances:
        instances = [instance for instance in obj["instances"] if instance != {}]
        result = ""
        for instance in instances:
            name = self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
            folder = self.register_data["object_folder"]
            file = self.register_data[const.KEY_DICT_OBJ_FILES][const.KEY_FILE_IMPL_H]
            file_path = f"{const.FOLDER_OBJECTS}/{folder}/{file}"
            resources_1 = self.get_enum_of_resources(file_path)
            resources_2 = [i for i in instance["resources"] if i != {}]
            merged_dictionary = self.assign_value_to_resources(resources_1, resources_2)
            result += f"\twpp::Instance *{name} = testObj.createInstance({instance[const.KEY_JSON_ID]});\n"
            result += f"\t{name}->subscribe(this);\n"
            for resource_key, resources_value in merged_dictionary.items():
                if type(resources_value) is int:
                    for i in ["INT_T", "UINT_T", "FLOAT_T", "TIME_T"]:
                        result += f"\t{name}->set({name}::{resource_key}, {i}({resources_value}));\n"
                    continue
                elif type(resources_value) is list:
                    for i in resources_value:
                        if i == {}:
                            continue
                        result += (f"\t{name}->set({{{name}::{resource_key}, "
                                   f"{i[const.KEY_JSON_ID]}}}, {i[const.KEY_JSON_VAL]});\n")
                    continue
                result += f"\t{name}->set({name}::{resource_key}, {resources_value});\n"
        return result

    def initialize(self):
        """
        Returns True if initialized example is created successful.
        """
        must_be_checked = False

        if self.list_of_id is not None:
            if not self.parse_list_of_id():
                func.LOG(self.log_tag,
                         self.initialize.__name__,
                         "the list of Objects' ID not parsed. Operation interrupted.")
                return False
            must_be_checked = True
        if not self.get_init_data():
            func.LOG(self.log_tag,
                     self.initialize.__name__,
                     "unable to the list of Objects' ID not parsed. Operation interrupted.")
            return False
        for obj in self.objects:
            # check if it's required to initialize this Object (was indicated by second parameter):
            if must_be_checked and obj[const.KEY_JSON_ID] not in self.list_of_id:
                continue
            if not self.search_object(obj[const.KEY_JSON_ID], obj[const.KEY_VER]):
                continue
            # todo: must be return false if loop above will return false for each object
            instances_txt = self.create_instances(obj)
            func.create_folder(self.register_data[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS])
            if not self.generate_header() or not self.generate_cpp(instances_txt) or not self.generate_cmake_lists():
                return False
        return True
