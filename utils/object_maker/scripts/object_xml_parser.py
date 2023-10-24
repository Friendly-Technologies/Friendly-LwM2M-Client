import functions as func

import json
import string
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
        self.xml_file_path = self.download_xml(xml_url) if xml_url is not None else xml_file

    def download_xml(self, xml_url):
        filename = xml_url.split("/")[-1]
        object_description = json.loads(requests.get(xml_url).content.decode('utf-8'))["payload"]["blob"]["rawLines"]
        func.write_to_file_line_by_line(f"./{filename}", object_description)
        return filename

    def parse_xml(self):
        tree = ElementTree.parse(self.xml_file_path)
        root = tree.getroot()

        # pack the dictionary of the object:
        object_data = {"object_name": root[0][0].text,
                       "object_description": root[0][1].text,
                       "object_id": root[0][2].text,
                       "object_urn": root[0][3].text,
                       "object_lwm2m_version": root[0][4].text,
                       "object_version": root[0][5].text,
                       "is_multiple": root[0][6].text == "Multiple",
                       "is_mandatory": root[0][7].text == "Mandatory",
                       }
        # pack the list of the dictionary of the resources:
        resources_list = []
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
            id_obj = object_data['object_id']
            id_res = resource_dict['ID']
            resource_dict['Define'] = f"RES_{id_obj}_{id_res}"
            # add prepared resource dictionary to the list:
            resources_list.append(resource_dict)

        return object_data, resources_list

    def create_metadata(self):
        object_data, resources_data = self.parse_xml()

        object_metadata = {}

        obj_name_plain = object_data["object_name"]                                 # 'LWM2M Server'
        obj_name_lower = obj_name_plain.lower()                                     # 'lwm2m server'
        obj_name_capwords = string.capwords(obj_name_lower)                         # 'Lwm2m Server'
        obj_name_class = obj_name_capwords.replace(' ', '')                         # 'Lwm2mServer'
        obj_name_camelcase = obj_name_class[0].lower() + obj_name_class[1:]         # 'lwm2mServer'
        obj_name_underline = obj_name_plain.replace(' ', '_')                       # 'LWM2M_Server'
        obj_name_underline_up = obj_name_underline.upper()                          # 'LWM2M_SERVER'
        obj_name_underline_lw = obj_name_underline.lower()                          # 'lwm2m_server'
        obj_requirement_short = "M" if object_data["is_mandatory"] else "O"         # 'M' | 'O'
        obj_version = object_data["object_version"].replace(".", "")                # 13
        obj_id = object_data['object_id']                                           # 1
        obj_name_folder = f"{obj_requirement_short.lower()}_" \
                          f"{obj_id}_" \
                          f"{obj_name_underline_lw}_"\
                          f"v{obj_version}"                                         # 'm_1_lwm2m_server_v13'
        obj_name_path_to_folder = f"../../wpp/registry/objects/"                    # '../../wpp/registry/objects/'
        obj_name_define = f"OBJ_{obj_requirement_short}_" \
                          f"{obj_id}_" \
                          f"{obj_name_underline_up}_" \
                          f"V{obj_version}"                                         # 'OBJ_M_1_LWM2M_SERVER_V13'

        object_metadata["obj_name_class"] = obj_name_class                          # 'Lwm2mServer'
        object_metadata["obj_name_camelcase"] = obj_name_camelcase                  # 'lwm2mServer'
        object_metadata["obj_name_define"] = obj_name_define                        # 'OBJ_M_1_LWM2M_SERVER_V13'
        object_metadata["obj_name_folder"] = obj_name_folder                        # 'm_1_lwm2m_server_v13'
        object_metadata["obj_name_path_to_folder"] = obj_name_path_to_folder        # '../../wpp/registry/objects/'
        object_metadata["obj_name_up_underline"] = obj_name_underline_up            # 'LWM2M_SERVER'

        return object_metadata
