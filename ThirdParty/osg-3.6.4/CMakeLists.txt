# add_library(osg SHARED IMPORTED GLOBAL)
# # file(GLOB_RECURSE OSG_DLL "${CMAKE_CURRENT_SOURCE_DIR}/bin/*.dll")
# # file(GLOB_RECURSE OSG_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.lib")

# foreach(idx ${OSG_DLL})
#   set_property(TARGET osg PROPERTY IMPORTED_LOCATION ${idx})
  
# endforeach()
# foreach(idx ${OSG_LIB})
#   set_property(TARGET osg PROPERTY IMPORTED_IMPLIB ${idx})
# endforeach()

# # set_property(TARGET osg PROPERTY IMPORTED_LOCATION ${OSG_DLL})
# set_property(TARGET osg PROPERTY IMPORTED_IMPLIB ${OSG_LIB})

install(FILES ${OSG_DLL} DESTINATION ${BINARY_ROOT_DIR})

# target_include_directories(osg INTERFACE ${OSG_INC_PATH})