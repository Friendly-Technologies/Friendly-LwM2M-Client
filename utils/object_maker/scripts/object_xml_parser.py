import functions as func
import constants as const

import json
import requests
import xml.etree.ElementTree as ElementTree


class ObjectXmlParser:
    """ Class get the xml-file (path) that contain LwM2M Object description,
    parse that info and pack it to 2 data structures:
    1. The dictionary that contain the data of the Object
    2. The list of the dictionaries each of which contain the data of the Resource
    Add some comments here.
    Also Class generated the dictionary that contain the different names and defines
    that will be useful at code generation to integrate Object to Wpp project structure.
    """

    def __init__(self, xml_file=None, xml_url=None):
        self.log_tag = f"[{self.__class__.__name__}]:"
        self.xml_file = xml_file
        self.xml_url = xml_url
        self.object_data = None
        self.resources_data = None

        if xml_file and xml_url:
            print(self.__class__.__name__, "only one source should be provided to create the the Object (file or url")

        self.set_xml_file()
        self.set_parsed_data()  # sets self.object_data and self.resources_data

    def set_xml_file(self):
        if self.xml_file:
            return
        self.xml_file = self.download_xml_file()

    def download_xml_file(self):
        """Downloads data from provided link, extract xml-representation of the LwM2M Object and save path to field"""
        filename = self.xml_url.split("/")[-1]
        web_page = self.xml_url.split("/")[2]

        raw_data = requests.get(self.xml_url).content.decode('utf-8')
        if web_page == const.LWM2M_WEB_RESOUCES[0]:                     # raw.githubusercontent.com
            func.write_to_file(f"./{filename}", raw_data)
            return filename

        if web_page == const.LWM2M_WEB_RESOUCES[1]:                     # github.com
            json_data = json.loads(raw_data)
            object_description = json_data["payload"]["blob"]["rawLines"]
            func.write_to_file_line_by_line(f"./{filename}", object_description)
        return filename

    def set_parsed_data(self):
        # ============================= parse the xml data ==============================
        tree = ElementTree.parse(self.xml_file)
        root = tree.getroot()
        # ========================= extract data of the Object ==========================
        object_data = {}
        for key in const.KEYS_OBJ_DATA.values():
            for i in root[0]:
                if i.tag == key:
                    object_data[key] = i.text
        if (const.KEYS_OBJ_DATA["lwm2m_version"] not in object_data.keys() or       # TODO: get consultation what to do if True
                const.KEYS_OBJ_DATA["version"] not in object_data.keys()):
            object_data[const.KEYS_OBJ_DATA["lwm2m_version"]] = "0"
            object_data[const.KEYS_OBJ_DATA["version"]] = "0"
        self.object_data = object_data
        # ===============================================================================
        # ====== extract data of each of the resources, and pack it to dictionary =======
        resources_data = []
        for resources in root.findall('./Object/Resources/Item'):
            # get already existing dictionary (with ID) and fill it by another data in loop:
            resource_dict = resources.attrib
            repl_characters = [' ', '-', '\\', '/', '(', ')', '.', ',']  # TODO: the "(s)" postfix changes to "_S_"
            for resource in resources:
                resource_name = ' '.join(resource.text.split()) if resource.text else "none"
                for character in repl_characters:
                    resource_name = resource_name.replace(character, '_')
                resource_dict[resource.tag] = resource_name.upper()
                # print(resource_name.upper())
            # generate define of the resource:
            name_res = resource_dict['Name']
            id_obj = object_data[const.KEYS_OBJ_DATA['id']]
            id_res = resource_dict['ID']
            resource_dict['Define'] = f"RES_{id_obj}_{id_res}"
            # ============ add prepared resource's dictionaries to the list =============
            resources_data.append(resource_dict)
            # ===========================================================================
        self.resources_data = resources_data
        # ===============================================================================
