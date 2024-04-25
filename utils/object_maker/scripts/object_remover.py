import constants as const
import functions as func

import sys
from optparse import OptionParser


class ObjectRemover:
    """Add some comments here"""

    def __init__(self, object_folder_path):
        self.log_tag = self.__class__.__name__
        self.object_folder_path = object_folder_path
        self.object_define = self.extract_define()

    def update_file(self, path_to_file):
        old_content = func.get_content_from_file(path_to_file)[1]
        new_content = []

        if old_content.find(self.object_define) == -1:
            func.LOG(self.log_tag, self.update_file.__name__,
                     f'the file "{path_to_file}" is not contain the Object\'s info that must be removed')
            return

        add_line = True     # the flag indicates whether leave the line
        for line in old_content.split("\n"):
            line_to_check = line.strip().replace(" ", "")

            if line_to_check in [f"#if{self.object_define}", f"#ifdef{self.object_define}"]:
                add_line = False

            # Different actions for cmake files
            if path_to_file == const.FILE_CONFIG_CMAKE and line.find(f"set(WPP_DEFINITIONS ${{WPP_DEFINITIONS}} {self.object_define})") != -1:
                if new_content[-1].find("# Include") != -1:
                    new_content.pop()
                add_line = False
            
            if add_line:
                new_content.append(line + "\n")
            else:   # check if all that must be deleted is already deleted:
                if line_to_check[:6] in ["#endif", "endif("] or path_to_file == const.FILE_CONFIG_CMAKE:
                    add_line = True

        # remove the last '\n' if it is present
        if new_content and new_content[-1] and new_content[-1][-1] == "\n":
            new_content[-1] = new_content[-1][:-1]

        func.write_to_file(path_to_file, ''.join(new_content))

    def update_files(self):
        if not self.extract_define():
            func.LOG(self.log_tag, self.update_files.__name__, "the define of the deleted Object not extracted")
            return False
        for file in [const.FILE_OBJECT_ID, const.FILE_REGISTRY_H, const.FILE_REGISTRY_CPP, const.FILE_CONFIG_CMAKE]:
            self.update_file(file)
        return True

    def extract_define(self):
        json_file = f"{self.object_folder_path}/{const.FILE_OBJ_METADATA}"
        errcode, data_dict = func.get_json_from_file(json_file)
        if not errcode:
            return False
        dict_obj_meta = data_dict[const.KEY_DICT_OBJ_NAMES][const.KEY_NAME_DEFINE]
        return dict_obj_meta

    def remove_object(self):
        if not self.update_files():
            func.LOG(self.log_tag, self.remove_object.__name__, "unable to get defines. Operation interrupted.")
            return False
        func.remove_folder(self.object_folder_path)
        func.LOG(self.log_tag, self.remove_object.__name__, "the Object removed successfully.")
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
