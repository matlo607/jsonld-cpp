#ifndef IMPL_REPORT_TESTRUNNER
#define IMPL_REPORT_TESTRUNNER

#include <set>
#include <string>
#include <vector>
#include <set>
#include "TestResult.h"
#include "CommandRunner.h"
#include <sstream>

class TestRunner
{
    private:
        std::string project;
        std::string manifest;
        std::string user;
        std::string path;
        std::vector<std::vector<std::string>> executables;
        size_t executable_iterator;
        CommandRunner cr;
        std::string command_output;
        std::string next_output;
        std::stringstream ss;
        bool has_next_output;
        //helper methods
        bool next_executable();
        void find_next_result();

    public:
        // constructor
        TestRunner(std::string, std::string, std::string, std::vector<std::vector<std::string>>);
        // control methods
        void start();
        TestResult next_result();
        bool has_next();
};
#endif // IMPL_REPORT_TESTRUNNER