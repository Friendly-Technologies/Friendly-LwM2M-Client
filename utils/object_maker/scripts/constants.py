#
FOLDER_TEMPLATES = "./file_templates"
FOLDER_WPP = "../../wpp"
FOLDER_REGISTRY = f"{FOLDER_WPP}/registry"
FOLDER_OBJECTS = f"{FOLDER_WPP}/registry/objects"
FOLDER_CONFIG = f"{FOLDER_WPP}/config"

FILE_OBJ_METADATA = "object_metadata.json"

FILE_OBJECT_ID = f"{FOLDER_WPP}/registry/ObjectID.h"
FILE_REGISTRY_H = f"{FOLDER_WPP}/registry/WppRegistry.h"
FILE_REGISTRY_CPP = f"{FOLDER_WPP}/registry/WppRegistry.cpp"
FILE_CONFIG_CMAKE = f"{FOLDER_WPP}/config/config.cmake"

FILE_TMPLT_IMPL_H = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_H.txt"
FILE_TMPLT_IMPL_CPP = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_CPP.txt"
FILE_TMPLT_INFO = f"{FOLDER_TEMPLATES}/FILE_OBJ_INFO.txt"
FILE_TMPLT_CONFIG = f"{FOLDER_TEMPLATES}/FILE_OBJ_CONFIG.txt"
FILE_TMPLT_CMAKE = f"{FOLDER_TEMPLATES}/FILE_OBJ_CMAKE.txt"

KEY_FILE_IMPL_H = "impl_h"
KEY_FILE_IMPL_CPP = "impl_cpp"
KEY_FILE_IMPL_CONFIG = "config"
KEY_FILE_IMPL_INFO = "info"
KEY_FILE_IMPL_CMAKE = "cmake"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]

KEY_DICT_OBJ_META = "object_data"
KEY_DICT_OBJ_NAMES = "object_names"
KEY_DICT_OBJ_FILES = "object_files"

KEY_DICT_OBJ_NAMES_CLASS = "class"
KEY_DICT_OBJ_NAMES_CAMELC = "camelcase"
KEY_DICT_OBJ_NAMES_DEFINE = "define"
KEY_DICT_OBJ_NAMES_UNDERL = "up_underline"

KEY_DICT_OBJ_META_MANDAT = "is_mandatory"
KEY_DICT_OBJ_META_NAME = "name"
KEY_DICT_OBJ_META_ID = "id"
KEY_DICT_OBJ_META_VER_LWM2M = "lwm2m_version"
KEY_DICT_OBJ_META_VER = "version"

TYPE_OPERATION = "ResOp"
TYPE_OBJECT = "Object"
