find_package(nlohmann_json REQUIRED)
find_package(uriparser 0.9.7 CONFIG REQUIRED char wchar_t)
#find_package(cpr REQUIRED)
#find_package(http-link-header-cpp REQUIRED)

set(TARGET_jsonldcpp jsonld-cpp)

set(Jsonldcpp_sources
    src/Context.cpp
    src/ContextProcessor.cpp
    src/DocumentLoader.cpp
    src/DoubleFormatter.cpp
    src/ExpansionProcessor.cpp
    src/FileLoader.cpp
    src/JSONDocument.cpp
    src/JsonLdError.cpp
    src/JsonLdOptions.cpp
    src/JsonLdProcessor.cpp
    src/JsonLdUrl.cpp
    src/JsonLdUtils.cpp
    src/MediaType.cpp
    src/Permutator.cpp
    src/RDFCanonicalizationProcessor.cpp
    src/RDFDataset.cpp
    src/RDFDatasetUtils.cpp
    src/RDFDatasetComparison.cpp
    src/RDFDocument.cpp
    src/RDFNode.cpp
    src/RDFSerializationProcessor.cpp
    src/RDFTriple.cpp
    src/RDFQuad.cpp
    src/RemoteDocument.cpp
    src/BlankNodeNames.cpp
    src/Uri.cpp
    src/UriParser.cpp
    src/sha256.cpp)

set_source_files_properties(${Jsonldcpp_sources} PROPERTIES LANGUAGE "CXX")

add_library(${TARGET_jsonldcpp} SHARED ${Jsonldcpp_sources})

target_include_directories(${TARGET_jsonldcpp}
    INTERFACE
    nlohmann_json::nlohmann_json
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(${TARGET_jsonldcpp}
    PUBLIC
    uriparser::uriparser
    nlohmann_json::nlohmann_json
    #PRIVATE
    #cpr::cpr # unused ?
    #http-link-header-cpp::http-link-header-cpp # unused ?
    )

target_compile_definitions(${TARGET_jsonldcpp}
    PRIVATE
    -DJSONLDCPP_VERSION_MAJOR=${JSONLDCPP_VERSION_MAJOR}
    -DJSONLDCPP_VERSION_MINOR=${JSONLDCPP_VERSION_MINOR}
    -DJSONLDCPP_VERSION_PATCH=${JSONLDCPP_VERSION_PATCH})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/jsonld-cpp" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS "${TARGET_jsonldcpp}"
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})