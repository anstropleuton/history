enable_testing()

set(CGUI_TESTS)

foreach(test_file ${CGUI_TESTS})
	get_filename_component(test ${test_file} NAME_WE)
	add_executable(cgui_test_${test} ${test_file})
	add_test(NAME cgui_test_${test} COMMAND cgui_test_${test})
	target_link_libraries(cgui_test_${test} PRIVATE crystalgui)
	target_link_libraries(cgui_test_${test} PRIVATE cgui_options)
endforeach()
