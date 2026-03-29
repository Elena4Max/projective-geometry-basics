#pragma once
#include "vec.h"
#include <cmath>

struct Mat3 {
    double m[3][3]{};

    static Mat3 identity() {
        return {{
            {1,0,0},
            {0,1,0},
            {0,0,1}
        }};
    }

    Vec3 operator*(const Vec3& v) const {
        return {
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
        };
    }

    Mat3 operator*(const Mat3& o) const {
        Mat3 r{};
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                for(int k=0;k<3;k++)
                    r.m[i][j] += m[i][k]*o.m[k][j];
        return r;
    }

    Mat3 transpose() const {
        Mat3 t{};
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                t.m[i][j] = m[j][i];
        return t;
    }

    double det() const {
        return
        m[0][0]*(m[1][1]*m[2][2]-m[1][2]*m[2][1]) -
        m[0][1]*(m[1][0]*m[2][2]-m[1][2]*m[2][0]) +
        m[0][2]*(m[1][0]*m[2][1]-m[1][1]*m[2][0]);
    }

    Mat3 inverse() const {
        Mat3 r{};
        double d = det();

        r.m[0][0] = (m[1][1]*m[2][2]-m[1][2]*m[2][1])/d;
        r.m[0][1] = (m[0][2]*m[2][1]-m[0][1]*m[2][2])/d;
        r.m[0][2] = (m[0][1]*m[1][2]-m[0][2]*m[1][1])/d;

        r.m[1][0] = (m[1][2]*m[2][0]-m[1][0]*m[2][2])/d;
        r.m[1][1] = (m[0][0]*m[2][2]-m[0][2]*m[2][0])/d;
        r.m[1][2] = (m[0][2]*m[1][0]-m[0][0]*m[1][2])/d;

        r.m[2][0] = (m[1][0]*m[2][1]-m[1][1]*m[2][0])/d;
        r.m[2][1] = (m[0][1]*m[2][0]-m[0][0]*m[2][1])/d;
        r.m[2][2] = (m[0][0]*m[1][1]-m[0][1]*m[1][0])/d;

        return r;
    }
};