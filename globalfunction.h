#ifndef GLOBALFUNCTION_H
#define GLOBALFUNCTION_H
#include <parameter.h>
#include <math.h>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QColor>
void setbitmap(int,int);
pointRgb IntersectColor(Threevector onepoint,Threevector onevector,int N);
double JDZ(double x);


//**********************************************光追引擎函数**************************************************************
//用于遍历bitmap，是IntersectColor()的接口函数
void setbitmap(int x1=X,int y1=Y)
{
    for(int i=0;i<=x1;i++)
    {        
        for(int j=0;j<=y1;j++)
        {
            Threevector levelx=viewrd-viewld;
            Threevector levely=viewlu-viewld;
            levelx.Unitization();
            levely.Unitization();
            Threevector levelxy=levelx*(i/fN)+levely*(j/fN);
            Threevector point1=viewld+levelxy;
            Threevector vector1=point1-viewpoint;
            vector1.Unitization();
            *bitmap[i][j]=IntersectColor(point1,vector1,0);
        }
    }

    //溢出判断，这点非常重要，因为qtrgb非常大的时候就开始乱画了
    for(int i=0;i<=x1;i++)
    {
        for(int j=0;j<=y1;j++)
        {
            int x=bitmap[i][j]->R;
            int y=bitmap[i][j]->G;
            int z=bitmap[i][j]->B;
            //采用以下插值函数，消去溢出点
            if(x>255)
            {
                bitmap[i][j]->R=255;
            }
            if(y>255)
            {
                bitmap[i][j]->G=255;
            }
            if(z>255)
            {
                bitmap[i][j]->B=255;
            }
        }
    }
}

void setpartbitmap(int i,int j)
{
    Threevector levelx=viewrd-viewld;
    Threevector levely=viewlu-viewld;
    levelx.Unitization();
    levely.Unitization();
    Threevector levelxy=levelx*(i/fN)+levely*(j/fN);
    Threevector point1=viewld+levelxy;
    Threevector vector1=point1-viewpoint;
    vector1.Unitization();
    *bitmap[i][j]=IntersectColor(point1,vector1,0);


    int x=bitmap[i][j]->R;
    int y=bitmap[i][j]->G;
    int z=bitmap[i][j]->B;
    //采用以下插值函数，消去溢出点
    if(x>255)
    {
        bitmap[i][j]->R=255;
    }
    if(y>255)
    {
        bitmap[i][j]->G=255;
    }
    if(z>255)
    {
        bitmap[i][j]->B=255;
    }
}
//IntersectColor()是计算从某一点沿着某一方向总色彩值的函数


void initmap()
{
     for(int i=0;i<=X;i++)
     {
         for(int j=0;j<=Y;j++)
         {
             bitmap[i][j]=new pointRgb(0,0,0);
         }
     }
     Flag1=1;
}


pointRgb IntersectColor(Threevector onepoint,Threevector onevector,int N)
{
    pointRgb res(0,0,0);



//第一步:遍历所有图形,判断最初交点
    //先判断是否直射向光源,如果是,计minflag为-2
    double tp=0;
    int minnumber=-1;
    information Minintersect;

    if(myset0!=NULL)
    {
        ifinformation ifintersect=myset0->Ifintersect(onepoint,onevector);
        if(ifintersect.Ifintersect==1)
        {
            Minintersect=myset0->Getintersection(onepoint,onevector,ifintersect.N);
            tp=Minintersect.T;
            minnumber=-2;
        }
    }


    for(int i=0;i<NOWNUMBER;i++)
    {
        ifinformation ifintersect=myset[i]->Ifintersect(onepoint,onevector);
        if(ifintersect.Ifintersect==1)
        {
            if(minnumber==-1)
            {
            Minintersect=myset[i]->Getintersection(onepoint,onevector,ifintersect.N);
            tp=Minintersect.T;
            minnumber=i;
            }
            else
            {
                information Nowintersect=myset[i]->Getintersection(onepoint,onevector,ifintersect.N);
                double tpi=Nowintersect.T;
                if(tp>tpi&&tpi>1e-6)
                {
                    tp=tpi;
                    minnumber=i;
                    Minintersect=Nowintersect;
                }
            }
        }
    }
    if(minnumber==-1)return res;

//第二步:计算视线图形交点的rgb值
//1.光源壳的光照计算
    if(minnumber==-2)//说明看到光源外壳
    {
        //交点和法向量计算
        Threevector Normalpoint0=Minintersect.Normalpoint;
        Threevector Normal0=Minintersect.Normal;
    //a.环境光计算
        pointRgb lowRgb=Minintersect.Rgb*ambient;

        res=res+lowRgb*(JDZ(onevector*Normal0));

    //b.强光计算
        //得到平面反射系数，1-ks得到漫反射系数
        double K=Minintersect.Ks;
        double ns=JDZ(Normal0*onevector);
        //计算漫反射比例，乘以光源得到漫反射强度
        pointRgb dreflection=(1-K)*sourceRgb*ns;
        res=res+dreflection;

    //c.间接光计算,递归实现
        //反射
        if(Minintersect.ifreflex)
        {
            if(N<Reflex)//先判断是否超出折射反射次数
            {

            Threevector Nvector=Normal0;
            if(onevector*Nvector>0)
            {
                Nvector=-1*Nvector;
            }
            Threevector reflect=onevector-2*(onevector*Nvector)*Nvector;
            reflect.Unitization();
            N++;
            pointRgb addres=IntersectColor(Normalpoint0,reflect,N)*Minintersect.Kreflex;
            res=res+addres;

            }
        }
        return res;
        }

//2.图形rgb计算

    //交点和法向量计算
    Threevector Normalpoint=Minintersect.Normalpoint;
    Threevector Normal=Minintersect.Normal;

    //a.环境光计算
    pointRgb lowRgb=Minintersect.Rgb*ambient;
    res=res+lowRgb*(JDZ(onevector*Normal));

    //b.强光计算
    //先要判断光源能否照射到该点
    Threevector thispoint=Normalpoint;//交点
    Threevector thisvector=sourcepoint-thispoint;//交点->光源向量
    thisvector.Unitization();

    //判断会不会遮挡
    int flag1=1;//表示可以到达
    double rftflag=1;//表示穿透损耗因子
    for(int i=0;i<NOWNUMBER;i++)
    {
        ifinformation ifintersect=myset[i]->Ifintersect(thispoint,thisvector);
        if(ifintersect.Ifintersect==1)
        {            
            information intersect1=myset[i]->Getintersection(thispoint,thisvector,ifintersect.N);
            //首先要考虑该物体是否位于点和光源之间
            double d=(sourcepoint-thispoint).Mold();
            if(intersect1.T<d)
            {

                //再判断该遮挡物体是否可折射
                if(intersect1.ifrefraction==1)
                {
                    rftflag=rftflag*intersect1.Krefraction;
                    continue;
                }
                //如果不能折射，说明发生了遮挡
                flag1=0;
                break;
            }     
        }
    }
    //判断无遮挡物后对镜面反射highRgb2和漫反射highRgb1进行计算
    if(flag1==1)
    {
        //**漫反射**
        //得到平面反射系数，1-ks得到漫反射系数
        double K=Minintersect.Ks;
        double ns=JDZ(Normal*thisvector);
        //计算漫反射比例，乘以光源得到漫反射强度
        Threevector highRgb1=(1-K)*Kscreen*Minintersect.Rgb*rftflag*ns;
        pointRgb dreflection=highRgb1*sourceRgb;
        res=res+dreflection;

        //**镜面反射**
        Threevector h=0.5*(thisvector+(-1)*onevector);
        double nl=JDZ(Normal*thisvector);
        double nh=JDZ(Normal*h);

        if(nh<1e-6)nh=0;
        if(nl<1e-6)nl=1;
        //计算镜面反射强度
        Threevector highRgb2=pow(nh,s)/nl*K*Minintersect.Rgb*rftflag;
        pointRgb sreflection=highRgb2*sourceRgb;


        res=res+sreflection;
    }


    //c.环境光计算
    //反射
    if(Minintersect.ifreflex)
    {
        if(N<Reflex)//先判断是否超出折射反射次数
        {

        Threevector Nvector=Normal;
        if(onevector*Nvector>0)
        {
            Nvector=-1*Nvector;
        }
        Threevector reflect=onevector-2*(onevector*Nvector)*Nvector;
        reflect.Unitization();
        N++;
        res=res+IntersectColor(thispoint,reflect,N)*Minintersect.Kreflex;

        }
    }
    //折射
    if(Minintersect.ifrefraction)
    {
        if(N<Reflex)
        {
        thispoint=thispoint+5*error1*onevector;//确保交点位于图形内部
        //寻找穿出点
        ifinformation ifintersect=myset[minnumber]->Ifintersect(thispoint,onevector);
        information intersection2=myset[minnumber]->Getintersection(thispoint,onevector,ifintersect.N);
        double tt=intersection2.T;
        Threevector thispoint2=thispoint+(tt+5*error1)*onevector;
        N++;
        res=res+IntersectColor(thispoint2,onevector,N)*Minintersect.Krefraction;
        }
    }



    //如果与任何图形都没有交点，返回000
    return res;
}
//*********************************************************************************************************************


//*************************************辅助函数***************************************
double JDZ(double x)
{
    if(x<=0)return -x;
    else return x;
}


/*Threevector viewpoint(X/2,Y/2,0);//视点
Threevector viewvector(0,0,-1);//视点方向
Threevector viewld(0,0,-f);//左下点
Threevector viewrd(X,0,-f);//右下点
Threevector viewlu(0,Y,-f);//左上点*/
void viewupmove()
{
    double T=200;
    viewpoint.Z=viewpoint.Z-T;
    viewld.Z=viewld.Z-T;
    viewrd.Z=viewrd.Z-T;
    viewlu.Z=viewlu.Z-T;
}
void viewdownmove()
{
    double T=200;
    viewpoint.Z=viewpoint.Z+T;
    viewld.Z=viewld.Z+T;
    viewrd.Z=viewrd.Z+T;
    viewlu.Z=viewlu.Z+T;
}

void viewleftmove()
{
    double T=200;
    viewpoint.X=viewpoint.X-T;
    viewld.X=viewld.X-T;
    viewrd.X=viewrd.X-T;
    viewlu.X=viewlu.X-T;
}
void viewrightmove()
{
    double T=200;
    viewpoint.X=viewpoint.X+T;
    viewld.X=viewld.X+T;
    viewrd.X=viewrd.X+T;
    viewlu.X=viewlu.X+T;
}
//***************************************************************************************

#endif // GLOBALFUNCTION_H
