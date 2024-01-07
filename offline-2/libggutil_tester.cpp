#include <bits/stdc++.h>
using namespace std;

#include "libggutil.hpp"

int main()
{
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
