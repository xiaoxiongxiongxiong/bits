# CMakeList.txt : CMake project, include source and define
# project specific logic here.
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
MESSAGE ("CMake version: " ${CMAKE_VERSION})

# 平台
OPTION (OS_BUILD_PLATFORM_32 "build 32-bit version" OFF)
IF (OS_BUILD_PLATFORM_32)
	SET (OS_BUILD_PLATFORM_CMD "-m32")
ELSE ()
	SET (OS_BUILD_PLATFORM_CMD "-m64")
ENDIF ()

# 宏定义
IF (CMAKE_HOST_WIN32)
     ADD_COMPILE_DEFINITIONS(OS_BITS_API_EXPORT=1)
ENDIF ()

# 包含路径
INCLUDE_DIRECTORIES ("./include")

# utils
SET (OS_BITS_INC ./include/bits.h)
SET (OS_BITS_SRC ./src/bits.cpp)
SET (OS_BITS_DEP )

# 标识
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OS_BUILD_PLATFORM_CMD}")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OS_BUILD_PLATFORM_CMD}")

# 动态库文件
MESSAGE (DEBUG ${OS_BITS_INC})
MESSAGE (DEBUG ${OS_BITS_SRC})
ADD_LIBRARY (os_bits SHARED
             ${OS_BITS_SRC})
TARGET_LINK_LIBRARIES (os_bits 
                       ${OS_BITS_DEP})

# 测试
OPTION (OS_BITS_WITH_EXAMPLE "build with test programs" OFF)
IF (OS_BITS_WITH_EXAMPLE)
     ADD_EXECUTABLE (bits_ctrl 
                     ./example/main.cpp)
     TARGET_LINK_LIBRARIES (bits_ctrl
                            os_bits)
ENDIF ()

# 安装 
INSTALL (FILES ${OS_BITS_INC} DESTINATION include)
INSTALL (TARGETS os_bits DESTINATION lib)
IF (OS_BITS_WITH_EXAMPLE)
     INSTALL (TARGETS bits_ctrl DESTINATION bin)
ENDIF ()
