
add_library(${ENGINE_NAME}

        include/Society.h
        src/Society/Engine.cpp
        include/Society/Engine.h

        # Main
        src/Society/Texture.cpp
        src/Society/Shader.cpp
        src/Society/Entity.cpp
        src/Society/Camera.cpp
        src/Society/Model.cpp
        src/Society/Mesh.cpp

        include/Society/Entity.h
        include/Society/Shader.h
        include/Society/Texture.h
        include/Society/Camera.h
        include/Society/Model.h
        include/Society/Mesh.h


        # Managers
        src/Society/Managers/ResourceManager.cpp
        src/Society/Managers/InputSystem.cpp
        src/Society/Managers/EventSystem.cpp

        include/Society/Managers/EventSystem.h
        include/Society/Managers/InputSystem.h
        include/Society/Managers/ResourceManager.h

        # Core
        include/Society/Core/Base.h

        src/Society/Core/Window.cpp
        src/Society/Core/Log.cpp
        src/Society/Core/Events.cpp

        include/Society/Core/Events.h
        include/Society/Core/Log.h
        include/Society/Core/Window.h

        # Components
        src/Society/Components/Component.cpp
        src/Society/Components/SpriteComponent.cpp
        src/Society/Components/TransformComponent.cpp
        src/Society/Components/ControllerComponent.cpp
        src/Society/Components/BoundingBoxDebugComponent.cpp
        src/Society/Components/Collision.cpp

        include/Society/Components/Component.h
        include/Society/Components/SpriteComponent.h
        include/Society/Components/TransformComponent.h
        include/Society/Components/ControllerComponent.h
        include/Society/Components/BoundingBoxDebugComponent.h
        include/Society/Components/Collision.h

        # Interfaces
        include/Society/Interfaces/ISingleton.h

        # Vendors
        include/Society/Vendor/stb_image.h
        src/Society/Vendor/stb_image.cpp
        src/Society/Core/Callbacks.cpp
        include/Society/Core/Callbacks.h

        # Primitive Meshes
        include/Society/Meshes/Sphere.h
        include/Society/Meshes/Cube.h
        include/Society/Meshes/Quad.h
        include/Society/Meshes/PrimitiveMesh.h
        
        src/Society/Meshes/Cube.cpp
        src/Society/Meshes/Sphere.cpp
        src/Society/Meshes/Quad.cpp


)


#target_precompile_headers(${LIB_NAME} PUBLIC pch/pch.h)

#target_include_directories(${ENGINE_NAME}
#        INTERFACE
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
#        $<INSTALL_INTERFACE:include>
#)

#target_include_directories(${ENGINE_NAME}
#        PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pch>
#        #        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#)


target_include_directories(${ENGINE_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${ENGINE_NAME} PUBLIC compiler_flags ${PACKAGE_LIBRARIES})

# paths for binaries and headers
install(TARGETS ${ENGINE_NAME} compiler_flags
        EXPORT "${ENGINE_NAME}Targets"
        DESTINATION lib
        #        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
        #        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
        #        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
        #        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
        #        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ENGINE_NAME} # include/SomeLibrary
)

install(DIRECTORY include/ DESTINATION include/Society)

# CMake config name, namespace and path
install(
        EXPORT "${ENGINE_NAME}Targets"
        FILE "${ENGINE_NAME}Targets.cmake"
        NAMESPACE Society::
        DESTINATION lib/cmake/Society
)


#
#install(TARGETS ${LIB_NAME}
#        EXPORT SocietyTargets
#        DESTINATION lib)
#install(FILES Society.h
#        DESTINATION include)

# install include headers
#install(FILES Society.h
#        CONFIGURATIONS Debug
#        DESTINATION include)


#install(EXPORT SocietyTargets
#        FILE SocietyTargets.cmake
#        DESTINATION lib/cmake/Society
#)

#message(${SocietyTargets})