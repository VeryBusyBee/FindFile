#include <iostream>
#include <vector>
#include <string>
#include "libsearch.hpp"
#include <experimental/filesystem>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::experimental::filesystem::directory_iterator;
using std::experimental::filesystem::recursive_directory_iterator;

int main(int argc, char *argv[]) {
    string path = "/usr/include";
    string sname;

    if (argc < 2) {
        cout << "Enter the name to look for" << endl;
        return EXIT_SUCCESS;
    }

    else
        sname.assign(argv[1]);

    cout << "Looking for '" << sname << "' in " << path << endl;

    for (const auto & file : directory_iterator(path)) {};

    run_search(path, sname);

    return EXIT_SUCCESS;
}
