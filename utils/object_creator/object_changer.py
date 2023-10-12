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

        user_code_blocks = read_files(options.folder_path)
        obj_r.remove_object()
        obj_g.object_code_generate()
        write_files(path_to_object_new, user_code_blocks)
        obj_i.update_files()
    else:
        parser.error("The path to the folder of the Object is not provided")
