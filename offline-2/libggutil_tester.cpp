#include <bits/stdc++.h>
using namespace std;

#include "libggutil.hpp"

int main()
{
    Line l1(0, 2);
    Line l2(1, 1);
    cout<<l1.get_intersection(l2)<<endl;

    LineSegment ls1(Point2d(2, 2), Point2d(4, 2));
    LineSegment ls2(Point2d(6, 7), Point2d(10, 11));
    cout<<ls1.get_intersection(ls2)<<endl;

    cout<<ls1.get_intersection(l2)<<endl;
    
    SquareMatrix m1(4);
    cout<<m1<<endl;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            m1.values[i][j] = i*4+j+1;
    //m1.values[2][2] = 8;
    m1.values[3][3] = 14;
    m1.values[2][2] = 10;
    cout<<m1<<endl;
    cout<<m1.determinant()<<endl;
    cout<<m1.inverse()<<endl;

    Matrix m2(4, 1);
    cout<<m2<<endl;
    for(int i=0;i<4;i++)
        m2.values[i][0] = i+1;
    cout<<m2<<endl;
    Matrix m3 = m1.matmul(m2);
    cout << m3 << endl;
    return 0;
}
