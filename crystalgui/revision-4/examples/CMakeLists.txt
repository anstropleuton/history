set(CGUI_EXAMPLES
	"basic.c"
)

foreach(example_file ${CGUI_EXAMPLES})
	get_filename_component(example ${example_file} NAME_WE)
	add_executable(cgui_example_${example} ${example_file})
	set_target_properties(cgui_example_${example} PROPERTIES OUTPUT_NAME "${example}") # For convenience
	target_link_libraries(cgui_example_${example} PRIVATE crystalgui)
	target_link_libraries(cgui_example_${example} PRIVATE cgui_options)
endforeach()
