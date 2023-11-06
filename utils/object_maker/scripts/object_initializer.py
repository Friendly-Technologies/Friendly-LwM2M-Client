import functions as func

import json


class ObjectInitializer:
    def __init__(self, path_json_file):
        self.log_tag = f"[{self.__class__.__name__}]:"
        self.path_json_file = path_json_file
        # self.path_registers_folder = path_registers_folder

    def get_init_data(self):
        """
        Reads data from provided json file.
        """
        errcode, data_str = func.get_file_content(self.path_json_file)
        if not errcode:
            print(f'{self.log_tag} There is no file with the metadata of the Object'
                  f'"{self.path_json_file}". Operation interrupted.')
            return False
        # try:
        data_dict = json.loads(data_str)
        # except json.decoder.JSONDecodeError:
        #     print(f'{self.log_tag} Unable to pars provided json-file. Operation interrupted.')
        #     return False
        print(data_dict[])
        return True

    def initialize(self):
        """
        Returns True if initialized example is created successful.
        """
        return self.get_init_data()
        # if error:
        #     return False
