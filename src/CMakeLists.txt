
add_executable(BoxRender 
    main.cpp
    stb_image_write.hpp
    stb_image.hpp
    fileController.cpp fileController.h    
    graphics/renderEngine.cpp graphics/renderEngine.h
    graphics/privateRenderEngine.cpp graphics/privateRenderEngine.h
)
target_link_libraries(BoxRender
    vulkan
)
target_include_directories(BoxRender PRIVATE
    ./
)


