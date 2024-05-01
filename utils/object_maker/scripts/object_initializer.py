import os

import functions as func
import constants as const


class ObjectInitializer:
    def __init__(self, path_json_file, list_of_id):
        self.initialization_data_list = None
        self.object_data_dict = None
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
        errcode, data_dict = func.get_json_from_file(self.path_json_file)
        if not errcode:
            func.LOG(self.log_tag, self.get_init_data.__name__,
                     f'unable to get data to initialize from "{self.path_json_file}" file. Operation interrupted.')
            return False
        self.initialization_data_list = [obj for obj in data_dict["objects"] if obj != {}]
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
            json_file_of_obj = f"{registers_folder}/{obj}/{const.FILE_OBJ_METADATA}"
            errcode, data_dict = func.get_json_from_file(json_file_of_obj)
            if not errcode:
                func.LOG(self.log_tag, self.search_object.__name__,
                         f'invalid "{json_file_of_obj}". Continue searching...')
            try:
                obtained_id = int(data_dict[const.KEY_DICT_OBJ_META][const.KEY_ID])
                obtained_version = float(data_dict[const.KEY_DICT_OBJ_META][const.KEY_VER])
            except KeyError as e:
                func.LOG(self.log_tag, self.search_object.__name__,
                         f"unable to parse json-file of the Object ({e} key not found). Continue searching...")
                continue

            existing_object_name = data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
            if obtained_id == required_id and obtained_version == required_version:
                func.LOG(self.log_tag, self.search_object.__name__,
                         f"the {existing_object_name} Object meet the requirements")
                data_dict["object_folder"] = obj
                self.object_data_dict = data_dict
                return True
            # func.LOG(self.log_tag,
            #          self.search_object.__name__,
            #          f"the {existing_object_name} Object doesn't meet the requirements. Continue searching...")
            continue

        func.LOG(self.log_tag, self.search_object.__name__,
                 f"unable to found the Object appropriate to required id {required_id} and version {required_version}")
        return False

    def generate_header(self, includes, inheritances, callbacks):
        errcode, content = func.get_content_from_file(const.FILE_TMPLT_INIT_H)
        if not errcode:
            return False
        register_name = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        register_name_up = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_UNDERLINE]
        content = content.replace("__CLASS_NAME__", register_name)
        content = content.replace("__INCLUDES__", includes)
        content = content.replace("__INHERITANCES__", inheritances)
        content = content.replace("__CALLBACKS__", callbacks)
        content = content.replace("<<IF_DEF_DIRECTIVE>>", register_name_up)
        func.create_file(f"{register_name}/{register_name}.h", content)
        return True

    def generate_cpp(self, instances_txt, callbacks_txt, obj_subscribes_txt):
        errcode, content = func.get_content_from_file(const.FILE_TMPLT_INIT_CPP)
        if not errcode:
            return False
        register_name = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        content = content.replace("__CALLBACKS__", callbacks_txt)
        content = content.replace("__INSTANCES__", instances_txt)
        content = content.replace("__SUBSCRB_1__", obj_subscribes_txt)
        content = content.replace("__CLASS_NAME__", register_name)
        func.create_file(f"{register_name}/{register_name}.cpp", content)
        return True

    def generate_cmake_lists(self):
        errcode, content = func.get_content_from_file(const.FILE_TMPLT_INIT_CMAKE)
        if not errcode:
            return False
        register_name = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
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
            func.LOG(self.log_tag, self.get_enum_of_resources.__name__,
                     "unable to extract the enum of the resources from the file")
            return None
        flag_fill = False
        enum_elements_counter = 0
        for line in content:
            line = str(line)
            resources_dict = {}
            if line.replace(" ", "").find(const.ENUM_END_PATTERN.replace(" ", "")) != -1:
                flag_fill = False
            if flag_fill:
                if line.find('#') != -1:
                    continue
                divided_line = line.split('=')                      # "SMS_NUMBER_9 = 9," -> ["SMS_NUMBER_9 ", " 9,"]
                divided_line = [i.strip() for i in divided_line]    # ["SMS_NUMBER_9 ", " 9,"] -> ["SMS_NUMBER_9", "9,"]
                if len(divided_line) == 1:                          # "SMS_NUMBER_9," -> ["SMS_NUMBER_9,"]
                    resource = divided_line[:-1]                    # ["SMS_NUMBER_9,"] -> "SMS_NUMBER_9"
                    number = enum_elements_counter
                else:
                    resource = divided_line[0]                      # "SMS_NUMBER_9"
                    number = int(divided_line[1][0:-1])             # "9," -> 9
                resources_dict[const.KEY_NAME] = resource           # {"name: "SMS_NUMBER_9"}
                resources_dict[const.KEY_ID] = number               # {"name: "SMS_NUMBER_9", "id": 9}
                resources.append(resources_dict)
                enum_elements_counter += number + 1
            if line.replace(" ", "").find(const.ENUM_START_PATTERN.replace(" ", "")) != -1:
                # print("START LINE: " + line)
                flag_fill = True
        return resources

    def transform_types_of_value(self, res_value, res_type):
        """
        Transform value in order to its types.
        """
        if res_type in ["INT_T", "UINT_T", "FLOAT_T", "TIME_T"]:
            return f'{res_type}({res_value})'
        if res_type in ["CORE_LINK_T", "STRING_T"]:
            return f'{res_type}("{res_value}")'
        elif res_type == "EXECUTE_T":
            return "(EXECUTE_T)[](Instance& inst, ID_T id, const OPAQUE_T& data) {\n\t\treturn true;\n\t}"
        elif res_type == "BOOL_T":
            return "true" if res_value else "false"
        elif res_type == "OPAQUE_T":
            bytes_values = [f"0x{res_value[i:i + 2]}" for i in range(0, len(res_value), 2)]
            bytes_values_j = ", ".join(bytes_values)
            return f"OPAQUE_T{{{bytes_values_j}}}"
        elif res_type == "OBJ_LINK_T":
            values = [str(i) for i in res_value.values()]
            return f"OBJ_LINK_T{{{', '.join(values)}}}"

    def is_resource_id_valid(self, resources_from_json, resources_from_obj):
        """
        Checks if obtained Object have the required resource (by id).
        """
        existing_ids = [i[const.KEY_ID] for i in resources_from_obj]
        required_ids = [i[const.KEY_ID] for i in resources_from_json]

        for id in required_ids:
            if id not in existing_ids:
                func.LOG(self.log_tag, self.is_resource_id_valid.__name__,
                         f"the existing Object haven't required id ({id})")
                return False
        return True

    def assign_value_to_resources(self, resources_from_json, resources_from_obj):
        """
        Creates a new list of the dictionary of the resources.
            Param 1: list of dict contains ids and values of the resources obtained from a provided JSON file.
            Param 2: list of dict contains ids and names of the resources obtained from the existing Object.
        """
        completed_resources = []
        # if len(resources_from_json) != len(resources_from_obj):
        #     func.LOG(self.log_tag, self.assign_value_to_resources.__name__,
        #              f"INFO: the number of resources from the existing Object "
        #              f"and the provided JSON do not match each other "
        #              f" ({len(resources_from_obj)}!={len(resources_from_json)})")
        for resource_dict_json in resources_from_json:
            completed_res_dict = {}
            if const.KEY_VALUE not in resource_dict_json:
                resource_dict_json[const.KEY_VALUE] = None
            res_id_json = resource_dict_json[const.KEY_ID]
            for resource_dict_obj in resources_from_obj:
                res_id_obj = resource_dict_obj[const.KEY_ID]
                if res_id_json == res_id_obj:
                    completed_res_dict[const.KEY_ID] = resource_dict_obj[const.KEY_ID]
                    completed_res_dict[const.KEY_NAME] = resource_dict_obj[const.KEY_NAME]
                    resource_dict_obj[const.KEY_VALUE] = resource_dict_json[const.KEY_VALUE]
                    completed_resources.append(resource_dict_obj)
        # [print(i) for i in completed_resources]
        return completed_resources

    def assign_type_to_resources(self, data, resources):
        for resource in resources:
            for line in data:
                if line.find(resource[const.KEY_NAME]) == -1:
                    continue
                for res_type in const.WPP_TYPES:
                    if line.find(res_type) != -1:
                        resource[const.KEY_TYPE] = f"{res_type}_T"
        # [print(i) for i in resources]
        return resources

    def check_multiple_resource(self, data, resources):
        for resource in resources:
            for line in data:
                if line.find(resource[const.KEY_NAME]) == -1:
                    continue
                if line.find("MULTIPLE") == -1 and type(resource[const.KEY_VALUE]) == list:
                    return False
        return True

    def is_object_multiple(self):
        return self.object_data_dict[const.KEY_DICT_OBJ_META][const.KEY_IS_MULTIPLE]

    def get_resources_parameters(self, file):
        """
        Reads cpp file of the existing Object line by line, extract
        the enum of the resources and return it as list of strings.
        """
        errcode, content = func.get_file_content_arr(file)
        if not errcode:
            func.LOG(self.log_tag, self.assign_type_to_resources.__name__,
                     "unable to get the types of the resources from the file")
            return None
        data = []
        flag_fill = False
        for line in content:
            if line.replace(" ", "").find(const.ENUM_END_PATTERN.replace(" ", "")) != -1:
                flag_fill = False
            if flag_fill:
                if line.find('#if') != -1 or line.find('#endif') != -1:
                    continue
                data.append(line.strip())
            if line.find("std::vector") != -1 and line.find("Resource") != -1:
                flag_fill = True
        # [print(i) for i in data]
        return data

    def create_instances(self, obj, types_enabled):
        name = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
        folder = self.object_data_dict["object_folder"]
        file_h = self.object_data_dict[const.KEY_DICT_OBJ_FILES][const.KEY_FILE_IMPL_H]
        file_cpp = self.object_data_dict[const.KEY_DICT_OBJ_FILES][const.KEY_FILE_IMPL_CPP]
        file_path_h = f"{const.FOLDER_OBJECTS}/{folder}/{file_h}"
        file_path_cpp = f"{const.FOLDER_OBJECTS}/{folder}/{file_cpp}"

        # filter empty structures of the instances:
        instances = [instance for instance in obj["instances"] if instance != {}]

        if len(instances) > 1 and not self.is_object_multiple():
            func.LOG(self.log_tag, self.create_instances.__name__,
                     f"the {name} Object is not multiple but there is a requirement")
            return False, ""

        result = ""
        instances_counter = 0
        for instance in instances:
            resources_from_json = [i for i in instance["resources"] if i != {}]
            resources_from_obj = self.get_enum_of_resources(file_path_h)
            if not self.is_resource_id_valid(resources_from_json, resources_from_obj):
                func.LOG(self.log_tag, self.assign_value_to_resources.__name__, "invalid required ID.")
                return False, ""

            resources_parameters = self.get_resources_parameters(file_path_cpp)
            resources_merged = self.assign_value_to_resources(resources_from_json, resources_from_obj)
            if not self.check_multiple_resource(resources_parameters, resources_merged):
                func.LOG(self.log_tag, self.assign_value_to_resources.__name__,
                         "invalid single/multiple parameter of the resource.")
                return False, ""

            resources_merged = self.assign_type_to_resources(resources_parameters, resources_merged)
            # [print(i) for i in resources_merged]
            instance_name = f"inst{instances_counter}"
            instance_id = instance[const.KEY_ID]
            result += f"\n\twpp::Instance *{instance_name} = obj.createInstance({instance_id});\n"
            if types_enabled["type3"]:
                result += f"\tinst{instances_counter}.opSubscribe(this);\n"
            elif types_enabled["type4"]:
                result += f"\tinst{instances_counter}.eventSubscribe(this);\n"
            for resource_dict in resources_merged:
                resources_value = resource_dict[const.KEY_VALUE]
                resources_type = resource_dict[const.KEY_TYPE]
                if type(resources_value) is list:
                    res_instances_list = [inst for inst in resources_value if inst != {}]
                    for res_instance_dict in res_instances_list:
                        value = self.transform_types_of_value(res_instance_dict[const.KEY_VALUE], resources_type)
                        result += f'\t{instance_name}->set({{{name}::{resource_dict[const.KEY_NAME]}}}, {value});\n'
                    continue
                value = self.transform_types_of_value(resources_value, resources_type)
                result += f'\t{instance_name}->set({name}::{resource_dict[const.KEY_NAME]}, {value});\n'
            instances_counter += 1
        return True, result

    def create_obj_subscribe(self, types_enabled):
        result = ""
        if types_enabled["type1"]:
            result += f"\tobj.opSubscribe(this);\n"
        elif types_enabled["type2"]:
            result += f"\tobj.actSubscribe(this);\n"
        return True, result

    def define_types_enabled(self, object_dict):
        types_enabled_dict = {
            "type1": object_dict["inst_create_clb"] or object_dict["inst_delete_clb"],
            "type2": object_dict["inst_restore_clb"],
            "type3": object_dict["res_read_clb"] or object_dict["res_write_clb"] or
                     object_dict["res_execute_clb"] or object_dict["res_replace_clb"],
            "type4": object_dict["res_inst_event_clb"]
        }
        return types_enabled_dict

    def create_includes(self, types_enabled_dict):
        includes = '#include "WppRegistry.h"\n'
        if types_enabled_dict["type1"]:
            includes += '#include "ObjOpObserver.h"\n'
        if types_enabled_dict["type2"]:
            includes += '#include "ObjActObserver.h"\n'
        if types_enabled_dict["type3"]:
            includes += '#include "InstOpObserver.h"\n'
        if types_enabled_dict["type4"]:
            includes += '#include "InstEventObserver.h"\n'
        return includes

    def create_inheritance(self, types_enabled_dict):
        inheritances = []
        if types_enabled_dict["type1"]:
            inheritances.append("public wpp::ObjOpObserver")
        if types_enabled_dict["type2"]:
            inheritances.append("public wpp::ObjActObserver")
        if types_enabled_dict["type3"]:
            inheritances.append("public wpp::InstOpObserver")
        if types_enabled_dict["type4"]:
            inheritances.append("public wpp::InstEventObserver")

        inheritances_txt = ", ".join(inheritances)
        return inheritances_txt

    def create_callbacks(self, object_dict):
        callbacks_h = ""
        callbacks_cpp = ""
        if object_dict["inst_create_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::instanceCreated(Object &object, ID_T instanceId) {\n}\n\n"
            callbacks_h += "\tvoid instanceCreated(Object &object, ID_T instanceId) override;\n"
        if object_dict["inst_delete_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::instanceDeleting(Object &object, ID_T instanceId) {\n}\n\n"
            callbacks_h += "\tvoid instanceDeleting(Object &object, ID_T instanceId) override;\n"
        if object_dict["inst_restore_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::objectRestore(Object &object) {\n\tobject.clear();\n\tinit(object);\n}\n\n"
            callbacks_h += "\tvoid objectRestore(Object &object) override;\n"
        if object_dict["res_read_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::resourceRead(Instance &inst, const ResLink &resId) {\n}\n\n"
            callbacks_h += "\tvoid resourceRead(Instance &inst, const ResLink &resId) override;\n"
        if object_dict["res_write_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::resourceWrite(Instance &inst, const ResLink &resId) {\n}\n\n"
            callbacks_h += "\tvoid resourceWrite(Instance &inst, const ResLink &resId) override;\n"
        if object_dict["res_execute_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::resourceExecute(Instance &inst, const ResLink &resId) {\n}\n\n"
            callbacks_h += "\tvoid resourceExecute(Instance &inst, const ResLink &resId) override;\n"
        if object_dict["res_replace_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::resourcesReplaced(Instance &inst) {\n}\n\n"
            callbacks_h += "\tvoid resourcesReplaced(Instance &inst) override;\n"
        if object_dict["res_inst_event_clb"]:
            callbacks_cpp += "void __CLASS_NAME__Impl::instEvent(Instance &inst, EVENT_ID_T eventId) {\n}\n\n"
            callbacks_h += "\tvoid instEvent(Instance &inst, EVENT_ID_T eventId) override;\n"
        return callbacks_h, callbacks_cpp

    def initialize(self):
        """
        Returns True if initialized example is created successful.
        """
        errcode = 0
        must_be_checked = False

        if self.list_of_id is not None:
            if not self.parse_list_of_id():
                func.LOG(self.log_tag, self.initialize.__name__,
                         "the list of Objects' ID not parsed. Operation interrupted.")
                errcode = 1
                return errcode
            must_be_checked = True
        if not self.get_init_data():
            func.LOG(self.log_tag, self.initialize.__name__,
                     "unable to get data for initializing. Operation interrupted.")
            errcode = 1
            return errcode

        for initialization_data_item in self.initialization_data_list:
            # check if it's required to initialize this Object (was indicated by second parameter):
            if must_be_checked and initialization_data_item[const.KEY_ID] not in self.list_of_id:
                continue
            print("===================================================================================================")
            func.LOG(self.log_tag, self.initialize.__name__,
                     f"the example with ID {initialization_data_item[const.KEY_ID]} initializing...")
            if not self.search_object(initialization_data_item[const.KEY_ID], initialization_data_item[const.KEY_VER]):
                func.LOG(self.log_tag, self.initialize.__name__, "Operation interrupted.")
                errcode = 1
                continue
            # todo: must be return false if loop above will return false for each object
            folder_name = self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]
            if not func.create_folder(self.object_data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_CLASS]):
                func.LOG(self.log_tag, self.initialize.__name__,
                         f"the folder {folder_name} is already exists. Operation interrupted.")
                errcode = 1
                continue
            callbacks_txt_h, callbacks_txt_cpp = self.create_callbacks(initialization_data_item)
            # CmakeLists stuff:
            if not self.generate_cmake_lists():
                errcode = 1
                return errcode
            # header stuff:
            types_enabled = self.define_types_enabled(initialization_data_item)

            includes_txt = self.create_includes(types_enabled)
            inheritance_txt = self.create_inheritance(types_enabled)
            if not self.generate_header(includes_txt, inheritance_txt, callbacks_txt_h):
                errcode = 1
                return errcode
            # cpp stuff:
            errcode, instances_txt = self.create_instances(initialization_data_item, types_enabled)
            if not errcode:
                func.LOG(self.log_tag, self.initialize.__name__, f"Operation interrupted.")
                errcode = 1
                return errcode
            errcode, obj_subscribes_txt = self.create_obj_subscribe(types_enabled)
            if not errcode:
                func.LOG(self.log_tag, self.initialize.__name__, f"Operation interrupted.")
                errcode = 1
                return errcode

            if not self.generate_cpp(instances_txt, callbacks_txt_cpp, obj_subscribes_txt):
                errcode = 1
                return errcode
            func.LOG(self.log_tag, self.initialize.__name__,
                     f"the example with ID {initialization_data_item[const.KEY_ID]} initialized successfully")
        return errcode
