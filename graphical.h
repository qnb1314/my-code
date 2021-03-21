#ifndef GRAPHICAL_H
#define GRAPHICAL_H
#include <rgbandvector.h>
#include <global.h>
#include <QFile>
#include <QImage>

//**************************************************一些求交函数*********************************************************
//平面求交函数
bool if0edge(Threevector A,Threevector N,Threevector P,Threevector Pvector)
{
    if(Pvector*N==0)return 0;

    double t=((A-P)*N)/(Pvector*N);
    if(t<error1)return 0;//表示不相交
    else return 1;//表示相交
}
double get0edge(Threevector A,Threevector N,Threevector P,Threevector Pvector)
{
    double t=((A-P)*N)/(Pvector*N);
    return t-error1;
}


//三边形求交函数
bool if3edge(Threevector A,Threevector B,Threevector C,Threevector N,Threevector P,Threevector Pvector)
{
    //先求与平面交点
    if(if0edge(A,N,P,Pvector)==0)
    {
        return  0;
    }
    //与平面的交点为
    Threevector Pp=P+Pvector*get0edge(A,N,P,Pvector);

    //再判断该点是否位于三角形内部
    //用重心法判断,我们选择相对于A点进行计算
    //判断投影在哪个面上
    Threevector Z(0,0,1);
    Threevector X(1,0,0);
    Threevector Y(0,1,0);
    if(N*Z!=0)
    {
        Pp.Z=0;
        A.Z=0;
        B.Z=0;
        C.Z=0;
    }
    else if(N*X!=0)
    {
        Pp.X=0;
        A.X=0;
        B.X=0;
        C.X=0;
    }
    else
    {
        Pp.Y=0;
        A.Y=0;
        B.Y=0;
        C.Y=0;
    }
    Threevector R=Pp-A;
    Threevector Q1=B-A;
    Threevector Q2=C-A;
    double Q12=Q1*Q1;
    double Q22=Q2*Q2;
    double Q1Q2=Q1*Q2;
    double RQ1=R*Q1;
    double RQ2=R*Q2;

    //构造计算矩阵
    matrix thismatrix1=unitzero;
    matrix thismatrix2=unitzero;
    //矩阵1
    thismatrix1.Nx=2;
    thismatrix1.Ny=2;
    thismatrix1.M[1][1]=Q22;
    thismatrix1.M[1][2]=-1*Q1Q2;
    thismatrix1.M[2][1]=-1*Q1Q2;
    thismatrix1.M[2][2]=Q12;
    //矩阵2
    thismatrix2.Nx=2;
    thismatrix2.Ny=1;
    thismatrix2.M[1][1]=RQ1;
    thismatrix2.M[2][1]=RQ2;

    //接收矩阵
    matrix res=thismatrix1*thismatrix2*(1/(Q12*Q22-Q1Q2*Q1Q2));

    double w2=res.M[1][1];
    double w3=res.M[2][1];
    double w1=1-w2-w3;
    if(w1<0||w1>1||w2<0||w2>1||w3<0||w3>1)return 0;

    return 1;
}

double get3edge(Threevector A,Threevector ,Threevector ,Threevector N,Threevector P,Threevector Pvector)
{
    //先求与平面交点
    return get0edge(A,N,P,Pvector);
}


//**********************************************************************************************************************


//*****************************************************返回信息类********************************************************
class information
{
public:
    Threevector Rgb;
    double T;
    Threevector Normalpoint;
    Threevector Normal;
    bool ifreflex;//反射
    bool ifrefraction;//折射
    double Ks;//镜面反射常数，通过改变该值改变图像的光滑度
    double Kreflex;//反射常数,用于调整反射能力
    double Krefraction;//折射常数
    information(Threevector rgb,double t,Threevector normalpoint,Threevector normal,bool ifrfx,bool ifrft,double ks,double kfx,double kft):
        Rgb(rgb),T(t),Normalpoint(normalpoint),Normal(normal),ifreflex(ifrfx),ifrefraction(ifrft),Ks(ks),Kreflex(kfx),Krefraction(kft){}
    information():
        Rgb(zero),T(0),Normalpoint(zero),Normal(zero),ifreflex(0),ifrefraction(0),Ks(0),Kreflex(0),Krefraction(0){}
};

class ifinformation
{
public:
    bool Ifintersect;
    int N;//表示编号
    ifinformation(bool ifintersect,int n=-1):Ifintersect(ifintersect),N(n){}
};

//**********************************************************************************************************************


//*****************************************************图形类************************************************************
//抽象图形类
class graphical
{
public:
    Threevector RGB;//用于表示图形的颜色常数，衰减系数
    bool ifreflex;//反射
    bool ifrefraction;//折射
    double Ks;//镜面反射常数，通过改变该值改变图像的光滑度
    double Kreflex;//反射常数,用于调整反射能力
    double Krefraction;//折射常数
    QString* Filename;
    QImage image;

    //定义纯虚函数，实现多态
    virtual information Getintersection(Threevector ,Threevector ,int)=0;//传入单位向量以及点，返回参数t
    virtual ifinformation Ifintersect(Threevector ,Threevector )=0;//判断是否相交

    Threevector getRgb(Threevector ,int )
    {
        return RGB;
    }
    graphical(Threevector rgb,bool ifrfx,bool ifrft,double ks,double krfx,double krft,QString* filename=NULL):
        RGB(rgb),ifreflex(ifrfx),ifrefraction(ifrft),Ks(ks),Kreflex(krfx),Krefraction(krft),Filename(filename){}
};
//1.球面
class sphere:public graphical
{
public:
    Threevector centerpoint;//球心位置
    double R;//半径

    Threevector Filexvector;
    Threevector Fileyvector;
    Threevector Filezvector;

    sphere(Threevector rgb,Threevector cpoint,bool ifrfx,bool ifrft,double r,double ks,double krfx,double krft,QString* filename=NULL):
        graphical(rgb,ifrfx,ifrft,ks,krfx,krft,filename),centerpoint(cpoint),R(r)
        {
            Filexvector=Threevector(1,0,0);
            Fileyvector=Threevector(0,1,0);
            Filezvector=Threevector(0,0,1);
            if(Filename!=NULL)
            {
                if(!image.load(*Filename))
                {
                    qDebug()<<*Filename<<"未成功载入";
                }
                image.load(*Filename);
            }
        }

    Threevector getRgb(Threevector point,int )
    {
        if(Filename==NULL)
        {
            return RGB;
        }

        Threevector vector=point-centerpoint;

        vector.Unitization();
        double vectorx=vector*Filexvector;
        double vectory=vector*Fileyvector;
        double vectorz=vector*Filezvector;

        double a;
        double b=acos(vectory);
        if(-error1<vectorz<error1)
        {
            if(vectorx>0)a=3.14159/2;
            else
            {
                a=3.14159/2*3;
            }
        }
        else
        {
        a=atan(vectorx/vectorz);
        if(a<0)
        {
            a=a+3.14159;
        }
        }


        double imagex=image.width();
        double imagey=image.height();

        //进行image图片裁剪
        if(imagex>=2*imagey)
        {
            imagex=2*imagey;
        }
        else
        {
            imagey=0.5*imagex;
        }
        double f=imagex/(2*3.14159);

        b=3.14159-b;
        a=a*f;
        b=b*f;

        int aint=a;
        int bint=b;

        QColor pointcolor=image.pixelColor(aint,bint);

        Threevector resrgb;
        resrgb.X=RGB.X*pointcolor.red()/255.0;
        resrgb.Y=RGB.Y*pointcolor.green()/255.0;
        resrgb.Z=RGB.Z*pointcolor.blue()/255.0;
        return resrgb;
    }
    //*关键*球面求交点，采用几何法
    information Getintersection(Threevector onepoint,Threevector onevector,int )//传入点和单位向量
    {
        Threevector l=centerpoint-onepoint;

        double tp=l*onevector;

        double d2=l*l-tp*tp;

        double R2=R*R;

        double t=sqrt(R2-d2);
        double tt=tp-t-error1;
        Threevector Normalpoint=onepoint+onevector*tt;
        Threevector Normal=Normalpoint-centerpoint;
        Normal.Unitization();

        information res(getRgb(Normalpoint,-1),tt,Normalpoint,Normal,this->ifreflex,this->ifrefraction,this->Ks,this->Kreflex,this->Krefraction);
        return res;
    }
    ifinformation Ifintersect(Threevector onepoint,Threevector onevector)
    {

        //判断射线是否与平面有交
        Threevector l=centerpoint-onepoint;
        double tp=l*onevector;
        if(tp<error1)return ifinformation(0);

        double d2=l*l-tp*tp;
        double R2=R*R;
        if(d2>R2)return ifinformation(0);


        return ifinformation(1);//表示有交点
    }
};

//2.平面
class plane:public graphical
{
public:
    Threevector ppoint;//初始已知点
    Threevector Normal;//单位法向量
    //纹理坐标基准值
    Threevector Filex;
    Threevector Filey;
    Threevector Filepoint;

    Threevector getRgb(Threevector point,int )
    {
        if(Filename==NULL)
        {
            return RGB;
        }

        Threevector vector=point-Filepoint;
        int pointX=vector*Filex;
        if(pointX<0)pointX=-pointX;
        int pointY=vector*Filey;
        if(pointY<0)pointY=-pointY;


        int x=image.width();
        int y=image.height();

        pointX=pointX%x;
        pointY=pointY%y;


        QColor pointcolor=image.pixelColor(pointX,pointY);

        Threevector resrgb;
        resrgb.X=RGB.X*pointcolor.red()/255.0;
        resrgb.Y=RGB.Y*pointcolor.green()/255.0;
        resrgb.Z=RGB.Z*pointcolor.blue()/255.0;
        return resrgb;
    }
    plane(Threevector rgb,bool ifrfx,bool ifrft,double ks,Threevector ppoint,Threevector N,double krfx,double krft,QString* filename=NULL,Threevector filepoint=zero,Threevector filex=zero,Threevector filey=zero):
        graphical(rgb,ifrfx,ifrft,ks,krfx,krft,filename),ppoint(ppoint),Normal(N),Filex(filex),Filey(filey),Filepoint(filepoint)
        {
        if(Filename!=NULL)
        {
            if(!image.load(*Filename))
            {
                qDebug()<<*Filename<<"未成功载入";
            }
            image.load(*Filename);
        }

        }
    information Getintersection(Threevector onepoint,Threevector onevector,int )//传入点和单位向量
    {
         double t=((ppoint-onepoint)*Normal)/(onevector*Normal);
         t=t-error1;
         Threevector Normalpoint=onepoint+t*onevector;
         information res(getRgb(Normalpoint,-1),t,Normalpoint,Normal,ifreflex,ifrefraction,Ks,Kreflex,Krefraction);
         return res;
    }
    ifinformation Ifintersect(Threevector onepoint,Threevector onevector)
    {
        if(onevector*Normal==0)return ifinformation(0);
        double t=((ppoint-onepoint)*Normal)/(onevector*Normal);
        if(t<error1)return ifinformation(0);
        else return ifinformation(1);
    }

};

//3.三角形
class triangular
{
public:
    Threevector Rgb;
    Threevector Apoint;
    Threevector Bpoint;
    Threevector Cpoint; 
    Threevector Normal;

    Threevector getRgb(Threevector ,int )
    {
        return Rgb;
    }
    triangular(Threevector apoint,Threevector bpoint,Threevector cpoint):
        Rgb(zero),Apoint(apoint),Bpoint(bpoint),Cpoint(cpoint),Normal(zero)
        {
        Threevector AB=Bpoint-Apoint;
        AB.Unitization();
        Threevector AC=Cpoint-Apoint;
        AC.Unitization();
        Normal=multiplication(AB,AC);
        Normal.Unitization();
        }
    triangular():Rgb(zero),Apoint(zero),Bpoint(zero),Cpoint(zero),Normal(zero){}

    double Getintersection(Threevector onepoint,Threevector onevector)//传入点和单位向量
    {
        double t=get3edge(Apoint,Bpoint,Cpoint,Normal,onepoint,onevector);
        return t;
    }
    bool Ifintersect(Threevector onepoint,Threevector onevector)
    {
        bool res=if3edge(Apoint,Bpoint,Cpoint,Normal,onepoint,onevector);
        return res;
    }
};

//4.多面体
class Npolyhedron:public graphical
{
public:
    Threevector *P[200000];
    triangular *F[200000];
    int FNumber;
    int PNumber;
    //包围球
    double SroundR;
    Threevector SroundPoint;

    Threevector getRgb(Threevector ,int )
    {
        return RGB;
    }
    void SetSround()
    {
        Threevector respoint=zero;
        double resr=0;
        //得到包围球球心
        for(int i=0;i<=PNumber-1;i++)
        {
            respoint=respoint+*P[i];
        }
        double pnumber=PNumber;
        respoint=respoint*(1/pnumber);

        for(int i=0;i<=PNumber-1;i++)
        {
            Threevector nvector=respoint-*P[i];
            double n=nvector.Mold();
            if(n>resr)
            {
                resr=n;
            }
        }

        SroundPoint=respoint;
        SroundR=resr+error1;

    }

    void PrintSround()
    {
        qDebug()<<SroundPoint.X<<SroundPoint.Y<<SroundPoint.Z<<SroundR;
    }
    Npolyhedron(Threevector rgb, triangular *f[],int fnumber,Threevector *p[],int pnumber,bool ifrfx,bool ifrft,double ks,double krfx,double krft,QString* filename=NULL):
         graphical(rgb,ifrfx,ifrft,ks,krfx,krft,filename),FNumber(fnumber),PNumber(pnumber)
    {
        for(int i=0;i<=FNumber-1;i++)
        {
            F[i]=f[i];
        }
        for(int i=0;i<=PNumber-1;i++)
        {
            P[i]=p[i];
        }
        SetSround();
    }

    information Getintersection(Threevector onepoint,Threevector onevector,int N)//传入点和单位向量
    {
        double t=F[N]->Getintersection(onepoint,onevector);
        Threevector Normalpoint=onepoint+t*onevector;

        //这里传入的是三棱锥的色彩值而不是三角面片的色彩值
        information res(getRgb(Normalpoint,-1),t,Normalpoint,F[N]->Normal,ifreflex,ifrefraction,Ks,Kreflex,Krefraction);
        return res;
    }

    ifinformation Ifintersect(Threevector onepoint,Threevector onevector)//判断是否相交并返回编号
    {
        //先判断是否和包围球相交
        Threevector l=SroundPoint-onepoint;
        double tp=l*onevector;
        if(tp<error1)return ifinformation(0);

        double d2=l*l-tp*tp;
        double R2=SroundR*SroundR;
        if(d2>R2)return ifinformation(0);

        //如果相交，说明有可能和三角面片相交
        double mint;
        int minN=-1;
        double t;
        for(int i=0;i<=FNumber-1;i++)
        {
            if(F[i]->Ifintersect(onepoint,onevector)==1)
            {
                if(minN==-1)
                {
                    mint=F[i]->Getintersection(onepoint,onevector);
                    minN=i;
                }
                else
                {
                    t=F[i]->Getintersection(onepoint,onevector);
                    if(t<mint)
                    {
                        minN=i;
                        mint=t;
                    }
                }
            }
        }
        if(minN==-1)//说明不相交
        {
            return ifinformation(0);
        }
        else
        {
            return ifinformation(1,minN);
        }
    }

    void MoveTo(Threevector point)
    {
        Threevector change=point-SroundPoint;
        SroundPoint=SroundPoint+change;

        for(int i=0;i<=PNumber-1;i++)
        {
            *P[i]=*P[i]+change;
        }

        for(int i=0;i<=FNumber-1;i++)
        {
            F[i]->Apoint=F[i]->Apoint+change;
            F[i]->Bpoint=F[i]->Bpoint+change;
            F[i]->Cpoint=F[i]->Cpoint+change;
        }
    }

    void Setbigger(double N)
    {
        SroundR=SroundR*N;
        SroundPoint=SroundPoint*N;

        for(int i=0;i<=PNumber-1;i++)
        {
            *P[i]=*P[i]*N;
        }

        for(int i=0;i<=FNumber-1;i++)
        {
            F[i]->Apoint=F[i]->Apoint*N;
            F[i]->Bpoint=F[i]->Bpoint*N;
            F[i]->Cpoint=F[i]->Cpoint*N;
        }
    }
};



//5.立方体


//***************************************************************************************************************************************


//文件读取函数
void Fileread(triangular* face[],int &fnumber,Threevector* point[],int &pnumber,QString filename)
{
    fnumber=0;
    pnumber=0;
    QFile file(filename);

    //读取点
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QByteArray array = file.readLine();
        QString str = QString(array);
        if(str.startsWith("$",Qt::CaseSensitive))
        {
            break;
        }
        if(str.startsWith("v",Qt::CaseSensitive)&&!str.startsWith("vt",Qt::CaseSensitive)&&!str.startsWith("vn",Qt::CaseSensitive))
        {
            int m1,m2,m3,m4;
            m1=str.indexOf(" ");
            m2=str.indexOf(" ",m1+1);
            m3=str.indexOf(" ",m2+1);
            m4=str.indexOf("\n",m3+1);

            //截取空格间的字符
            QString strx=str.mid(m1+1,m2-m1-1);
            QString stry=str.mid(m2+1,m3-m2-1);
            QString strz=str.mid(m3+1,m4-m3-1);
            double x=strx.toDouble();
            double y=stry.toDouble();
            double z=strz.toDouble();

            point[pnumber]=new Threevector(x,y,z);
            pnumber++;
        }
    }
    file.close();

    //读取面
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QByteArray array = file.readLine();
        QString str = QString(array);
        if(str.startsWith("$",Qt::CaseSensitive))
        {
            break;
        }
        if(str.startsWith("f",Qt::CaseSensitive))
        {
            //要先判断是多边形格式还是三角形格式
            int m1,m2,m3,m4,m5;//表示空格和回车
            int m1t,m2t,m3t,m4t;//表示空格后第一个斜杠
            m1=str.indexOf(" ");
            m2=str.indexOf(" ",m1+1);
            m3=str.indexOf(" ",m2+1);
            m4=str.indexOf(" ",m3+1);
            m5=str.indexOf("\n");
            //当m4的值为-1时，说明是三角形格式
            if(m4==-1||(m5==m4+1))
            {
                m1t=str.indexOf("/",m1);
                m2t=str.indexOf("/",m2);
                m3t=str.indexOf("/",m3);
                //截取空格和斜杠间的字符
                QString strn1=str.mid(m1+1,m1t-m1-1);
                QString strn2=str.mid(m2+1,m2t-m2-1);
                QString strn3=str.mid(m3+1,m3t-m3-1);

                int n1=strn1.toInt();
                int n2=strn2.toInt();
                int n3=strn3.toInt();


                face[fnumber]=new triangular(*point[n1-1],*point[n2-1],*point[n3-1]);
                fnumber++;
            }
            else
            {
                m1t=str.indexOf("/",m1);
                m2t=str.indexOf("/",m2);
                m3t=str.indexOf("/",m3);
                m4t=str.indexOf("/",m4);

                //截取空格和斜杠间的字符
                QString strn1=str.mid(m1+1,m1t-m1-1);
                QString strn2=str.mid(m2+1,m2t-m2-1);
                QString strn3=str.mid(m3+1,m3t-m3-1);
                QString strn4=str.mid(m4+1,m4t-m4-1);


                int n1=strn1.toInt();
                int n2=strn2.toInt();
                int n3=strn3.toInt();
                int n4=strn4.toInt();

                face[fnumber]=new triangular(*point[n1-1],*point[n2-1],*point[n3-1]);
                fnumber++;
                face[fnumber]=new triangular(*point[n1-1],*point[n3-1],*point[n4-1]);
                fnumber++;
            }

        }
    }
    file.close();
}

#endif // GRAPHICAL_H
