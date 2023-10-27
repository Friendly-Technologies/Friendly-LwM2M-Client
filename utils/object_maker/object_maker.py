import sys
from optparse import OptionParser
sys.path.insert(1, './scripts')
import scripts.object_generator as OG
import scripts.object_integrator as OI
import scripts.object_changer as OC
import scripts.object_remover as OR

GENERATOR_CHOICES = ['file', 'link', 'meta']


def arguments_init(parser):
    parser.add_option("-g", "--generate", dest="generate", type="choice", choices=GENERATOR_CHOICES,
                      help="generate main files of the Object and store in the current folder")

    parser.add_option("-i", "--integrate", dest="integrate", action="store_true", default=False,
                      help="integrate indicated main files of the Object to Wpp")

    parser.add_option("-r", "--remove", dest="remove", action="store_true", default=False,
                      help="remove the exist Object from Wpp")

    parser.add_option("-c", "--change", dest="change", action="store_true", default=False,
                      help="change the exist Object in Wpp")

    options, args = parser.parse_args()
    return options, args


def integrate_object(path_to_folder):
    return OI.ObjectIntegrator(path_to_folder).update_files()


def main():
    parser = OptionParser()
    options, args = arguments_init(parser)
    if not options.generate and not options.integrate and not options.remove and not options.change:
        parser.error("no options selected")

    # ============================== generation ==============================
    if options.generate:
        obj_file = None
        obj_name = None
        obj_ver = None
        obj_link = None

        if options.generate == GENERATOR_CHOICES[0]:    # file
            # print(GENERATOR_CHOICES[0])
            if len(args) != 1:
                parser.error("please, provide the XML-file's path of the Object to generate")
            obj_file = args[0]

        elif options.generate == GENERATOR_CHOICES[1]:    # link
            # print(GENERATOR_CHOICES[1])
            # TODO: implement Object generation by link
            # print("Not implemented yet")
            # sys.exit(1)
            if len(args) != 1:
                parser.error("please, provide the link of the Object to generate")
            obj_link = args[0]

        elif options.generate == GENERATOR_CHOICES[2]:    # meta
            # print(GENERATOR_CHOICES[2])
            # TODO: implement Object generation by name and version
            print("Not implemented yet")
            sys.exit(1)
            # if len(args) != 2:
            #     parser.error("please, provide Object's name and version to generate Object by meta info")
            # obj_name = args[0]
            # obj_ver = args[1]

        og = OG.ObjectGenerator(obj_file, obj_link)
        og.object_code_generate()

        if options.integrate:
            crated_obj_folder = og.get_folder_path()
            if not integrate_object(crated_obj_folder):
                sys.exit(1)

    # ============================== integration ==============================
    elif options.integrate:
        if len(args) != 1:
            parser.error("please, provide the folder's path of the Object to integrate")
        if not integrate_object(args[0]):
            sys.exit(1)

    # ============================== changing ==============================
    elif options.change:
        if len(args) != 2:
            parser.error("please, provide the folder's path to the existing Object and path to the new Object's data")
        if not OC.ObjectChanger(args[0], args[1]).change():
            sys.exit(1)

    # ============================== removing ==============================
    elif options.remove:
        if len(args) != 1:
            parser.error("please, provide the folder's path to the existing Object to remove")
        if not OR.ObjectRemover(args[0]).remove_object():
            sys.exit(1)

    sys.exit(0)


if __name__ == '__main__':
    main()
