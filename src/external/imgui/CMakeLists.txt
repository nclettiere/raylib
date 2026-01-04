find_package(SDL3 REQUIRED)

add_library(ImGui STATIC
	imconfig.h
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_internal.h
	imgui_stdlib.cpp
	imgui_stdlib.h
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.cpp
	imgui.h
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
	backends/imgui_impl_sdl3.cpp
	backends/imgui_impl_sdl3.h
	backends/imgui_impl_opengl3.cpp
	backends/imgui_impl_opengl3.h)
	
target_link_libraries(ImGui PUBLIC SDL3::SDL3)

target_include_directories(ImGui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" raylib)