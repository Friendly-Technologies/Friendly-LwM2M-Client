from optparse import OptionParser

xp = object_xml_parser.ObjectXmlParser("./1-1_1.xml")
obj_dict, res_list = xp.parse_xml()
obj_names = xp.create_metadata()
og = ObjectGenerator(obj_dict, res_list, obj_names)
og.object_code_generate()
