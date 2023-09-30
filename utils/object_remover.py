# def update_file(content, path_to_file):
#     is_stop_string_present = False
#     new_content = ''
#     old_content = ''
#     with open(path_to_file, 'r') as f:
#         for i in f:
#             old_content += i
#     f.close()
#
#     if old_content.find(content) != -1:
#         # print(f"File {path_to_file} is already updated")
#         return
#
#     for line in old_content.split("\n"):
#         if " ".join(line.split()) == stop_string:
#             is_stop_string_present = True
#             new_content += content
#         new_content += line + "\n"
#
#     if not is_stop_string_present:
#         # print(f"The {path_to_file} was not updated!")
#         return
#
#     with open(path_to_file, 'w') as f:
#         f.write(new_content[:-1])
#     f.close()

def generate_define(path):
    folders = path.split("/")
    print(folders)


path_to_object = f"../wpp/registry/objects/mandatory/lwm2m_server_1_1"
generate_define(path_to_object)
