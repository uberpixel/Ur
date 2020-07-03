cmake_minimum_required(VERSION 2.8.4)
project(Ur)

if(ANDROID)
    option(RN_BUILD_OCULUSMOBILE_MODULE "Build the Oculus Mobile module" ON)
    option(RN_BUILD_VULKAN_MODULE "Build the Vulkan renderer module" ON)
elseif(UNIX AND NOT APPLE)
    option(RN_BUILD_VULKAN_MODULE "Build the Vulkan renderer module" ON)
    option(RN_BUILD_OCULUS_MODULE "Build the Oculus module" OFF)
    option(RN_BUILD_OPENVR_MODULE "Build the OpenVR module" ON)
elseif(APPLE)
    option(RN_BUILD_VULKAN_MODULE "Build the Vulkan renderer module" OFF)
    option(RN_BUILD_OCULUS_MODULE "Build the Oculus module" OFF)
    option(RN_BUILD_OPENVR_MODULE "Build the OpenVR module" OFF)
elseif(MSVC)
    option(RN_BUILD_VULKAN_MODULE "Build the Vulkan renderer module" ON)
    option(RN_BUILD_D3D12_MODULE "Build the D3D12 renderer module" ON)
    option(RN_BUILD_OCULUS_MODULE "Build the Oculus module" ON)
    option(RN_BUILD_OPENVR_MODULE "Build the OpenVR module" ON)
else()
    option(RN_BUILD_D3D12_MODULE "Build the D3D12 renderer module" OFF)
    option(RN_BUILD_VULKAN_MODULE "Build the Vulkan renderer module" ON)
    option(RN_BUILD_OPENVR_MODULE "Build the OpenVR module" ON)
endif()

option(RN_BUILD_BULLET_MODULE "Build the Bullet physics module" ON)
option(RN_BUILD_PHYSX_MODULE "Build the PhysX physics module" OFF)
option(RN_BUILD_ASSIMP_MODULE "Build the Assimp module" ON)
option(RN_BUILD_OGG_MODULE "Build the ogg audio file module" ON)
option(RN_BUILD_STEAMAUDIO_MODULE "Build the Steam Audio module" OFF)
option(RN_BUILD_OPENAL_MODULE "Build the OpenAL module" ON)
option(RN_BUILD_ENET_MODULE "Build the ENet module" ON)
option(RN_BUILD_RECAST_MODULE "Build the Recast pathfinding module" OFF)
option(RN_BUILD_SAMPLES "Enable building the sample projects" OFF)

if(WIN32)
    set(CMAKE_SYSTEM_VERSION "10.0.10240.0")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Build)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH @executable_path;@executable_path/Resources/Modules/RayneBullet;@executable_path/Resources/Modules/RayneMetal;@executable_path/Resources/Modules/RayneOgg;@executable_path/Resources/Modules/RayneOpenAL;@executable_path/Resources/Modules/RayneOpenVR;@executable_path/Resources/Modules/RayneVR)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../Rayne" "${CMAKE_CURRENT_BINARY_DIR}/Rayne")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../Rayne/CMake/Rayne.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCES
	Sources/main.cpp
    Sources/URApplication.cpp
    Sources/URAsteroid.cpp
    Sources/URClient.cpp
    Sources/UREnemy.cpp
    Sources/URExplosion.cpp
    Sources/URMissile.cpp
    Sources/URRadar.cpp
    Sources/URServer.cpp
    Sources/URSpaceShip.cpp
    Sources/URTrailingEffect.cpp
    Sources/URWorld.cpp)

set(HEADERS
    Sources/URApplication.h
    Sources/URAsteroid.h
    Sources/URClient.h
    Sources/UREnemy.h
    Sources/URExplosion.h
    Sources/URMissile.h
    Sources/URPacket.h
    Sources/URRadar.h
    Sources/URServer.h
    Sources/URSpaceShip.h
    Sources/URTrailingEffect.h
    Sources/URWorld.h)

set(RESOURCES
	manifest.json
	Resources)

if(ANDROID)
    add_library(Ur SHARED "${SOURCES}" "${HEADERS}")
else()
    add_executable(Ur "${SOURCES}" "${HEADERS}")
endif()

rayne_copy_resources(Ur "${RESOURCES}" "")

rayne_link_with(Ur)
rayne_use_modules(Ur RayneBullet)
rayne_use_modules(Ur RayneOpenAL)
rayne_use_modules(Ur RayneOgg)
rayne_use_modules(Ur RayneENet)

if(NOT APPLE)
    rayne_use_modules(Ur RayneVR)
endif()

if(ANDROID)
    rayne_use_modules(Ur RayneVulkan)
    rayne_use_modules(Ur RayneOculusMobile)

    target_compile_definitions(Ur PRIVATE -DBUILD_FOR_OCULUS)
    target_include_directories(Ur PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/OculusPlatform/Include)
    target_link_libraries(Ur ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/OculusPlatform/Android/libs/arm64-v8a/libovrplatformloader.so)
elseif(UNIX AND NOT APPLE)
    rayne_use_modules(Ur RayneVulkan)
    rayne_use_modules(Ur RayneOpenVR)
else()
    if(NOT APPLE)
        rayne_use_modules(Ur RayneOpenVR)

        if(${RN_BUILD_VULKAN_MODULE})
            rayne_use_modules(Ur RayneVulkan)
        endif()

        if(WIN32 AND ${RN_BUILD_D3D12_MODULE})
            rayne_use_modules(Ur RayneD3D12)
            rayne_use_modules(Ur RayneOculus)
        endif()

        add_custom_command(TARGET Ur POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:OpenAL>" "$<TARGET_FILE_DIR:Ur>/$<TARGET_FILE_NAME:OpenAL>")
    else()
        rayne_use_modules(Ur RayneMetal)
    endif()
endif()

if(APPLE)
    set_target_properties(Ur PROPERTIES MACOSX_BUNDLE TRUE)
    #set_target_properties(Ur PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Other/Info.plist.in)
endif()

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Ur)
endif()
