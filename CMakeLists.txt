cmake_minimum_required(VERSION 3.27)
project(Practica1PAG)

set(CMAKE_CXX_STANDARD 20)

# Define variables agrupando nombres de archivo
file( GLOB MY_FILES *.cpp ) # Código fuente de tu proyecto
file( GLOB GLAD_FILES GLAD/src/*.c ) # Código fuente de GLAD
file( GLOB IMGUI_FILES imgui/*.cpp )

# Indica qué archivos hay que compilar para generar el ejecutable
add_executable( ${PROJECT_NAME} ${MY_FILES}
        ${GLAD_FILES}
        ${IMGUI_FILES}
        ShaderProgram.cpp
        ShaderProgram.h
        Camara.h
        Camara.cpp
        Modelo.h
        Modelo.cpp
        Malla.cpp
        Malla.h
        Material.h
)

# Indica directorios (adicionales a los por defecto) para buscar includes
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include imgui/include)
file(COPY ${CMAKE_SOURCE_DIR}/Modelos DESTINATION ${CMAKE_BINARY_DIR})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
