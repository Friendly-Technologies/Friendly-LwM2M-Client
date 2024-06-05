#
FOLDER_TEMPLATES = "./file_templates"
FOLDER_WPP = "../../wpp"
FOLDER_REGISTRY = f"{FOLDER_WPP}/registry"
FOLDER_OBJECTS = f"{FOLDER_WPP}/registry/objects"
FOLDER_CONFIG = f"{FOLDER_WPP}/configs"

FILE_OBJ_METADATA = "object_metadata.json"
FILE_CMAKE_LISTS = "CMakeLists.txt"

FILE_OBJECT_ID = f"{FOLDER_REGISTRY}/ObjectID.h"
FILE_REGISTRY_H = f"{FOLDER_REGISTRY}/WppRegistry.h"
FILE_REGISTRY_CPP = f"{FOLDER_REGISTRY}/WppRegistry.cpp"
FILE_CONFIG_CMAKE = f"{FOLDER_CONFIG}/wpp_config.cmake"

FILE_TMPLT_IMPL_H = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_H.txt"
FILE_TMPLT_IMPL_CPP = f"{FOLDER_TEMPLATES}/FILE_OBJ_IMPL_CPP.txt"
FILE_TMPLT_INFO = f"{FOLDER_TEMPLATES}/FILE_OBJ_INFO.txt"
FILE_TMPLT_CONFIG = f"{FOLDER_TEMPLATES}/FILE_OBJ_CONFIG.txt"
FILE_TMPLT_CMAKE = f"{FOLDER_TEMPLATES}/FILE_OBJ_CMAKE.txt"

FILE_TMPLT_INIT_CPP = f"{FOLDER_TEMPLATES}/FILE_INIT_CPP.txt"
FILE_TMPLT_INIT_H = f"{FOLDER_TEMPLATES}/FILE_INIT_H.txt"
FILE_TMPLT_INIT_CMAKE = f"{FOLDER_TEMPLATES}/FILE_INIT_CMAKE.txt"

KEY_FILE_IMPL_H = "impl_h"
KEY_FILE_IMPL_CPP = "impl_cpp"
KEY_FILE_IMPL_CONFIG = "config"
KEY_FILE_IMPL_INFO = "info"
KEY_FILE_IMPL_CMAKE = "cmake"

STOP_STRING_CNFG_CMK = ["# ---------- Mandatory objects config block end ----------",
                        "# ---------- Optional objects config block end ----------"]
STOP_STRING_OBJ_ID = ["/* ---------- Mandatory objects ID block end ---------- */",
                      "/* ---------- Optional objects ID block end ---------- */"]
STOP_STRING_REG_CPP_INIT = ["/* ---------- Mandatory objects init block end ---------- */",
                            "/* ---------- Optional objects init block end ---------- */"]
STOP_STRING_REG_CPP_METHOD = ["/* ---------- Mandatory objects method block end ---------- */",
                              "/* ---------- Optional objects method block end ---------- */"]
STOP_STRING_REG_PRT = ["/* ---------- Mandatory objects prototype block end ---------- */",
                       "/* ---------- Optional objects prototype block end ---------- */"]
STOP_STRING_REG_INCL = ["/* ---------- Mandatory objects include block end ---------- */",
                        "/* ---------- Optional objects include block end ---------- */"]

KEY_DICT_OBJ_META = "object_data"
KEY_DICT_OBJ_NAMES = "object_names"
KEY_DICT_OBJ_FILES = "object_files"

KEY_NAME_CLASS = "class"
KEY_NAME_CAMELCASE = "camelcase"
KEY_NAME_DEFINE = "define"
KEY_NAME_UNDERLINE = "up_underline"
KEY_PATH_TO_FOLDER = "name_path_to_folder"
KEY_NAME_OF_FOLDER = "name_of_folder"

KEY_NAME = "name"
KEY_TYPE = "type"
KEY_VALUE = "value"
KEY_ID = "id"
KEY_ID_OBJ = "obj_id"
KEY_ID_RES = "res_id"

KEY_URN = "urn"
KEY_VER = "version"
KEY_VER_LWM2M = "lwm2m_version"
KEY_DESCRIPTION = "description"
KEY_IS_MULTIPLE = "is_multiple"
KEY_IS_MANDATORY = "is_mandatory"
KEY_OPERATIONS = "operations"
KEY_DEFINE_RES = "res_define"

TYPE_OPERATION = "ItemOp"
TYPE_OBJECT = "Object"
TYPE_OBJECT_IMPL = "ObjectImpl"
TYPE_ID = "TYPE_ID"

# there is a map between terms used in program, and terms used in LwM2M below:
DATA_KEYS = {KEY_NAME:          "Name",
             KEY_DESCRIPTION:   "Description1",
             KEY_ID_OBJ:        "ObjectID",
             KEY_URN:           "ObjectURN",
             KEY_VER_LWM2M:     "LWM2MVersion",
             KEY_VER:           "ObjectVersion",
             KEY_IS_MULTIPLE:   "MultipleInstances",
             KEY_IS_MANDATORY:  "Mandatory",
             KEY_OPERATIONS:    "Operations",
             KEY_TYPE:          "Type",
             KEY_ID_RES:        "ID",
             KEY_DEFINE_RES:    "Define",
             }

WPP_TYPES = ["INT", "UINT", "BOOL", "STRING", "EXECUTE", "OPAQUE", "FLOAT", "OBJ_LINK", "TIME", "CORE_LINK", "NONE"]

LWM2M_WEB_RESOURCES = ["raw.githubusercontent.com", "github.com"]

OPTIONS_MANDATORY = ["Mandatory", "Optional"]
OPTIONS_VARIETY = ["Multiple", "Single"]

ENUM_FIELD_PATTERN = r" = \d+"
ENUM_START_PATTERN = "enum ID: ID_T {"
ENUM_END_PATTERN = "};"
ENUM_FIELD_PARTS_CNT = 2

USER_CODE_BLOCK_START = r'block \d+ start'
USER_CODE_BLOCK_END = r'block \d+ end'
