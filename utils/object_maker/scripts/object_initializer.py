import os
import json
import sys

import functions as func
import constants as const


class ObjectInitializer:
    def __init__(self, path_json_file):
        self.objects = None
        self.register_data = None
        self.log_tag = self.__class__.__name__
        self.path_json_file = path_json_file

    def get_init_data(self):
        """
        Reads data from provided json file.
        """
        errcode, data_str = func.get_file_content(self.path_json_file)
        if not errcode:
            print(f'{self.log_tag} There is no file with the metadata of the Object'
                  f'"{self.path_json_file}". Operation interrupted.')
            return False
        try:
            data_dict = json.loads(data_str)
        except json.decoder.JSONDecodeError:
            print(f'{self.log_tag} Unable to pars provided json-file. Operation interrupted.')
            return False
        self.objects = [obj for obj in data_dict["objects"] if obj != {}]
        return True

    def initialize(self):
        """
        Returns True if initialized example is created successful.
        """
        if not self.get_init_data():
            return False
        print(len(self.objects))
        return True
