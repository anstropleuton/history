set(CGUI_HEADERS
	"cgui_api.h"
	"cgui_array.h"
	"cgui_compiler.h"
	"cgui_event.h"
	"cgui_extra.h"
	"cgui_icon.h"
	"cgui_layout.h"
	"cgui_matrix3.h"
	"cgui_misc.h"
	"cgui_node.h"
	"cgui_proto.h"
	"cgui_render.h"
	"cgui_shape.h"
	"cgui_sound.h"
	"cgui_theme.h"
	"cgui_transition.h"
	"cgui_widget.h"
	"crystalgui.h"
)

set(CGUI_SOURCES
	"cgui_extra.c"
	"cgui_layout.c"
	"cgui_node.c"
	"cgui_shape.c"
	"cgui_transition.c"
)

add_library(crystalgui ${CGUI_SOURCES})

target_include_directories(crystalgui
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/internal
)

target_link_libraries(crystalgui PUBLIC raylib)
target_link_libraries(crystalgui PRIVATE cgui_options)

include(GNUInstallDirs)

install(TARGETS crystalgui cgui_options
	EXPORT CrystalGUITargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CGUI_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crystalgui
)

install(EXPORT CrystalGUITargets
	FILE CrystalGUITargets.cmake
	NAMESPACE CrystalGUI::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CrystalGUI
)
