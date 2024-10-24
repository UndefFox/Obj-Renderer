
add_executable(obj-renderer 
    main.cpp
    stb_image_write.hpp
    stb_image.hpp
    fileController.cpp fileController.h    
    graphics/renderEngine.cpp graphics/renderEngine.h
    graphics/renderDevice.cpp graphics/renderDevice.h
    graphics/renderResources.cpp graphics/renderResources.h
    graphics/renderColorPass.cpp graphics/renderColorPass.h
    graphics/renderTextPass.cpp graphics/renderTextPass.h
    graphics/renderHelpers.cpp graphics/renderHelpers.h
    graphics/renderLinePass.cpp graphics/renderLinePass.h
    graphics/vertex.h
    graphics/uboCamera.h
    graphics/renderParametrs.h
)
target_link_libraries(obj-renderer
    vulkan
)
target_include_directories(obj-renderer PRIVATE
    ./
)


