import sys
from optparse import OptionParser
sys.path.insert(1, './scripts')
import scripts.object_generator as OG
import scripts.object_integrator as OI
import scripts.object_changer as OC
import scripts.object_remover as OR

GENERATOR_CHOICES = ['file', 'link', 'meta']


def arguments_init(parser):
    parser.add_option("-g", "--generate",
                      type="choice",
                      dest="generate",
                      choices=GENERATOR_CHOICES,
                      help="generate code for Object")

    parser.add_option("-i", "--integrate",
                      dest="integrate",
                      action="store_true",
                      default=False,
                      help="file of the Object")

    parser.add_option("-r", "--remove", dest="remove", help="file of the Object")
    parser.add_option("-c", "--change", dest="change", help="file of the Object")

    options, args = parser.parse_args()
    return options, args


def integrate_object(path_to_folder):
    OI.ObjectIntegrator(path_to_folder).update_files()


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
            if len(args) != 1:
                parser.error("please, provide path to XML-file of the Object")
            obj_file = args[0]
        elif options.generate == GENERATOR_CHOICES[2]:    # meta
            if len(args) != 2:
                parser.error("please, provide Object's name and version to generate Object by meta info")
            # TODO: implement Object generation by name and version
            obj_name = args[0]
            obj_ver = args[1]
        elif options.generate == GENERATOR_CHOICES[1]:    # link
            if len(args) != 1:
                parser.error("please, provide link to Object")
            obj_link = args[1]

        og = OG.ObjectGenerator(obj_file, obj_link)
        og.object_code_generate()

        if options.integrate:
            crated_obj_folder = og.get_folder_path()
            integrate_object(crated_obj_folder)
        return

    # ============================== integration ==============================
    if options.integrate:
        if len(args) != 1:
            parser.error("please, provide path to folder of the Object")
        integrate_object(args[0])
        return

    # ============================== changing ==============================
    if options.change:
        if len(args) != 2:
            parser.error("please, provide the folder's path of the exists Object and path to the new data")
        # TODO: implement it
        return

    # ============================== removing ==============================
    if options.remove:
        if len(args) != 1:
            parser.error("please, provide path to folder of the Object")
        OR.ObjectRemover(options.remove).remove_object()
        return


if __name__ == '__main__':
    main()
