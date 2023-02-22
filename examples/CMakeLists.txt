set(TARGET_jsonld2rdf jsonld2rdf)

set(jsonld2rdf_src jsonld2rdf.cpp)
set_source_files_properties(${jsonld2rdf_src} PROPERTIES LANGUAGE "CXX")

add_executable(${TARGET_jsonld2rdf} ${jsonld2rdf_src})
target_compile_definitions(${TARGET_jsonld2rdf}
    PRIVATE
    -DJSONLDCPP_VERSION_MAJOR=${JSONLDCPP_VERSION_MAJOR}
    -DJSONLDCPP_VERSION_MINOR=${JSONLDCPP_VERSION_MINOR}
    -DJSONLDCPP_VERSION_PATCH=${JSONLDCPP_VERSION_PATCH})
target_link_libraries(${TARGET_jsonld2rdf} PRIVATE jsonld-cpp)

install(TARGETS ${TARGET_jsonld2rdf}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})