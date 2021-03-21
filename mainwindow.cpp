#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QColor>
#include <qpainter.h>
#include <QKeyEvent>
#include <QDialog>
#include <globalfunction.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(X,Y);
    setWindowTitle("光线追踪");

    //初始化图形空间
    Setmyset1();

    initmap();

    //载入图形参数
    dlg=new QDialog(this);//定义对话框的对象
    dlg->resize(200,100);//规定对话框大小
    dlg->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
    dlg->show();//弹出对话框

    //setbitmap();

    dlg->close();
    //绘画
    repaint();

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{
      QPainter q(this);//用来把image画在window
      QPainter p;//用来画image
      //画图工具初始化
      QImage image(X+1,Y+1,QImage::Format_RGB32);
      p.begin(&image);


      //画image图
      //示例:image.setPixel(1,1,qRgb(255,255,255));
      if(Flag1==1)
      {
      for(int i=0;i<=X;i++)
      {
          for(int j=0;j<=Y;j++)
          {
              int x=bitmap[i][Y-j]->R;
              int y=bitmap[i][Y-j]->G;
              int z=bitmap[i][Y-j]->B;
              image.setPixel(i,j,qRgb(x,y,z));
          }
      }
      }

      //将image图像画在window上
      QRect target(0, 0, X, Y); //建立目标矩形，该区域是显示图像的目的地
      QRect source(0.0, 0.0, X, Y);
      q.drawImage(target,image,source);

      p.end();
      q.end();
      image.save("../001.jpg");//将结果图保存

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Left)
    {
        viewleftmove();
        dlg->show();//弹出对话框

        setbitmap();

        dlg->close();
        this->repaint();
    }
    if(event->key()==Qt::Key_Up)
    {
        viewupmove();
        dlg->show();//弹出对话框

        setbitmap();

        dlg->close();
        this->repaint();
    }
    if(event->key()==Qt::Key_Right)
    {
        viewrightmove();
        dlg->show();//弹出对话框

        setbitmap();

        dlg->close();
        this->repaint();
    }
    if(event->key()==Qt::Key_Down)
    {
        viewdownmove();
        dlg->show();//弹出对话框

        setbitmap();

        dlg->close();
        this->repaint();

    }
    if(event->key()==Qt::Key_Space)
    {
        dlg->show();//弹出对话框
        for(int i=0;i<=X;i++)
        {
            for(int j=0;j<=Y;j++)
            {
                setpartbitmap(i,j);
            }
            this->repaint();
        }
        dlg->close();
    }
}
