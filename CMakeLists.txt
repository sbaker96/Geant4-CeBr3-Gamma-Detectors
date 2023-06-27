#(1)

cmake_minimum_required(VERSION 3.16...3.21)
project(CeBr3Detector)

#(2)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#(3)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#(4)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#(5)
add_executable(CeBr3Detector CeBr3Detector.cc ${sources} ${headers})
target_link_libraries(CeBr3Detector ${Geant4_LIBRARIES})


#(6)
set(EX_SCRIPTS
	vis.mac
	60Co.mac
	137Cs.mac
	152Eu.mac

	exampleRun.mac
	
	GeneratePlots.C
	GetCounts.C

	SphericalShell.obj
	)

foreach(_script ${EX_SCRIPTS})
	configure_file(
	${PROJECT_SOURCE_DIR}/${_script}
	${PROJECT_BINARY_DIR}/${_script}
	COPYONLY
	)
endforeach()

#(7)

install(TARGETS CeBr3Detector DESTINATION bin)

