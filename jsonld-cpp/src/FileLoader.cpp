#include "jsonld-cpp/FileLoader.h"
#include "jsonld-cpp/MediaType.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/JSONDocument.h"
#include "jsonld-cpp/RDFDocument.h"
#include <sstream>
#include <fstream>

using json = nlohmann::json;

namespace {

    MediaType detectContentType(const std::string& filename) {
        if(filename.empty())
            throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                              "filename is empty");

        const std::string nq = ".nq";
        if(filename.rfind(nq) == filename.length() - nq.length())
            return MediaType::n_quads();

        const std::string json = ".json";
        if(filename.rfind(json) == filename.length() - json.length())
            return MediaType::json();

        const std::string jsonld = ".jsonld";
        if(filename.rfind(jsonld) == filename.length() - jsonld.length())
            return MediaType::json_ld();

        const std::string html = ".html";
        if(filename.rfind(html) == filename.length() - html.length())
            return MediaType::html();

        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Unknown media type for file: " + filename);
    }

}

FileLoader::FileLoader(const std::filesystem::path& filepath):
    istream_ptr_(nullptr),
    filepath_(filepath),
    content_type_(MediaType::any())
{}

FileLoader::FileLoader(std::istream& istream, MediaType content_type):
    istream_ptr_(&istream),
    filepath_(),
    content_type_(content_type)
{}

std::unique_ptr<RemoteDocument> FileLoader::loadDocument(
    MediaType contentType,
    std::istream& inputStream,
    std::string localUrl)
{
    if (JSONDocument::accepts(contentType)) {
        if(localUrl.find("file://") == std::string::npos) {
            localUrl = "file://" + localUrl;
        }

        return std::unique_ptr<RemoteDocument>(
                new JSONDocument(JSONDocument::of(contentType, inputStream, localUrl)));
    }

    if (RDFDocument::accepts(contentType)) {
        return std::unique_ptr<RemoteDocument>(
                new RDFDocument(RDFDocument::of(contentType, inputStream)));
    }

    std::stringstream ss;
    ss << "Unsupported content type: '" << contentType << "'. Supported content types are: ["
       << MediaType::json_ld() << ", " << MediaType::json() << " and " << MediaType::n_quads() << "]";
    throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
}

std::unique_ptr<RemoteDocument> FileLoader::loadDocument(const std::string &url) {
    using namespace std::string_literals;
    // todo: check cache

    // check file properties
    if(url.find("file://") != 0 && url.find('/') != 0)
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Unsupported URL scheme. FileLoader only loads files.");

    std::string localUrl = url;

    if(localUrl.find("file://") == 0) {
        localUrl = localUrl.substr(7);
    }

    //MediaType contentType = (istream_ptr_) ? content_type_ : detectContentType(localUrl);
    MediaType contentType = detectContentType(localUrl);
    // open stream
    if (istream_ptr_) {
        if (istream_ptr_->good() && !istream_ptr_->eof()) {
            auto doc = loadDocument(contentType, *istream_ptr_, localUrl);
            // Reset string stream as this function can be called several time on the same string stream.
            istream_ptr_->seekg(0, istream_ptr_->beg);
            return doc;
        }
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Stream associated to \""s + localUrl + "\" was supposed to be open"s);
    } else {
        std::ifstream inputStream{localUrl};
        if(!inputStream.is_open()) {
            throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                              "Failed to open file: "s + localUrl);
        }
        // todo: add to cache
        return loadDocument(contentType, inputStream, localUrl);
    }
}

FileLoader *FileLoader::clone() const {
    return new FileLoader(*this);
}
