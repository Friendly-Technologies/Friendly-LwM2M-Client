import shutil

files = ["../wpp/registry/ObjectID.h",
         "../wpp/config/config.cmake",
         "../wpp/registry/WppRegistry.h",
         "../wpp/registry/WppRegistry.cpp"]


def update_file(define, path_to_file):
    old_content = ''
    new_content = ''
    with open(path_to_file, 'r') as f:
        for i in f:
            old_content += i
    f.close()

    if old_content.find(define) == -1:
        print(f"The file {path_to_file} is not contain Object that must be removed")
        return

    add_line = True     # the flag indicates whether leave the line
    for line in old_content.split("\n"):
        line_to_check = line.strip().replace(" ", "")

        if line_to_check in [f"#if{define}", f"#ifdef{define}"] or line_to_check.find(f"option({define}") != -1:
            add_line = False

        if add_line:
            new_content += line + "\n"

        else:   # check if all that must be deleted is already deleted:
            if line_to_check[:6] in ["#endif", "endif("]:
                add_line = True

    with open(path_to_file, 'w') as f:
        f.write(new_content[:-1])
    f.close()


def update_files(define, files_list):
    for file in files_list:
        update_file(define, file)


def extract_define(path_to_file):
    define = ""
    try:
        with open(f"{path_to_file}/CMakeLists.txt", 'r') as f:
            for line in f:
                line = line.replace(" ", "")
                if line[:3] == "if(":
                    define = line[3:-2]  # remove "if(" and ")" symbols
        f.close()
    except FileNotFoundError:
        print("There is no files to extract the define with")

    return define


def remove_object_folder(path_to_object_folder):
    try:
        shutil.rmtree(path_to_object_folder)
    except FileNotFoundError:
        print("There is no folder/files to remove")


path_to_object = f"../wpp/registry/objects/mandatory/lwm2m_server_1_1"
obj_define = extract_define(path_to_object)
update_files(obj_define, files)
remove_object_folder(path_to_object)
