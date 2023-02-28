function(SetupEmbed)
    if (NOT EXISTS ${PROJECT_DIR}/classes)
        file(MAKE_DIRECTORY ${PROJECT_DIR}/classes)
    endif ()

    # add_library(class_files
    #         ${PROJECT_DIR}/components/classes_combined/lib/classes.h
    #         ${PROJECT_DIR}/components/classes_combined/classes_combined.c)
    # target_include_directories(class_files PUBLIC ${PROJECT_DIR}/classes)

    set(output_h "
#ifndef BIFIT_CLASSES_H_
#define BIFIT_CLASSES_H_
#include \"stdint.h\"
")
    file(WRITE ${PROJECT_DIR}/components/classes_combined/lib/classes.h
            ${output_h})

    set(output_c "
#ifndef BIFIT_CLASSES_COMBINED_H_
#define BIFIT_CLASSES_COMBINED_H_
#include \"include/classes.h\"
")
    file(WRITE ${PROJECT_DIR}/components/classes_combined/include/classes_combined.h
            ${output_c})
endfunction()

function(EmbedManifestFile path)

    message("load manifest file ${path}")

    # load manifest file into ${manifest_content}
    file(READ ${path} manifest_content)

    # find the main class entry
    string(REGEX MATCH "Main-Class: .*" main_class_line ${manifest_content})

    # get the identifier
    string(REPLACE "Main-Class: " "" main_class_identifier ${main_class_line})
    string(REGEX REPLACE "\n" "" main_class_identifier "${main_class_identifier}")
    string(REGEX REPLACE "\\." "/" main_class_identifier "${main_class_identifier}")
    message("found main class identifier: ${main_class_identifier}")

    # make the identifier accessible in C
    set(output_h "
extern char * bifit_main_class_identifier\;
")
    file(APPEND ${PROJECT_DIR}/components/classes_combined/lib/classes.h
            ${output_h})

    set(output_c "
char * bifit_main_class_identifier = \"${main_class_identifier}\"\;
")
    file(APPEND ${PROJECT_DIR}/components/classes_combined/include/classes_combined.h
            ${output_c})

endfunction()

function(EmbedClassFiles path)

    file(GLOB_RECURSE files ${path})

    set(output_c "
classfile_pointer bifit_embedded_class_files[] = {")

    file(APPEND ${PROJECT_DIR}/components/classes_combined/include/classes_combined.h
            ${output_c})

    set(counter 0)
    foreach (file ${files})
        EmbedClassFile(${file})
        MATH(EXPR counter "${counter}+1")
    endforeach ()

    set(output_combined "
\n};\n
unsigned int bifit_embedded_class_files_size = ${counter};
#endif // CLASSES_COMBINED_H
")

    file(APPEND ${PROJECT_DIR}/components/classes_combined/include/classes_combined.h
            ${output_combined})

    set(output_h "
typedef uint8_t * classfile_pointer\;
extern classfile_pointer bifit_embedded_class_files[]\;
unsigned int bifit_embedded_class_files_size\;
#endif // CLASSES_COMBINED_H
")

    file(APPEND ${PROJECT_DIR}/components/classes_combined/lib/classes.h
            ${output_h})

endfunction()

function(EmbedClassFile file)
    message("embed class file ${file}")
    ClassFileGenerate(${file} var)
endfunction()

function(ClassFileGenerate file generated_c)

    get_filename_component(base_filename ${file} NAME)
    set(output_filename "${base_filename}.c")
    string(MAKE_C_IDENTIFIER ${base_filename} c_name)
    file(READ ${file} content HEX)

    string(REGEX MATCHALL "([A-Fa-f0-9][A-Fa-f0-9])" SEPARATED_HEX ${content})

    set(output_c "")

    set(counter 0)
    foreach (hex IN LISTS SEPARATED_HEX)
        string(APPEND output_c "0x${hex},")
        MATH(EXPR counter "${counter}+1")
        if (counter GREATER 16)
            string(APPEND output_c "\n    ")
            set(counter 0)
        endif ()
    endforeach ()

    set(output_h "
#ifndef ${c_name}_H
#define ${c_name}_H
#include \"stdint.h\"
uint8_t ${c_name}_data[] = {
    ${output_c}
}\;
unsigned ${c_name}_size = sizeof(${c_name}_data)\;
#endif // ${c_name}_H
")

    # TODO: PRIV_INCLUDE_DIRS?
    file(WRITE ${PROJECT_DIR}/components/classes_combined/lib/${c_name}.h
            ${output_h})

    file(APPEND ${PROJECT_DIR}/components/classes_combined/lib/classes.h
            "\n#include \"${c_name}.h\"")

    file(APPEND ${PROJECT_DIR}/components/classes_combined/include/classes_combined.h
            "\n${c_name}_data,")

    set(${generated_c} ${PROJECT_DIR}/components/classes_combined/${c_name}.c PARENT_SCOPE)

endfunction()

if (RUN_CLASS_FILE_GENERATE)
    ClassFileGenerate(${CLASS_FILE_GENERATE_PATH} var)
endif ()
