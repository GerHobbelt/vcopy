project(libvcopy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}
	lib1-pub-impl.cpp
	lib1-priv-impl.cpp
)

add_library(libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#target_sources(${PROJECT_NAME}
#	PUBLIC
#		lib1-pub-impl.cpp
#	PRIVATE
#		lib1-priv-impl.cpp
#)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_11
)
