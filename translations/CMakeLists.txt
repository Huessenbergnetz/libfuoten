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

qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations ALL
                  DEPENDS ${QM_FILES}
                  COMMENT "Compiles translation messages")

install(FILES ${QM_FILES}
        DESTINATION ${I18NDIR}
        COMPONENT runtime)
