import os
import sys
import shutil


def get_file_content(file_path):
    try:
        with open(file_path, 'r') as file:
            return True, file.read()
    except FileNotFoundError:
        print(f'No such file "{file_path}"')
        return False, ""


def write_to_file(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)
    file.close()


def write_to_file_line_by_line(file_path, content):
    with open(file_path, 'w') as file:
        for line in content:
            file.write(line + '\n')
    file.close()


def create_file(filename, filetype, content):
    file = filename if filetype == "" else f"{filename}.{filetype}"
    if content is None:
        LOG(__name__, create_file.__name__, "content must be str, not None")
        return False
    with open(file, 'w+') as file:
        file.write(content)
    file.close()
    return True

def create_folder(folder_name):
    try:
        os.mkdir(folder_name)
    except FileExistsError:
        LOG(__name__, create_folder.__name__, "the folder already exists")


def remove_folder(folder_path):
    try:
        shutil.rmtree(folder_path)
    except FileNotFoundError:
        # print("There is no folder/files to remove")
        pass


def LOG(tag, func, text):
    print(f"[{tag}:{func}()]: {text}")
