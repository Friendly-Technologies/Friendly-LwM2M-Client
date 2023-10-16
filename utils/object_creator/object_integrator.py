import shutil
import os
from optparse import OptionParser

FOLDER_OBJECTS = "../../wpp/registry/objects"
FOLDER_CONFIG = "../../wpp/config"
FOLDER_REGISTRY = "../../wpp/registry"

FILE_ADD_DATA = "additional.json"


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
            return

        data_dict = eval(data_str)

        for i in data_dict:
            stop_string = data_dict[i]["stop_string"]
            content = data_dict[i]["content"]
            file_name = data_dict[i]['file_name']

            folder = FOLDER_CONFIG if file_name == "config.cmake" else FOLDER_REGISTRY
            self.update_file(stop_string, content, f"{folder}/{file_name}")

    def remove_unused_files(self, path, file):
        try:
            os.remove(f"{path}/{file}")
        except FileNotFoundError:
            print(f"No such file \"{path}/{file}\"")

    def copy_main_files(self):
        try:
            shutil.copytree(self.folder_name, f"{FOLDER_OBJECTS}/{self.folder_name}")
        except FileExistsError:
            print(f"The folder \"{FOLDER_OBJECTS}/{self.folder_name}\" is already exists")

    def update_files(self):
        self.copy_main_files()
        self.remove_unused_files(f"{FOLDER_OBJECTS}/{self.folder_name}", FILE_ADD_DATA)
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
