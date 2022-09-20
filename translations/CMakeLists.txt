# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

include(CMakeParseArguments)

set(TS_FILES libfuoten.ts
             libfuoten_da.ts
             libfuoten_de.ts
             libfuoten_el.ts
             libfuoten_en_GB.ts
             libfuoten_en_US.ts
             libfuoten_fr.ts
             libfuoten_it.ts
             libfuoten_nl.ts
             libfuoten_sv.ts
             libfuoten_zh_CN.ts)

function(fuoten_add_trans_func _qm_files)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_LRELEASE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lrelease_files ${_LRELEASE_UNPARSED_ARGUMENTS})

    foreach(_current_FILE ${_lrelease_files})
        get_filename_component(_abs_FILE ${_current_FILE} ABSOLUTE)
        get_filename_component(qm ${_abs_FILE} NAME)
        # everything before the last dot has to be considered the file name (including other dots)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${qm})
        get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)
        if(output_location)
            file(MAKE_DIRECTORY "${output_location}")
            set(qm "${output_location}/${FILE_NAME}.qm")
        else()
            set(qm "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.qm")
        endif()

        add_custom_command(OUTPUT ${qm}
                    COMMAND ${Qt5_LRELEASE_EXECUTABLE}
                    ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${qm}
                    DEPENDS ${_abs_FILE} VERBATIM
                )
                list(APPEND ${_qm_files} ${qm})
    endforeach()
    set(${_qm_files} ${${_qm_files}} PARENT_SCOPE)
endfunction()

function(fuoten_add_translation _qm_files)
    if (QT_VERSION VERSION_LESS "5.11.0")
        fuoten_add_trans_func("${_qm_files}" ${ARGN})
    else()
        if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
            qt5_add_translation("${_qm_files}" ${ARGN})
        elseif(QT_DEFAULT_MAJOR_VERSION EQUAL 6)
            qt6_add_translation("${_qm_files}" ${ARGN})
        endif()
    endif()
    set("${_qm_files}" "${${_qm_files}}" PARENT_SCOPE)
endfunction()

fuoten_add_translation(QM_FILES ${TS_FILES} OPTIONS -idbased)
add_custom_target(translations ALL
                  DEPENDS ${QM_FILES}
                  COMMENT "Compiles translation messages"
                  SOURCES ${TS_FILES})

install(FILES ${QM_FILES}
        DESTINATION ${LIBFUOTEN_I18NDIR}
        COMPONENT runtime)
