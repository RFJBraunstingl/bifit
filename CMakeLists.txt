# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_world)

# provide custom implementation for base classes
message("run gradle before compile")
execute_process(
        COMMAND bash -c "gradle build"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/system"
        OUTPUT_VARIABLE outVar
)
message("gradle process executed - output was:")
message("${outVar}")
file(
        REMOVE_RECURSE
        "${PROJECT_SOURCE_DIR}/jar/java"
)
file(
        RENAME
        "${PROJECT_SOURCE_DIR}/system/build/classes/java/main"
        "${PROJECT_SOURCE_DIR}/jar/java"
)

# include all classes
include(EmbedJarContent.cmake)
SetupEmbed()
EmbedManifestFile("jar/META-INF/MANIFEST.MF")
EmbedClassFiles("jar/*.class")