#pragma once

#include "Resource.h"

class ResourceCache
{
private:
  std::map<String, Resource*> cache;
public:
  Resource * get(const String &key) {
    auto pair = cache.find(key);

    if (pair != cache.end()) {
      return pair->second;
    }

    return null;
  }

  void put(const String &key, Resource *resource) {
    if(resource != null) {
      cache[key] = resource;
    } else {
      if(contains(key)) {
        cache.erase(key);
      }
    }
  }

  void evict(const String &key) {
    put(key, null);
  }

  void evict(Resource *resource) {

  }


  bool contains(const String &key) {

  }
};
