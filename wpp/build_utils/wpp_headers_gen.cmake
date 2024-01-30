#                                       
# Script for copying all headers from WPP and Wakaama in specific order
# Creating folder headers in _build/wpp/
#

file(COPY ../wpp/ DESTINATION ../../temporary_f/ )
file(COPY ../temporary_f/registry/objects/ DESTINATION headers/ )

file(REMOVE_RECURSE ../temporary_f/registry/objects)

file(GLOB_RECURSE HEADERS ../temporary_f/*.h)
file(COPY ${HEADERS} DESTINATION headers/ )

file(REMOVE_RECURSE ../temporary_f)

file(COPY ../2305-Wakaama/include/ DESTINATION headers/ )

add_custom_command(
    TARGET Wpp POST_BUILD
    COMMAND find headers -type f ! -name "*.h" -exec rm {} +
)
