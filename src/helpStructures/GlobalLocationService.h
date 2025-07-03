#ifndef GLOBALLOCATIONSERVICE_H_
#define GLOBALLOCATIONSERVICE_H_

#include <map>
#include <vector>
#include <omnetpp.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include "GeoMathHelper.h"

using namespace std;

struct RPLSubTree {

};

class GlobalLocationService {
 private:
  static cModule *networkModule;
  static bool initialized;
  static bool rangeUpdated;
  static std::vector<Point> locations;
  static std::vector<std::vector<NeighborRecord>> neighborTables;
  static double cellWidth;
  static double cellHeight;
  static std::vector<int> numReceiveds;
  static std::vector<std::map<int, int>> colors;

  static double range;
  double range_internal;

 public:
	GlobalLocationService();

	//TODO: needs a cleaner rangeUpdate mechanism
	//instead of directly calling the static class
  static void setRange(double range_) {
      if(!rangeUpdated) {
          range= range_;
          rangeUpdated=true;
      }
  };

  static double getRange(){
      return range;
  };

  static void initialize(cModule*);
  static std::vector<NeighborRecord> getNeighborTable(int);
  static std::vector<int> getNeighborIndex(int);
  static Point getLocation(int);
  static int getId(Point point);
  static int getNumReceived(int);
  static void increaseNumReceived(int);
  static map<tuple<int, int>, int> spCache;
  static int numHopShortestPath(int source, int destination);
  static bool isInSameCell(Point A, Point B);
  static vector<Point> getCell(Point p);
  static int getColor(int current, int destination);
  static int getColor(int node);
  static void increaseColor(int current, int destination);
  static double calculatePathLength(vector<int> path);
  static double convexHullPerimeter(vector<int> polygon);
};

#endif
