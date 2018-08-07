cmake_minimum_required(VERSION 3.4.1)

set(BOOST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../boost_1_67_0)
set(BOOST_GIL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/boost-gil-extension)
set(ZIP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/julienr-libzip-android/jni)
set(LIBPNG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/libpng_1.4.1_android)
set(LIBJPEG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/jpeg-9)
set(EIGEN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../eigen)
set(SOL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../sol2)
set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/lua-5.3.4/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../tes-engine)
include_directories(${BOOST_PATH})
include_directories(${EIGEN_PATH})
include_directories(${SOL2_PATH})
include_directories(${LUA_PATH})
include_directories(${LIBPNG_PATH})
include_directories(${LIBJPEG_PATH})
include_directories(${ZIP_PATH})
include_directories(${BOOST_GIL_PATH})

add_definitions(-DTARGET_ANDROID)

add_library(engine SHARED IMPORTED

)
			 
set_target_properties(engine PROPERTIES IMPORTED_LOCATION

    ${CMAKE_CURRENT_SOURCE_DIR}/../../../tes-engine/SalmonEngineAndroid/app/build/intermediates/cmake/debug/obj/${ANDROID_ABI}/libengine.so

)

add_library(DoubleHitBalls SHARED

    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/android_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/main_code.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/creditscode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/galaxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/galaxy_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/galaxy_objects.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/gamecode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/loadingcode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../game/menucode.cpp

)

find_library(log-lib
    log
)
			  
find_library(GLESv2-lib
    GLESv2
)

target_link_libraries(DoubleHitBalls
    engine
    ${log-lib}
    ${GLESv2-lib}
)
