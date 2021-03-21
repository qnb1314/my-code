#ifndef RGBANDVECTOR_H
#define RGBANDVECTOR_H
#include <math.h>
#include <QDebug>

//1.RGB点类
class pointRgb
{
public:
    int R;
    int G;
    int B;
    pointRgb(int r,int g,int b):R(r),G(g),B(b){}

    friend pointRgb operator*(pointRgb,pointRgb);
    friend pointRgb operator*(pointRgb,double);
    friend pointRgb operator*(double,pointRgb);
    friend pointRgb operator+(pointRgb,pointRgb);
};
pointRgb operator*(pointRgb a,pointRgb b)
{
    pointRgb res(a.R*b.R,a.G*b.G,a.B*b.B);
    return res;
}
pointRgb operator+(pointRgb a,pointRgb b)
{
    pointRgb res(a.R+b.R,a.G+b.G,a.B+b.B);
    return res;
}
pointRgb operator*(pointRgb a,double b)
{
    int x=a.R*b;
    int y=a.G*b;
    int z=a.B*b;
    pointRgb res(x,y,z);
    return res;
}
pointRgb operator*(double b,pointRgb a)
{
    int x=a.R*b;
    int y=a.G*b;
    int z=a.B*b;
    pointRgb res(x,y,z);
    return res;
}


//2.向量类,点类
class Threevector
{
public:
    double X;
    double Y;
    double Z;
    Threevector(double x,double y,double z):X(x),Y(y),Z(z){}
    Threevector():X(0),Y(0),Z(0){}
    void Unitization()
    {
        double s=sqrt(X*X+Y*Y+Z*Z);
        X=X/s;
        Y=Y/s;
        Z=Z/s;
    }
    double Mold()
    {
        double res=sqrt(X*X+Y*Y+Z*Z);
        return res;
    }
    friend double operator*(Threevector,Threevector);
    friend Threevector operator*(double,Threevector);
    friend Threevector operator*(Threevector,double);
    friend Threevector operator-(Threevector,Threevector);
    friend Threevector operator+(Threevector,Threevector);
};
double operator*(Threevector a,Threevector b)
{
    double res=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return res;
}
Threevector operator*(double a,Threevector b)
{
    Threevector res(a*b.X,a*b.Y,a*b.Z);
    return res;
}
Threevector operator*(Threevector b,double a)
{
    Threevector res(a*b.X,a*b.Y,a*b.Z);
    return res;
}
Threevector operator-(Threevector a,Threevector b)
{
    Threevector res(a.X-b.X,a.Y-b.Y,a.Z-b.Z);
    return res;
}
Threevector operator+(Threevector a,Threevector b)
{
    Threevector res(a.X+b.X,a.Y+b.Y,a.Z+b.Z);
    return res;
}

Threevector multiplication(Threevector A,Threevector B)
{
    Threevector res(0,0,0);
    res.X=A.Y*B.Z-A.Z*B.Y;
    res.Y=A.Z*B.X-A.X*B.Z;
    res.Z=A.X*B.Y-A.Y*B.X;
    return res;
}

//颜色与向量相乘函数
pointRgb operator*(Threevector a,pointRgb b)
{
    int x=a.X*b.R;
    int y=a.Y*b.G;
    int z=a.Z*b.B;
    pointRgb res(x,y,z);
    return res;
}

//定义零向量，用于初始化
Threevector zero(0,0,0);



//3.矩阵类,在这里采用最大为四维即可
class matrix
{
public:
    int Nx;//矩阵的行数
    int Ny;//矩阵的列数
    double M[5][5];
    matrix(int nx,int ny,double m[5][5]):Nx(nx),Ny(ny)
    {
        //初始化
        for(int i=0;i<=4;i++)
        {
            for(int j=0;j<=4;j++)
            {
                M[i][j]=0;
            }
        }
        for(int i=1;i<=nx;i++)
        {
            for(int j=1;j<=ny;j++)
            {
                M[i][j]=m[i][j];
            }
        }
    }

    friend matrix operator *(matrix m1,matrix m2);
    friend matrix operator *(double x,matrix m);
    friend matrix operator *(matrix m,double x);
};
double Mzero[5][5]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
matrix unitzero(4,4,Mzero);
matrix operator *(matrix m1,matrix m2)
{
    if(m1.Ny!=m2.Nx)
    {
        qDebug()<<"矩阵相乘行列不对应，错误";
        return unitzero;
    }
    double m[5][5];
    //初始化
    for(int i=0;i<=4;i++)
    {
        for(int j=0;j<=4;j++)
        {
            m[i][j]=0;
        }
    }
    //计算乘积
    for(int i=1;i<=m1.Nx;i++)
    {
        for(int j=1;j<=m2.Ny;j++)
        {
            for(int num=1;num<=m1.Ny;num++)
            {
                m[i][j]+=m1.M[i][num]*m2.M[num][j];
            }
        }
    }
    matrix res(m1.Nx,m2.Ny,m);
    return res;
}
matrix operator *(double x,matrix m1)
{
    double m[5][5];
    //初始化
    for(int i=0;i<=4;i++)
    {
        for(int j=0;j<=4;j++)
        {
            m[i][j]=0;
        }
    }
    for(int i=1;i<=m1.Nx;i++)
    {
        for(int j=1;j<=m1.Ny;j++)
        {
            m[i][j]=m1.M[i][j]*x;
        }
    }
    matrix res(m1.Nx,m1.Ny,m);
    return res;
}
matrix operator *(matrix m1,double x)
{
    double m[5][5];
    //初始化
    for(int i=0;i<=4;i++)
    {
        for(int j=0;j<=4;j++)
        {
            m[i][j]=0;
        }
    }
    for(int i=1;i<=m1.Nx;i++)
    {
        for(int j=1;j<=m1.Ny;j++)
        {
            m[i][j]=m1.M[i][j]*x;
        }
    }
    matrix res(m1.Nx,m1.Ny,m);
    return res;
}

#endif // RGBANDVECTOR_H
