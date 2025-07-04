#ifndef UTIL_H__
#define UTIL_H__

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "helpStructures/GeoMathHelper.h"
#include "node/communication/routing/VirtualRouting.h"
#include <iterator>
#include "helpStructures/GlobalLocationService.h"
using namespace std;

class Util {
public:
  static string intVectorToString(vector<int> xs) {
    stringstream res;
    copy(xs.begin(), xs.end(), ostream_iterator<int>(res, " "));

    return res.str();
  }

  static vector<int> stringToIntVector(string s) {
    std::stringstream iss(s);

    int number;
    std::vector<int> res;
    while (iss >> number)
      res.push_back(number);

    return res;
  }
};
#endif
