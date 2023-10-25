import constants as const
import functions as func
import object_generator
import object_remover
import object_integrator

import os
import re
import shutil
from optparse import OptionParser

RES_DEF_PATTERN = r'#define RES_\d+_\d+_'
RES_DEF_PARTS_CNT = 3

ENUM_FIELD_PATTERN = r" = \d"
ENUM_START_PATTERN = "enum ID: ID_T {"
ENUM_END_PATTERN = "};"
ENUM_FIELD_PARTS_CNT = 2

FILE_TYPE_COMPONENTS_CNT = 2


class ObjectChanger:
    """Add some comments here"""

    def __init__(self, obj_folder_to_change, obj_metadata_to_use):
        self.user_codes_relations = None
        self.obj_folder_to_change = obj_folder_to_change
        self.obj_metadata_to_use = obj_metadata_to_use

    def set_relations(self, folders):
        """ note: the order of files on parameter is very important:
        the first must be old, the second is new"""
        relations = {}
        datas = []

        for folder in folders:
            # just read and save data to list:
            errcode, content = func.get_file_content(f"{folder}/{const.FILE_OBJ_METADATA}")
            if not errcode:
                return False
            datas.append(content)

        # parse, pack and save object_files-dict to Class-field:
        try:
            relations["old"] = eval(datas[0])[const.KEY_DICT_OBJ_FILES]
            relations["new"] = eval(datas[1])[const.KEY_DICT_OBJ_FILES]
            self.user_codes_relations = relations
            return True

        except KeyError:
            # print(f'There is no "{const.KEY_DICT_OBJ_FILES}" key on dictionaries')
            return False

    def get_info(self, path_to_file):
        add_line = False
        counter = 0
        user_code_block = ""
        user_code_blocks = {}
        with open(path_to_file, 'r') as f:
            for line in f:
                if re.search("block \d start", line):
                    add_line = True
                    continue

                if re.search("block \d end", line):
                    add_line = False
                    user_code_blocks[counter] = user_code_block
                    user_code_block = ""
                    counter += 1

                if add_line:
                    user_code_block += line
        f.close()

        return user_code_blocks

    def put_info(self, path_to_file, file_user_code_dict):
        counter = 0
        new_content = ''
        old_content = func.get_file_content(path_to_file)[1]
        add_flag = True
        
        for line in old_content.split("\n"):
            if re.search("block \d start", line):
                new_content += line + "\n"
                if file_user_code_dict[counter] == "":
                    counter += 1
                    continue
                add_flag = False
                new_content += file_user_code_dict[counter]
                counter += 1
            if re.search("block \d end", line):
                add_flag = True
            if add_flag:
                new_content += line + "\n"
        func.write_to_file(path_to_file, new_content[:-1])

    def read_files(self):
        print(self.read_files.__name__)
        datas = {}
        dir_list = os.listdir(self.obj_folder_to_change)
        for file in dir_list:
            file_path = f"{self.obj_folder_to_change}/{file}"
            file_type = self.get_file_type_by_name("old", file)
            print(file_path, "->", file_type)
            if not file_type:
                # print(f"Incorrect file type in '{file}'")
                continue
            user_codes = self.get_info(file_path)
            datas[file_type] = user_codes
        return datas

    def write_files(self, folder_path, datas):
        dir_list = os.listdir(folder_path)
        for file in dir_list:
            file_path = f"{folder_path}/{file}"
            file_type = self.get_file_type_by_name("new", file)
            if not file_type:
                # print(f"Incorrect file type in '{file_path}'")
                continue
            self.put_info(file_path, datas[file_type])

    def get_file_name_by_type(self, relations, file_type):
        try:
            return self.user_codes_relations[relations][file_type]
        except KeyError:
            return ""

    def get_file_type_by_name(self, relations, file_name):
        try:
            return list(self.user_codes_relations[relations].keys())[list(self.user_codes_relations[relations].values()).index(file_name)]
        except ValueError:
            return ""

    def get_obj_res_enum_content_from_file(self, obj_path):
        impl_file = self.get_file_name_by_type("old", const.KEY_FILE_IMPL_H)
        if not impl_file:
            print(f"File with type '{const.KEY_FILE_IMPL_H}' not found in the path '{obj_path}'")
            return ""

        with open(obj_path + "/" + impl_file, 'r') as file:
            lines = file.readlines()
            enum_start_id = -1
            enum_end_id = -1
            block = list()

            for id in range(len(lines)):
                if re.search(ENUM_START_PATTERN, lines[id]):
                    enum_start_id = id
                    break
            if enum_start_id == -1:
                return list()
            for id in range(id, len(lines)):
                if re.search(ENUM_END_PATTERN, lines[id]):
                    enum_end_id = id
                    break
                block.append(lines[id])
            if enum_end_id == -1:
                return list()
        return block

    def get_resource_names_old(self):
        print(self.get_resource_names_old.__name__)
        # Return ["line0", "line1", ...]
        res_enum = self.get_obj_res_enum_content_from_file(self.obj_folder_to_change)
        if not res_enum:
            print("Enum with resources definitions not found")
            return dict()

        names = dict()
        for line in res_enum:
            if not re.search(ENUM_FIELD_PATTERN, line):
                continue
            field_parts = line.replace(" ", "").strip("\t,\n").split("=")
            if len(field_parts) != ENUM_FIELD_PARTS_CNT or not field_parts[1].isnumeric():
                print(f"Incorrect enum field format '{line}'")
                continue
            names[int(field_parts[1])] = field_parts[0]
        return names

    def get_resource_names_new(self, obj_gen):
        print(self.get_resource_names_new.__name__)
        names = dict()
        for res in obj_gen.meta_resources:
            names[int(res['ID'])] = f"{res['Name']}_{res['ID']}"
        return names

    def create_map_for_attribute(self, attr1, attr2):
        result_map = list()
        for id in attr1:
            if id in attr2.keys():
                result_map.append([attr1[id], attr2[id]])
        return result_map

    def get_updated_user_code(self, object_generator_obj):
        print(self.get_updated_user_code.__name__)
        # Returns {ID: [<str>, <int>]}
        old_res_names = self.get_resource_names_old()
        new_res_names = self.get_resource_names_new(object_generator_obj)

        # Returns [[old_attr, new_attr],...]
        res_names_map = self.create_map_for_attribute(old_res_names, new_res_names)

        old_user_code_blocks = self.read_files()
        new_user_code_blocks = dict()
        for type, old_blocks in old_user_code_blocks.items():
            new_user_code_blocks[type] = dict()
            for id, old_block in old_blocks.items():
                new_block = old_block
                for name_pair in res_names_map:
                    new_block = new_block.replace(name_pair[0], name_pair[1])
                new_user_code_blocks[type][id] = new_block
        return new_user_code_blocks

    def change(self):
        # 1. generate new code from file/link/meta in current folder:
        obj_g = object_generator.ObjectGenerator(self.obj_metadata_to_use, None)  # TODO: implement cr-n by link
        obj_g.object_code_generate()
        path_to_new_object = obj_g.get_folder_path()

        # 2. check if there is possible and set the relations of user-code blocks (old_object -> updated_object):
        if not self.set_relations([self.obj_folder_to_change, path_to_new_object]):     # don't change order in list
            try:
                shutil.rmtree(path_to_new_object)
            except FileNotFoundError:
                pass
            return False

        # 3. update the code of the new Object by user-code block of the old Object:
        user_code_blocks = self.get_updated_user_code(obj_g)
        self.write_files(path_to_new_object, user_code_blocks)

        # 4. remove old Object code:
        obj_r = object_remover.ObjectRemover(self.obj_folder_to_change)
        obj_r.remove_object()

        # 5. integrate new code of the Object:
        obj_i = object_integrator.ObjectIntegrator(path_to_new_object)
        if not obj_i.update_files():
            print("Please, Check the destination of the Object folder and try again")


def main():
    parser = OptionParser()
    parser.add_option("--file", dest="file_path", help="The path to the xml file of the Object")
    parser.add_option("--folder", dest="folder_path", help="The path to the folder of the Object")
    options, args = parser.parse_args()
    if not options.file_path or not options.folder_path:
        parser.error("The path to the folder of the Object is not provided")
    oc = ObjectChanger(options.folder_path, options.file_path)
    return oc.change()


if __name__ == "__main__":
    main()
