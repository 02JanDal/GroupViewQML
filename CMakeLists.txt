cmake_minimum_required(VERSION 3.0)

project(GroupView)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_EXAMPLE "Should the example be built?" ON)

qt5_add_resources(LIB_RES src/resources.qrc)
add_library(groupview SHARED
	src/GroupView.h
	src/GroupView.cpp
	src/GroupViewHelper_p.h
	src/GroupViewHelper.cpp
	src/GroupViewDropArea_p.h
	src/GroupViewDropArea.cpp
	src/GroupViewProxy_p.h
	src/GroupViewProxy.cpp
	${LIB_RES}
)
target_link_libraries(groupview PUBLIC Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)
target_include_directories(groupview PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)

add_custom_target(dummy_files_for_ide_lib SOURCES src/qml/GroupView.qml src/qml/ScrollBar.qml)

if(WITH_EXAMPLE)
	qt5_add_resources(EXAMPLE_RES example/images.qrc example/qml.qrc)
	add_executable(groupview_example
		example/InstanceModel.h
		example/InstanceModel.cpp
		example/main.cpp
		${EXAMPLE_RES}
	)
	target_link_libraries(groupview_example groupview)

	add_custom_target(dummy_files_for_ide_example SOURCES example/main.qml)
endif()

install(TARGETS groupview EXPORT GroupView
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include
)
install(EXPORT GroupView DESTINATION cmake)
install(FILES src/GroupView.h DESTINATION include)
