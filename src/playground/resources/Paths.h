/*
 * Paths.h
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */

#pragma once
#include <sys/stat.h>
#include <stdio.h>
#include "JavaLike.h"
#include "StringUtils.h"

class Paths {
public:

  /**
   * Mostly a helper for error messages - it is clearer to have an absolutePath
   */
  static String absolute(const String &relative) {
    char resolvedPath[PATH_MAX];
    realpath(relative.c_str(), resolvedPath);

    return String(resolvedPath);
  }

  static String normalized(const String &path) {
    std::vector<String> tokens = StringUtils::split(path, '/');
    std::vector<String>normalizedPath;

    //printf("Normalized [%s]\n", path.c_str());

    for(auto token : tokens) {
      //printf("token [%s]\n", token.c_str());
      token = StringUtils::trim(token);
      if(token == "..") {
        if(normalizedPath.size() > 0) {
          normalizedPath.pop_back();
        } else {
          throw std::invalid_argument("Path [%s] can not refer outside of the root");
        }
      } else if (token == "") {
        if(normalizedPath.size() == 0) {  //this means a starting / in path, then add "" to have a starting / in the result
          normalizedPath.push_back("");
        }
      } else if(token == "~") { //only support ~ at the start of the path
        if(normalizedPath.size() != 0) {
          throw std::invalid_argument("Invalid path [" + path + "]. Unexpected '~': only supported as first character");
        } else {
          normalizedPath.push_back(""); //add "" to have a starting / in the result
        }
      } else if (token == ".") { // skip
      } else {
        normalizedPath.push_back(token);
      }
      //printf("Normalized path: [%s]\n", StringUtils::join(normalizedPath, '/').c_str());
    }

    //printf("\n");

    return StringUtils::join(normalizedPath, '/');
  }


  /**
   * Absolute paths are considered if starting with / or ~/.
   * if provided postfix is an absolute path, return that.
   * Otherwise return normalized "prefix/postfix"
   */
  static String add(const String &prefix, const String &postFix) {
    String path = StringUtils::trim(postFix);
    String pathPrefix = StringUtils::trim(prefix);

    if (path.find("/") != 0 && path.find("~/") != 0 && !pathPrefix.empty()) { //if prefix is empty or postfix is absolute path, then return postfix, otherwise concatenate prefix + / + postfix
      if(path.empty()) { //if postfix is empty, return prefix
        path = pathPrefix;
      } else {
        path = pathPrefix + "/" + path;
      }
    }

    return Paths::normalized(path);
  }

  /**
   * Return the folder part of a path to a file.
   */
  static String getDirname(const String &filePath) {
    if (isFile(filePath)) {
      unsigned long location = filePath.find_last_of('/');
      return (location == std::string::npos ? "." : filePath.substr(0, location));
    }

    return filePath;
  }

  /**
   * Return the file part of a path to a file.
   */
  static String getBasename(const String &filePath) {
    unsigned long location = filePath.find_last_of('/');
    return (location == std::string::npos ? filePath : filePath.substr(location + 1, filePath.size() - location));
  }

  /*
   * Returns whether the path is a folder (and implicitly we can read it) or not
   */
  static bool isFolder(const String &path) {
    struct stat sb;
    return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
  }

  /*
   * Returns whether the path is a file (and implicitly we can read it) or not
   */
  static bool isFile(const String &path) {
    struct stat sb;
    return stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode);
  }

  /**
   * inspect uri and return the longest path that is a real file/directory
   */
  static String getActualPath(const String &uri) {
    std::vector<String> folders = StringUtils::split(uri, '/');

    String path = uri;
    while (folders.size() != 0) {
      if (isFile(path)) {
        return path;
      }
      folders.pop_back();
      path = StringUtils::join(folders, '/');
    }

    return uri;
  }
private:

};
