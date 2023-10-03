import object_remover

from optparse import OptionParser
import re


def get_info(path_to_file):
    add_line = False
    counter = 0
    user_code_block = ''
    user_code_blocks = {}
    with open(path_to_file, 'r') as f:
        for line in f:
            if re.search("block \d start", line):
                add_line = True
                continue

            if re.search("block \d end", line):
                add_line = False
                user_code_blocks[counter] = user_code_block
                counter += 1

            if add_line:
                user_code_block += line + "\n"
    f.close()

    return user_code_blocks


# path_to_object = f"../wpp/registry/objects/mandatory/lwm2m_server_1_1"
# obj_rem = object_remover.ObjectRemover(path_to_object)
