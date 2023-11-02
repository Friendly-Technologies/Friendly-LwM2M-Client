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
FILE_CONFIG_CMAKE = f"{FOLDER_WPP}/config/objects_config.cmake"

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

STOP_STRING_CNFG_CMK = ["# ---------- Mandatory objects config blok end ----------",
                        "# ---------- Optional objects config blok end ----------"]
STOP_STRING_OBJ_ID = ["/* ---------- Mandatory objects ID blok end ---------- */",
                      "/* ---------- Optional objects ID blok end ---------- */"]
STOP_STRING_REG_CPP_INIT = ["/* ---------- Mandatory objects init blok end ---------- */",
                            "/* ---------- Optional objects init blok end ---------- */"]
STOP_STRING_REG_CPP_METHOD = ["/* ---------- Mandatory objects method blok end ---------- */",
                              "/* ---------- Optional objects method blok end ---------- */"]
STOP_STRING_REG_PRT = ["/* ---------- Mandatory objects prototype blok end ---------- */",
                       "/* ---------- Optional objects prototype blok end ---------- */"]
STOP_STRING_REG_INCL = ["/* ---------- Mandatory objects include blok end ---------- */",
                        "/* ---------- Optional objects include blok end ---------- */"]

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
TYPE_OBJECT = "ObjectImpl"

KEYS_OBJ_DATA = {"name": "Name",
                 "description": "Description1",
                 "id": "ObjectID",
                 "urn": "ObjectURN",
                 "lwm2m_version": "LWM2MVersion",
                 "version": "ObjectVersion",
                 "is_multiple": "MultipleInstances",
                 "is_mandatory": "Mandatory",
                 "operations": "Operations",
                 "type": "Type",
                 "res_id": "ID",
                 "res_define": "Define",
                 }

LWM2M_WEB_RESOURCES = ["raw.githubusercontent.com", "github.com"]

OPTIONS_MANDATORY = ["Mandatory", "Optional"]
OPTIONS_VARIETY = ["Multiple", "Single"]
