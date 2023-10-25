import constants as const
import functions as func

import sys
import shutil
from optparse import OptionParser


class ObjectRemover:
    """Add some comments here"""

    def __init__(self, object_folder_path):
        self.object_folder_path = object_folder_path
        self.object_define = self.extract_define()

    def update_file(self, path_to_file):
        old_content = func.get_file_content(path_to_file)[1]
        new_content = ''

        if old_content.find(self.object_define) == -1:
            print(f"The file {path_to_file} is not contain Object that must be removed")
            return

        add_line = True     # the flag indicates whether leave the line
        for line in old_content.split("\n"):
            line_to_check = line.strip().replace(" ", "")

            if line_to_check in [f"#if{self.object_define}", f"#ifdef{self.object_define}"] or line_to_check.find(f"option({self.object_define}") != -1:
                add_line = False

            if add_line:
                new_content += line + "\n"

            else:   # check if all that must be deleted is already deleted:
                if line_to_check[:6] in ["#endif", "endif("]:
                    add_line = True

        func.write_to_file(path_to_file, new_content[:-1])

    def update_files(self):
        if not self.extract_define():
            print("The define of Object must be removed not extracted")
            return False
        for file in [const.FILE_OBJECT_ID, const.FILE_REGISTRY_H, const.FILE_REGISTRY_CPP, const.FILE_CONFIG_CMAKE]:
            self.update_file(file)
        return True

    def extract_define(self):
        errcode, data_str = func.get_file_content(f"{self.object_folder_path}/{const.FILE_OBJ_METADATA}")
        if not errcode:
            return False

        data_dict = eval(data_str)
        dict_obj_meta = data_dict["object_names"]["define"]
        return dict_obj_meta

    def remove_object_folder(self):
        try:
            shutil.rmtree(self.object_folder_path)
        except FileNotFoundError:
            print("There is no folder/files to remove")

    def remove_object(self):
        if not self.update_files():
            return False
        self.remove_object_folder()
        return True


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--folder", dest="folder_path", help="The path to the folder of the Object")
    options, args = parser.parse_args()

    if not options.folder_path:
        parser.error("The path to the folder of the Object is not provided")

    object_remover = ObjectRemover(options.folder_path)
    if not object_remover.remove_object():
        sys.exit(1)
