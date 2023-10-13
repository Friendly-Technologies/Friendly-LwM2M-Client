import object_generator
import object_remover
import object_integrator

from optparse import OptionParser
import re
import os


def get_info(path_to_file):
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


def put_info(path_to_file, file_user_code_dict):
    counter = 0
    new_content = ''
    old_content = ''
    add_flag = True
    with open(path_to_file, 'r') as f:
        for i in f:
            old_content += i
    f.close()

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

    with open(path_to_file, 'w') as f:
        f.write(new_content[:-1])
    f.close()


def read_files(path):
    datas = {}
    dir_list = os.listdir(path)
    for file in dir_list:
        datas[scan_filename(file)] = get_info(f"{path}/{file}")
    return datas


def write_files(folder_path, datas):
    dir_list = os.listdir(folder_path)
    for file_playn in dir_list:
        file_keyword = scan_filename(file_playn)
        put_info(f"{folder_path}/{file_playn}", datas[file_keyword])


def scan_filename(filename: str):
    keywords = ["Config", "Info", "CMakeLists", ".h", ".cpp"]
    for keyword in keywords:
        if re.search(keyword, filename):
            return keyword


def get_path_to_obj_file(obj_path, type):
    files_list = os.listdir(obj_path)
    for file_name in files_list:
        with open(obj_path+"/"+file_name, 'r') as file:
            for line in file:
                if re.search(f"File type: {type}", line):
                    return obj_path+"/"+file_name
    return ""


def get_resource_defines_from_file(obj_path):
    cfg_file = get_path_to_obj_file(obj_path, "Object configuration")
    if not cfg_file:
        return dict()

    defines = dict()
    with open(obj_path+"/"+cfg_file, 'r') as file:
        for line in file:
            if not re.search("#define RES_\d_\d_", line):
                continue
            line_parts = line.split(" ")
            if len(line_parts) < 3:
                print(f"Incorrect definition format '{line}'")
                continue
            define_parts = line_parts[1].split("_")
            if len(define_parts) < 4:
                print(f"Incorrect define name format '{line_parts[1]}'")
                continue
            if not define_parts[2].isnumeric():
                print(f"Incorrect define prefix format '{line_parts[1]}'")
                continue
            defines[int(define_parts[2])] = line_parts[1]


def get_obj_res_enum_content_from_file(obj_path):
    impl_file = get_path_to_obj_file(obj_path, "Object implementation header")
    if not impl_file:
        return ""

    defines = dict()
    with open(obj_path + "/" + impl_file, 'r') as file:
        lines = file.readlines()
        enum_start_id = -1
        enum_end_id = -1
        block = list()

        for id in range(len(lines)):
            if re.search("enum ID: ID_T {", lines[id]):
                enum_start_id = id
                break
        if enum_start_id == -1:
            return list()
        for id in range(id, len(lines)):
            if re.search("};", lines[id]):
                enum_end_id = id
                break
            block.append(lines[id])
        if enum_end_id == -1:
            return list()

        return block


def get_resource_names_from_file(obj_path):
    # Return ["line0", "line1", ...]
    res_enum = get_obj_res_enum_content_from_file(obj_path)
    if not res_enum:
        return dict()

    names = dict()
    for line in res_enum:
        if not re.search(" = \d", line):
            continue
        field_parts = line.strip("\t ,\n").split("=")
        if len(field_parts) != 2 or not field_parts[1].isnumeric():
            print(f"Incorrect enum field format '{line}'")
            continue

        names[int(field_parts[1])] = field_parts[0]


def get_resource_defines_from_gen(obj_gen):
    defines = dict()
    for res in obj_gen.meta_resources:
        defines[res['ID']] = res['Define']
    return defines


def get_resource_names_from_gen(obj_gen):
    names = dict()
    for res in obj_gen.meta_resources:
        names[res['ID']] = f"{res['Name']}_{res['ID']}"
    return names


def create_map_for_attribute(attr1, attr2):
    result_map = list()
    for id in attr1:
        if id in attr2.keys():
            result_map.append([attr1[id], attr2[id]])
    return result_map


def get_updated_user_code(old_object_path, new_obj_gen):
    # Returns {ID: <str>}
    old_res_defines = get_resource_defines_from_file(old_object_path)
    old_res_names = get_resource_names_from_file(old_object_path)
    new_res_defines = get_resource_defines_from_gen(new_obj_gen)
    new_res_names = get_resource_names_from_gen(new_obj_gen)

    # Returns [[old_attr, new_attr],...]
    res_defs_map = create_map_for_attribute(old_res_defines, new_res_defines)
    res_names_map = create_map_for_attribute(old_res_names, new_res_names)

    old_user_code_blocks = read_files(old_object_path)
    new_user_code_blocks = dict()
    for blocks in old_user_code_blocks.values():
        for id, old_block in blocks.items():
            new_block = ""
            for def_pair in res_defs_map:
                new_block += old_block.replace(def_pair[0], def_pair[1])
            for name_pair in res_names_map:
                new_block += old_block.replace(name_pair[0], name_pair[1])
            new_user_code_blocks[id] = new_block
    return new_user_code_blocks


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("--file", dest="file_path", help="The path to the xml file of the Object")
    parser.add_option("--folder", dest="folder_path", help="The path to the folder of the Object")
    options, args = parser.parse_args()

    if options.file_path is not None and options.folder_path is not None:
        obj_r = object_remover.ObjectRemover(options.folder_path)
        obj_g = object_generator.ObjectGenerator(options.file_path, None)
        obj_i = object_integrator.ObjectIntegrator(options.file_path, None)

        path_to_object_old = options.folder_path
        path_to_object_new = obj_g.get_folder_path()

        user_code_blocks = get_updated_user_code(path_to_object_old, obj_g)
        obj_r.remove_object()
        obj_g.object_code_generate()
        write_files(path_to_object_new, user_code_blocks)
        obj_i.update_files()
    else:
        parser.error("The path to the folder of the Object is not provided")
