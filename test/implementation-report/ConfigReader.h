#ifndef IMPL_REPORT_CONFIG
#define IMPL_REPORT_CONFIG

#include <string>
#include <set>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * \brief   Interface for reading the configuration file
 *
 * \details The ConfigReader class has been created to handle the parsing of the
 *          configuration file generated by configuration.html in the project source
 *          directory
 *
 *          The configuration file is in json format and generated from the
 *          configuration.html file that is included in the implementation-report source
 *          directory.
 *
 *          The structure of the data in the file is split into two sections:
 *
 *          1.  "header" which contains the information to be included at the top of the
 *              generated implementation report.  This is further split into the sections
 *              \li  "prefix" that provides the context for the implementation-report
 *              \li  "subject" that provides the project and developer data in a format that
 *                  will be used in the implementation report
 *          2.  "testsuites" which details the testsuites to be included in the
 *              implementation report
 *
 *          The JSON is expected in the following format:
 *
 * \code
 *          {
 *              "header": {
 *                  "prefix": [
 *                      { <prefix> : <url> }
 *                  ],
 *                  "subject": [
 *                      {
 *                          "id" : <identifier>,
 *                          "type" : <object>,
 *                          "value" : <value>,
 *                          "properties" : [
 *                              {
 *                                  "type" : <object>,
 *                                  "value" : <value>
 *                              }
 *                          ]
 *                      }
 *                  ]
 *              },
 *              "testsuites" : [
 *                  {
 *                      "file" : <filename>,
 *                      "manifest" : <manifest name>
 *                  }
 *              ]
 *          }
 * \endcode
 */
class ConfigReader
{
    private:
        nlohmann::json document;

    public:

        /**
         * \brief Constructor for the ConfigReader
         *
         * \param filename The full path to the configuration file as detailed in \ref config_sec
         */
        explicit ConfigReader(const std::string& filename);

        /**
         * \brief Destructor for the ConfigReader
         */
        ~ConfigReader() = default;

        /**
         *  \brief  Getter for the TestSuite data
         *
         *  \return This method reformats the json testsuite data as a vector of vectors
         *          containing the "file" and "manifest" values from each of the
         *          testsuites defined in the generated configuration file.
         */
        std::vector<std::vector<std::string>> getTestsuites();

        /**
         *  \brief   Getter for the subject data
         *
         *  \details The subject data is contained in the "header" section of
         *           the configuration file and this method returns it.
         *
         *  \return  Returns the subject data as json from the generated
         *           configuration file.
         */
        nlohmann::json getSubjects();

        /**
         *  \brief   Getter for the namespace prefixes required
         *
         *  \details The namespace prefixes are contained in the "header" section of
         *           the configuration file and this method returns it.
         *
         *  \return  Returns the namespace prefixes as json from the generated
         *           configuration file.
         */
        nlohmann::json getPrefixes();
};

#endif //IMPL_REPORT_CONFIG