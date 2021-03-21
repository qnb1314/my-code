#ifndef PARAMETER_H
#define PARAMETER_H
#include <math.h>
#include<qdebug.h>
#include <graphical.h>

//**************************1.像素参数****************************
const int X=1200;//屏幕宽,也指横向像素点数量
const int Y=900;//屏幕高
//投影面像素集
pointRgb* bitmap[X+1][Y+1];
//**************************************************************


//**************************2.视角参数****************************
const double viewf=2.718281;//视距大小，设为e
const double viewangle=1.732;//视角一半的正切值
const double viewX=viewf*viewangle*2;
const double viewY=viewf*viewangle*Y/X*2;

//视角位置
//零点初始位置
/*
Threevector viewpoint(viewX/2,viewY/2,0);//视点
Threevector viewvector(0,0,-1);//视点方向
Threevector viewld(0,0,-viewf);//左下点
Threevector viewrd(viewX,0,-viewf);//右下点
Threevector viewlu(0,viewY,-viewf);//左上点
*/

//高视角位置
Threevector viewpoint(viewX/2+X-viewX/2,viewY/2+1*Y-viewY/2,-800);//视点
Threevector viewvector(0,0,-1);//视点方向
Threevector viewld(0+X-viewX/2,0+1*Y-viewY/2,-viewf-800);//左下点
Threevector viewrd(viewX+X-viewX/2,0+1*Y-viewY/2,-viewf-800);//右下点
Threevector viewlu(0+X-viewX/2,viewY+1*Y-viewY/2,-viewf-800);//左上点


const double fN=X/viewX;//投影面放大系数,用于做像素点映射变换
//***************************************************************


//**************************3.光照参数****************************
const int Reflex=4;//允许反射折射次数
const double Kscreen=0.5;//场景漫反射常数，用于调整漫反射物体亮度

//定义环境光强
const int Ambientcolor=150;//用于调整场景亮度
const double s=10;//高光指数，一般不进行更改，容易造成溢出
//定义环境光
pointRgb ambient(Ambientcolor,Ambientcolor,Ambientcolor);


//**************************4.光源参数****************************
Threevector sourcepoint(X,Y,-X);
pointRgb sourceRgb(255,255,255);
//***************************************************************




//*******************************图形空间实例化(参数/函数)***************************************
//x:0-X,y:0-Y//空间图形参数
const int NUMBER=100;//空间内图形总数
int NOWNUMBER=0;

//图形集合
graphical* myset[NUMBER];
graphical* myset0;//指向光源图形体


//通用参数表:
//ks->镜面反射系数,krfx反射指数,krft折射指数,ifrfx是否反射,ifrft是否折射,rgb颜色,
void Setmyset0()
{
    sourcepoint.X=X;
    sourcepoint.Y=1.5*Y;
    sourcepoint.Z=-1*X;
    //0.id0-灯罩
    Threevector id0rgb(1,1,1);//光源图形颜色
    Threevector id0centerpoint=sourcepoint;//球心位置
    double id0R=50;//半径大小
    myset0=new sphere(id0rgb,id0centerpoint,0,0,id0R,0.3,0,0);

    //1.id1-球面
    Threevector id1rgb(0.16*4,0.238*4,0.255*4);//衰减系数
    Threevector id1centerpoint(1200,100,-2100);//球心位置
    double id1R=100;//半径大小
    myset[NOWNUMBER]=new sphere(id1rgb,id1centerpoint,1,1,id1R,0.9,0.05,0.05);
    NOWNUMBER++;

    //2.id2-球面
    Threevector id2rgb(0.244*4,0.096*4,0.108*4);//衰减系数
    Threevector id2centerpoint(300,200,-2000);//球心位置
    double id2R=200;//半径大小
    myset[NOWNUMBER]=new sphere(id2rgb,id2centerpoint,1,1,id2R,0.9,0.05,0.05);
    NOWNUMBER++;

    //3.id3-平面-底面
    Threevector id3rgb(1,1,1);//衰减系数
    Threevector id3point(0,0,0);//参照点
    Threevector id3vector(0,1,0);
    myset[NOWNUMBER]=new plane(id3rgb,1,0,0,id3point,id3vector,0.3,0.8);
    NOWNUMBER++;

    //4.id4-平面-左面
    Threevector id4rgb(1,0.48,0.54);//衰减系数
    Threevector id4point(0,0,0);//参照点
    Threevector id4vector(1,0,0);
    myset[NOWNUMBER]=new plane(id4rgb,0,0,0,id4point,id4vector,0.8,0.8);
    NOWNUMBER++;

    //5.id5-平面-前面
    Threevector id5rgb(1,1,1);//衰减系数
    Threevector id5point(0,0,0);//参照点
    Threevector id5vector(0,0,1);
    myset[NOWNUMBER]=new plane(id5rgb,0,0,0,id5point,id5vector,0.3,0.8);
    NOWNUMBER++;

    //6.id6-平面-右面
    Threevector id6rgb(0.12,1,0.86);//衰减系数
    Threevector id6point(2*X,0,0);//参照点
    Threevector id6vector(-1,0,0);
    myset[NOWNUMBER]=new plane(id6rgb,0,0,0,id6point,id6vector,0.8,0.8);
    NOWNUMBER++;


    //7.id7-平面-后面
    Threevector id7rgb(1,1,1);//衰减系数
    Threevector id7point(0,0,-2*X);//参照点
    Threevector id7vector(0,0,1);
    myset[NOWNUMBER]=new plane(id7rgb,1,0,0,id7point,id7vector,0.3,0.8);
    NOWNUMBER++;

    //8.id8-平面-顶面
    Threevector id8rgb(1,1,1);//衰减系数
    Threevector id8point(0,2*Y,0);//参照点
    Threevector id8vector(0,1,0);
    myset[NOWNUMBER]=new plane(id8rgb,0,0,0,id8point,id8vector,0.3,0.8);
    NOWNUMBER++;

    //9.id9-玻璃球
    Threevector id9rgb(0,0,0);//衰减系数
    Threevector id9centerpoint(600,50,-1600);//球心位置
    double id9R=50;//半径大小
    myset[NOWNUMBER]=new sphere(id9rgb,id9centerpoint,1,1,id9R,1,0.55,0.25);
    NOWNUMBER++;

    //10.id10-三棱锥
    Threevector id10rgb(0.16*4,0.238*4,0.255*4);
    Threevector *point[4];
    point[0]=new Threevector(1200+800,500,-1200-800);
    point[1]=new Threevector(1000+800,0,-1400-800);
    point[2]=new Threevector(1000+800,0,-1000-800);
    point[3]=new Threevector(1400+800,0,-1000-800);
    triangular *face[4];
    face[0]=new triangular(*point[1],*point[2],*point[3]);
    face[1]=new triangular(*point[0],*point[2],*point[3]);
    face[2]=new triangular(*point[0],*point[1],*point[3]);
    face[3]=new triangular(*point[0],*point[1],*point[2]);
    myset[NOWNUMBER]=new Npolyhedron(id10rgb,face,4,point,4,0,0,0.9,0.05,0.05);
    NOWNUMBER++;


    /*
    //11.id11-复杂图形
    triangular *face2[200000];
    int fnumber2=0;
    Threevector *point2[200000];
    int pnumber2=0;
    Fileread(face2,fnumber2,point2,pnumber2,":/3d/obj/tree.txt");
    Threevector id11rgb(0.16*4,0.238*4,0.255*4);
    Npolyhedron *id2=new Npolyhedron(id11rgb,face2,fnumber2,point2,pnumber2,0,0,0.9,0.05,0.05);
    Threevector flag(1000,150,-1500);
    id2->Setbigger(150);
    id2->MoveTo(flag);
    id2->PrintSround();
    myset[NOWNUMBER]=id2;
    NOWNUMBER++;
    */
}

void Setmyset1()
{

    sourcepoint.X=X;
    sourcepoint.Y=1.5*Y;
    sourcepoint.Z=-0.5*X;
    //0.id0-灯罩
    Threevector id0rgb(1,1,1);//光源图形颜色
    Threevector id0centerpoint=sourcepoint;//球心位置
    double id0R=50;//半径大小
    myset0=new sphere(id0rgb,id0centerpoint,0,0,id0R,0.45,0,0);


    //3.id3-平面-底面
    Threevector id3rgb(1,1,1);//衰减系数
    Threevector id3point(0,0,0);//参照点
    Threevector id3vector(0,1,0);
    myset[NOWNUMBER]=new plane(id3rgb,1,0,0,id3point,id3vector,0.3,0.8);
    NOWNUMBER++;


    /*
    //4.id4-平面-左面
    Threevector id4rgb(1,1,1);//衰减系数
    Threevector id4point(0,0,0);//参照点
    Threevector id4vector(1,0,0);

    Threevector filepoint4(0,0,1800);
    Threevector filex4(0,0,-1);
    Threevector filey4(0,-1,0);

    QString* filename4=new QString(":/3d/obj/background/bizhi3.jpg");

    myset[NOWNUMBER]=new plane(id4rgb,0,0,0,id4point,id4vector,0.8,0.8,filename4,filepoint4,filex4,filey4);
    NOWNUMBER++;

    //5.id5-平面-前面
    Threevector id5rgb(1,1,1);//衰减系数
    Threevector id5point(0,0,0);//参照点
    Threevector id5vector(0,0,1);
    myset[NOWNUMBER]=new plane(id5rgb,0,0,0,id5point,id5vector,0.3,0.8);
    NOWNUMBER++;

    //6.id6-平面-右面
    Threevector id6rgb(1,1,1);//衰减系数
    Threevector id6point(2*X,0,0);//参照点
    Threevector id6vector(-1,0,0);

    Threevector filepoint6(2400,1800,-2400);
    Threevector filex6(0,0,1);
    Threevector filey6(0,-1,0);

    QString* filename6=new QString(":/3d/obj/background/bizhi3.jpg");
    myset[NOWNUMBER]=new plane(id6rgb,0,0,0,id6point,id6vector,0.8,0.8,filename6,filepoint6,filex6,filey6);
    NOWNUMBER++;
    */

    //7.id7-平面-后面
    Threevector id7rgb(1*166.0/255.0,1*238.0/255.0,1*225.0/255.0);//衰减系数
    Threevector id7point(0,0,-2400);//参照点
    Threevector id7vector(0,0,1);

    Threevector filepoint7(0,1800,-2400);
    Threevector filex7(1,0,0);
    Threevector filey7(0,-1,0);

    QString* filename7=new QString(":/3d/obj/background/background3.jpg");

    myset[NOWNUMBER]=new plane(id7rgb,0,0,0,id7point,id7vector,0.3,0.8,filename7,filepoint7,filex7,filey7);
    NOWNUMBER++;

    /*
    //8.id8-平面-顶面
    Threevector id8rgb(1,1,1);//衰减系数
    Threevector id8point(0,2*Y,0);//参照点
    Threevector id8vector(0,1,0);
    myset[NOWNUMBER]=new plane(id8rgb,1,0,0,id8point,id8vector,0.3,0.8);
    NOWNUMBER++;
    */






    //2.id2-复杂图形
    triangular *face2[200000];
    int fnumber2=0;
    Threevector *point2[200000];
    int pnumber2=0;
    Fileread(face2,fnumber2,point2,pnumber2,":/3d/obj/tree.txt");
    Threevector id2rgb(0.16*4,0.238*4,0.255*4);
    Npolyhedron *id2=new Npolyhedron(id2rgb,face2,fnumber2,point2,pnumber2,0,0,0.9,0.05,0.05);
    Threevector flag(1200,223.908*2,-1900);
    id2->Setbigger(40);
    id2->MoveTo(flag);
    id2->PrintSround();
    //myset[NOWNUMBER]=id2;
    //NOWNUMBER++;


    //1.id1-包围球
    Threevector id1rgb(0.16*4,0.238*4,0.255*4);//衰减系数
    Threevector id1centerpoint(1200,373.908,-1900);//球心位置
    double id1R=373.908;//半径大小
    //myset[NOWNUMBER]=new sphere(id1rgb,id1centerpoint,0,0,id1R,0.4,0.05,0.05);
    //NOWNUMBER++;


    //9.id9-球面
    Threevector id9rgb(0.244*4,0.096*4,0.108*4);//衰减系数
    Threevector id9centerpoint(500,200,-2000);//球心位置
    double id9R=200;//半径大小
    QString* filename9=new QString(":/3d/obj/background/bizhi2.jpg");
    myset[NOWNUMBER]=new sphere(id9rgb,id9centerpoint,0,0,id9R,0.9,0.05,0.05,filename9);
    NOWNUMBER++;


    //10.id10-三棱锥
    Threevector id10rgb(0.16*4,0.238*4,0.255*4);
    Threevector *point[4];
    point[0]=new Threevector(1200+800,500,-1200-800);
    point[1]=new Threevector(1000+800,0,-1400-800);
    point[2]=new Threevector(1000+800,0,-1000-800);
    point[3]=new Threevector(1400+800,0,-1000-800);
    triangular *face[4];
    face[0]=new triangular(*point[1],*point[2],*point[3]);
    face[1]=new triangular(*point[0],*point[2],*point[3]);
    face[2]=new triangular(*point[0],*point[1],*point[3]);
    face[3]=new triangular(*point[0],*point[1],*point[2]);
    myset[NOWNUMBER]=new Npolyhedron(id10rgb,face,4,point,4,0,0,0.9,0.05,0.05);
    NOWNUMBER++;
}
//***********************************************************************************
#endif // PARAMETER_H
