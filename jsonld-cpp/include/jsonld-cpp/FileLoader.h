#ifndef LIBJSONLD_CPP_FILELOADER_H
#define LIBJSONLD_CPP_FILELOADER_H

#include <jsonld-cpp/DocumentLoader.h>
#include <jsonld-cpp/MediaType.h>

#include <filesystem>
#include <functional>
#include <istream>

class RemoteDocument;

class FileLoader:
    public DocumentLoader
{
    public:
        FileLoader(const std::filesystem::path&);
        FileLoader(std::istream&, MediaType);
        FileLoader* clone() const override;

        // load url and return a RemoteDocument
        std::unique_ptr<RemoteDocument> loadDocument(const std::string &url) override;

    private:
        std::unique_ptr<RemoteDocument> loadDocument(
            MediaType contentType,
            std::istream& inputStream,
            std::string localUrl);

        std::istream* istream_ptr_;
        std::filesystem::path filepath_;
        MediaType content_type_;
};

#endif //LIBJSONLD_CPP_FILELOADER_H
