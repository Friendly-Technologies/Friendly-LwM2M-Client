import shutil

files = ["../wpp/registry/ObjectID.h",
         "../wpp/config/config.cmake",
         "../wpp/registry/WppRegistry.h",
         "../wpp/registry/WppRegistry.cpp"]


class ObjectRemover:
    """Add some comments here"""

    def __init__(self, object_folder_path):
        self.object_folder_path = object_folder_path
        self.object_define = self.extract_define()

    def update_file(self, path_to_file):
        # print(path_to_file)
        old_content = ''
        new_content = ''
        with open(path_to_file, 'r') as f:
            for i in f:
                old_content += i
        f.close()

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

        with open(path_to_file, 'w') as f:
            f.write(new_content[:-1])
        f.close()

    def update_files(self):
        for file in files:
            self.update_file(file)

    def extract_define(self):
        define = ""
        try:
            with open(f"{self.object_folder_path}/CMakeLists.txt", 'r') as f:
                for line in f:
                    line = line.replace(" ", "")
                    if line[:3] == "if(":
                        define = line[3:-2]  # remove "if(" and ")" symbols
            f.close()
        except FileNotFoundError:
            print("There is no files to extract the define with")

        # print(define)
        return define

    def remove_object_folder(self):
        try:
            shutil.rmtree(self.object_folder_path)
        except FileNotFoundError:
            print("There is no folder/files to remove")

    def remove_object(self):
        self.update_files()
        self.remove_object_folder()


path_to_object = f"../wpp/registry/objects/mandatory/lwm2m_server_1_1"      # TODO: must be unhardcoded!
object_remover = ObjectRemover(path_to_object)
object_remover.remove_object()
