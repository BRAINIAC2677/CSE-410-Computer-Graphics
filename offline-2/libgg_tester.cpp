#include<bits/stdc++.h>
using namespace std;

#include "libgg.hpp"

int main()
{
  ggInit(1000, 1000);
  ggDrawTriangle(Point(0.0, 0.0, 5.0), Point(5.0, 0.0, 5.0), Point(0.0, 5.0, 5.0));
  ggPushMatrix();
  ggScale(2.0, 2.0, 2.0);
  ggDrawTriangle(Point(0.0, 0.0, 1.0), Point(5.0, 0.0, 1.0), Point(0.0, 5.0, 1.0));
  ggTranslate(10.0, 0.0, 0.0);
  ggDrawTriangle(Point(0.0, 0.0, -10.0), Point(5.0, 0.0, -10.0), Point(0.0, 5.0, -10.0));
  ggRotate(90.0, 0.0, 0.0, 1.0);
  ggDrawTriangle(Point(0.0, 0.0, 10.0), Point(5.0, 0.0, 10.0), Point(0.0, 5.0, 10.0));
  ggPopMatrix();
  ggDrawTriangle(Point(0.0, 0.0, 0.0), Point(20.0, 0.0, 0.0), Point(0.0, 20.0, 0.0));
  ggPrintTriangles(0);
  return 0;
}
