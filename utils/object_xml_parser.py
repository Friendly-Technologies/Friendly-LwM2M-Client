import xml.etree.ElementTree as ElementTree
import requests
import json


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
        with open(f'./{filename}', 'w') as f:
            for i in object_description:
                f.write(i + "\n")
        f.close()
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
            resources_list.append(resource_dict)

        return object_data, resources_list

    def create_metadata(self):
        object_data, resources_data = self.parse_xml()

        object_metadata = {}

        obj_name_plain = object_data["object_name"]                                         # LwM2M Server
        _obj_name_plain_no_space = obj_name_plain.replace(' ', '')             # LwM2MServer
        _obj_name_plain_underline = obj_name_plain.replace(' ', '_')           # LwM2M_Server
        _obj_name_plain_list = obj_name_plain.split(" ")
        _obj_name_plain_list[0] = _obj_name_plain_list[0][0].lower() + _obj_name_plain_list[0][1:]
        obj_name_camelcase = ''.join(_obj_name_plain_list)                                  # lwM2MServer
        obj_name_class = f"Wpp{_obj_name_plain_no_space}"                                   # WppLwM2MServer
        _obj_requirement = "mandatory" if object_data["is_mandatory"] else "optional"
        _obj_version = object_data["object_lwm2m_version"].replace(".", "_")
        obj_name_folder = f"{_obj_name_plain_underline.lower()}_{_obj_version}"             # lwm2m_server_1_1
        _path_to_folder = f"{_obj_requirement}/{obj_name_folder}"                           # mandatory/lwm2m_server_1_1
        obj_name_path_to_folder = \
            f"../wpp/registry/objects/{_path_to_folder}"            # ../wpp/registry/objects/mandatory/lwm2m_server_1_1

        obj_name_object_info = f"WPP_{_obj_name_plain_underline}_OBJ_INFO".upper()          # WPP_LWM2M_SERVER_OBJ_INFO
        obj_name_define = \
            f"{_obj_requirement}_{_obj_name_plain_underline}_{_obj_version}_OBJ".upper()    # MANDATORY_LWM2M_SERVER_1_1_OBJ

        object_metadata["obj_name"] = obj_name_plain
        object_metadata["obj_name_camelcase"] = obj_name_camelcase
        object_metadata["obj_name_class"] = obj_name_class
        object_metadata["obj_name_folder"] = obj_name_folder
        object_metadata["obj_name_path_to_folder"] = obj_name_path_to_folder
        object_metadata["obj_name_define"] = obj_name_define
        object_metadata["obj_name_object_info"] = obj_name_object_info

        return object_metadata
