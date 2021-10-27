#ifndef __COD_HPP
#define __COD_HPP

#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
using std::string;

#ifndef IgnoreHidden
#define IgnoreHidden true
#endif

#ifndef ZipExtension
#define ZipExtension "zip"
#endif

#ifndef GZipExtension
#define GZipExtension "gz"
#endif

#ifndef TarExtension
#define TarExtension "tar"
#endif

/**
 * @brief Find extensions of in a directory
 * Directory should include only one data type
 *
 * @param dir_path : (string) directory to be searched
 * @return string: found extension Gzip, zip or tar
 */
string find_ext(string dir_path);

struct DECOMPRESSDIR {
    /** DECOMPRESSDIR STRUCT
     * @brief Decompressor ( functor ) of refacto library
     *
     *  Looks for file extension and assign the correct decompressor
     *  - options:
     *          - tar files
     *          - zip files
     *          - gzip files
     *
     *  Runs system call ( linux only ) to find the files with the correct
     * extensions Uses the tar, gzip zip operations
     *
     */
    string extension;

    /**
     * @brief Construct a new DECOMPRESSDIR object
     *
     * @param extension: (string) default is set to searh for extension
     *      if the extension is given, extension search is skipped
     * @param extension
     */
    DECOMPRESSDIR(string extension = "") : extension{extension} {}

    /**
     * @brief DECOMPRESSDIR Operator
     * Overloaded operator
     *
     * @param path: (string) directory path which files be extracted
     */
    void operator()(string path) {
        if (extension.empty()) extension = find_ext(path);
        char tmp[256];
        getcwd(tmp, 256);
        string cwd{tmp};

        string cmd = "unzip {} -d " + cwd + "/" + path + " \\;";
        if (extension == "gz")
            cmd = "gunzip -k {} \\;";
        else if (extension == "tar")
            cmd = "tar -xzvf {} -C" + cwd + "/" + path + " \\;";
        else if (extension == "zip")
            cmd = "unzip {} -d " + cwd + "/" + path + " \\;";
        std::ostringstream stringStream;
        stringStream << "yes 2>/dev/null"
                     << " | "
                     << "find " << path << " -name '*." << extension
                     << "' -type f -exec " << cmd;
        std::system(stringStream.str().c_str());
    }
};

struct COMPRESSDIR {
    /** DECOMPRESSDIR STRUCT
     * @brief Decompressor ( functor ) of refacto library
     *
     *  Looks for file extension and assign the correct decompressor
     *  - options:
     *          - tar files
     *          - zip files
     *          - gzip files
     *
     *  Runs system call ( linux only ) to find the files with the correct
     * extensions Uses the tar, gzip zip operations
     *
     */
    void operator()(string path, string name) {
        char tmp[256];
        getcwd(tmp, 256);
        string cwd{tmp};
        string cmd;
        std::ostringstream stringStream;
        std::cout << cwd << "/" << name << ".tar.gz " << std::endl;
        stringStream << "tar -czvf " << cwd << "/" << name << ".tar.gz "
                     << path;
        std::system(stringStream.str().c_str());
    }
};

struct CLEANFILES {
    /**
     * @brief File Cleaner struct
     *
     * Clean files in given directory
     * Deleted files are NOT recoverable
     * @param pattern: (string) pattern of seach file to be deleted
     */
    void operator()(string path, string pattern) {
        char tmp[256];
        getcwd(tmp, 256);
        string cmd{"rm"};
        string cwd{tmp};
        // string cmd="unzip {} -d " + cwd +"/"+ path ;
        std::ostringstream stringStream;
        stringStream << "yes "
                     << " | "
                     << "find " << path << " -name '" << pattern
                     << "' -type f -exec " << cmd << " {} \\;";
        std::system(stringStream.str().c_str());
    }
};

string find_ext(string dir_path) {
    DIR* dirFile = opendir(dir_path.c_str());

    if (dirFile != NULL) {
        struct dirent* hFile;
        errno = 0;
        while ((hFile = readdir(dirFile)) != NULL) {
            if (!strcmp(hFile->d_name, ".")) continue;
            if (!strcmp(hFile->d_name, "..")) continue;
            if (IgnoreHidden && (hFile->d_name[0] == '.')) continue;

            if (strstr(hFile->d_name, GZipExtension))
                return GZipExtension;
            else if (strstr(hFile->d_name, ZipExtension))
                return ZipExtension;
            else if (strstr(hFile->d_name, TarExtension))
                return TarExtension;
        }
        closedir(dirFile);
        throw std::runtime_error("No Files Found at " + dir_path);
    } else {
        throw std::invalid_argument("Cannot open " + dir_path);
    }
}

#endif