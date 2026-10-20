set(CGUI_EXPERIMENTS
    "fields_extension.c"
	"gradients_approaches.c"
	"inverted_text.c"
	"node_layouts.c"
)

foreach(experiment_file ${CGUI_EXPERIMENTS})
	get_filename_component(experiment ${experiment_file} NAME_WE)
	add_executable(cgui_experiment_${experiment} ${experiment_file})
	target_link_libraries(cgui_experiment_${experiment} PRIVATE crystalgui)
	target_link_libraries(cgui_experiment_${experiment} PRIVATE cgui_options)
endforeach()
