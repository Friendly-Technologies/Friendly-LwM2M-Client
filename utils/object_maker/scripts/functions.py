import os
import json
import shutil

TAG = "functions"


def get_content_from_file(file_path):
    try:
        with open(file_path, 'r') as file:
            return True, file.read()
    except FileNotFoundError:
        LOG(TAG, get_content_from_file.__name__, f'the file "{file_path}" not found.')
        return False, ""


def get_json_from_file(json_file_path):
    func_name = get_json_from_file.__name__
    errcode, data_str = get_content_from_file(json_file_path)
    if not errcode:
        LOG(TAG, func_name, f'unable to get content of the "{json_file_path}" file.')
        return False, ""
    try:
        data_dict = json.loads(data_str)
    except json.decoder.JSONDecodeError as e:
        LOG(TAG, func_name, f'unable to parse the content from "{json_file_path}" file ({e}).')
        return False, ""
    return True, data_dict


def get_file_content_arr(file_path):
    try:
        with open(file_path) as file:
            return True, [line.rstrip() for line in file]
    except FileNotFoundError:
        LOG(TAG, get_file_content_arr.__name__, f'No such file "{file_path}"')
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


def create_file(filename, content):
    if content is None:
        LOG(__name__, create_file.__name__, "content must be str, not None")
        return False
    with open(filename, 'w+') as file:
        file.write(content)
    file.close()
    return True


def create_folder(folder_name):
    try:
        os.mkdir(folder_name)
    except FileExistsError:
        # LOG(__name__, create_folder.__name__, "the folder already exists")
        return False
    return True


def remove_folder(folder_path):
    try:
        shutil.rmtree(folder_path)
    except FileNotFoundError:
        # print("There is no folder/files to remove")
        pass

def copy_file(src, dst):
    try:
        shutil.copy(src, dst)
    except FileNotFoundError:
        LOG(__name__, copy_file.__name__, f"the file {src} not found")
        return False
    return True

def copy_folder(src, dst):
    try:
        shutil.copytree(src, dst)
    except FileNotFoundError:
        LOG(__name__, copy_folder.__name__, f"the folder {src} not found")
        return False
    return True

def LOG(tag, func, text):
    print(f"[{tag}::{func}]: {text}")
