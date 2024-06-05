# This file is used to build wakaama core library as part of other project
# It is not used to build wakaama as standalone library, compilation
# options and configs must be set in parent project

set(WAKAAMA_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../2305-Wakaama")

function(target_link_wakaama target)
    # Include file with wakaama sources config
    include(${WAKAAMA_DIRECTORY}/wakaama.cmake)
    
    # Include wakaama core sources
    target_sources_wakaama(${target})
endfunction()
