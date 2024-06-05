#!/usr/bin/env python3

import os
import sys
from optparse import OptionParser

sys.path.insert(1, './scripts')
import scripts.object_generator as OG
import scripts.object_integrator as OI
import scripts.object_changer as OC
import scripts.object_remover as OR
import scripts.object_initializer as OZ

# GENERATOR_CHOICES = ['file', 'link', 'meta']
GENERATOR_CHOICES = ['file', 'link']


def arguments_init(parser):
    parser.add_option("-g", "--generate", dest="generate", type="choice", choices=GENERATOR_CHOICES,
                      help="generate main files of the Object and store in the current folder")

    parser.add_option("-i", "--integrate", dest="integrate", action="store_true", default=False,
                      help="integrate indicated main files of the Object to Wpp")

    parser.add_option("-r", "--remove", dest="remove", action="store_true", default=False,
                      help="remove the exist Object from Wpp")

    parser.add_option("-c", "--change", dest="change", action="store_true", default=False,
                      help="change the exist Object in Wpp")

    parser.add_option("-z", "--initialize", dest="initialize", action="store_true", default=False,
                      help="create and initialize the example of the exist Object")

    options, args = parser.parse_args()
    return options, args


def integrate_object(path_to_folder):
    return OI.ObjectIntegrator(path_to_folder).update_files()


def main():
    parser = OptionParser()
    options, args = arguments_init(parser)
    if (not options.generate
            and not options.integrate
            and not options.remove
            and not options.change
            and not options.initialize):
        parser.error("no options selected. Choose one from: -g, -i, -c, -r, -z.\n\nUse -h for help.")

    # ============================= generation =============================
    if options.generate:
        obj_file = None
        obj_name = None
        obj_ver = None
        obj_link = None

        if options.generate == GENERATOR_CHOICES[0]:  # file
            # print(GENERATOR_CHOICES[0])
            if len(args) != 1:
                parser.error("please, provide the path to the XML-file of the Object description to generate\n"
                             "For example: object_maker.py -g file ./new_object_data.xml")
            obj_file = os.path.normpath(args[0])

        elif options.generate == GENERATOR_CHOICES[1]:  # link
            # print(GENERATOR_CHOICES[1])
            if len(args) != 1:
                parser.error("please, provide the link of the Object to generate\n"
                             "For example: object_maker.py -g link "
                             "https://raw.githubusercontent.com/OpenMobileAlliance/lwm2m-registry/prod/0.xml")
            obj_link = args[0]

        # elif options.generate == GENERATOR_CHOICES[2]:  # meta
        #     # print(GENERATOR_CHOICES[2])
        #     # TODO: implement Object generation by name and version
        #     print("Not implemented yet")
        #     sys.exit(1)
        #     # if len(args) != 2:
        #     #     parser.error("please, provide Object's name and version to generate Object by meta info")
        #     # obj_name = args[0]
        #     # obj_ver = args[1]

        og = OG.ObjectGenerator(obj_file, obj_link)
        og.object_code_generate()

        if options.integrate:
            crated_obj_folder = og.get_folder_path()
            if not integrate_object(crated_obj_folder):
                sys.exit(1)

    # ============================= integration ============================
    elif options.integrate:
        if len(args) != 1:
            parser.error("please, provide the path to the folder contain the Object to integrate\n"
                         "For example: object_maker.py -i ./o_4_connectivity_monitoring_v13")
        if not integrate_object(os.path.normpath(args[0])):
            sys.exit(1)

    # ============================== changing ==============================
    elif options.change:
        if len(args) != 2:
            parser.error("please, provide two arguments:\n"
                         "1 - the path to the folder contain existing Object\n"
                         "2 - the path to the xml file contain the new data of the Object\n"
                         "For example: object_maker.py -c "
                         "D:\\Client\\wpp\\registry\\objects\\o_4_connectivity_monitoring_v13 "
                         "./new_object_data.xml")
        if not OC.ObjectChanger(os.path.normpath(args[0]), args[1]).change():
            sys.exit(1)

    # ============================== removing ==============================
    elif options.remove:
        if len(args) != 1:
            parser.error("please, provide the path to the folder contain existing Object to remove\n"
                         "For example: object_maker.py -r "
                         "D:\\Client\\wpp\\registry\\objects\\o_4_connectivity_monitoring_v13")
        if not OR.ObjectRemover(os.path.normpath(args[0])).remove_object():
            sys.exit(1)

    # ============================== initializing ==============================
    elif options.initialize:
        if len(args) not in [1, 2]:
            parser.error("please, provide at least one (second is optional) argument:\n"
                         "1 - the path to the json-file contain data for creating and initializing\n"
                         "2 - the list of Objects' ID must be initialized (separated by coma without whitespaces)\n"
                         "For example: object_maker.py -z .\\initialize_info.json [1,2,6]")

        json_file = args[0]
        list_of_id = None if len(args) != 2 else args[1]
        if not OZ.ObjectInitializer(os.path.normpath(json_file), list_of_id).initialize():
            sys.exit(1)

    sys.exit(0)


if __name__ == '__main__':
    main()
