import os
import xml.etree.ElementTree as ET
from optparse import OptionParser

class ObjectXmlParser:
    """Add some comments here"""

    def create_folder(self, folder_name):
        try:
            os.mkdir(folder_name)
        except FileExistsError:
            pass

    def create_file(self, path, filename, filetype, content):
        f = open(f"{path}/{filename}.{filetype}", "w+")
        f.write(content)
        f.close()

    def parse_xml(self, file_path):
        tree = ET.parse(file_path)
        root = tree.getroot()

        # pack the dictionary of the object:
        object_meta = {"object_name": root[0][0].text,
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

        return object_meta, resources_list
