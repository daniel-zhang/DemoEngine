#define CATCH_CONFIG_RUNNER

// https://github.com/philsquared/Catch
#include "catch.hpp"

int main(int argc, char* const argv[])
{
    //
    // Setup global environment;
    //
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    Catch::Session session; // There must be exactly once instance

    // writing to session.configData() here sets defaults
    // this is the preferred way to set them

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) // Indicates a command line error
        return returnCode;

    // writing to session.configData() or session.Config() here 
    // overrides command line args
    // only do this if you know you need to

    int result = session.run();

    //
    // Teardown global environment;
    //
    CoUninitialize();

    return result;
}