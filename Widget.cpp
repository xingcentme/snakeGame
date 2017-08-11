#include "Widget.h"
#include<QPainter>
#include<QPen>
#include<QDebug>
#include<QBrush>
#include<QThread>
#include<QApplication>
#include<QMediaPlaylist>
#include<QMediaContent>

// =============================================================================
// 构造器
Widget::Widget(QWidget *parent)
    : QWidget(parent),  m_gbox(this),m_bgm(this),m_vol(Qt::Horizontal,this),m_music(this),
      m_score(this), m_timer(this), m_lcdScore(4,this)
{
    setFixedSize(600,500);          // 设置固定的大小
    initialMList();                 // 初始化蛇身
    getFood();                      //初始化一个食物
    initialBlock();                 // 初始化障碍物
    initialUi();                    // 生成界面
    initialGroupBox();              // 初始化 groupbox
    initialBGM();
    setFocus();                     // 获取焦点
    setSpeed(800);                  //设置初始的速度是800
    m_timer.start(getSpeed());       // 设置开始速度
    setConnect();                    // 练接响应函数

}

Widget::~Widget()
{

}


//=======================================================
// 绘制蛇身
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);                     //获取一个画笔
    painter.setViewport(10,10,380,470);         // 设置绘制视口
    painter.setWindow(0,470,380,-470);          // 设置绘制的窗口
    painter.fillRect(0,470,380,-470,Qt::white); //填充一个矩形 颜色为白色

    painter.setPen(Qt::red);                    // 设置画笔
    for( int i= 0 ; i< m_block.count() ; ++i)   // 绘制障碍物
    {
        painter.drawPoint(m_block[i]);
    }

    painter.setPen(Qt::blue);                   // 重新设置画笔
    painter.drawRect(m_food);                   // 绘制食物

    painter.setBrush(Qt::SolidPattern);         //设置刷子

    if(!m_list.isEmpty())                       //绘制蛇身
    {
        for( int i = 0 ; i< m_list.count(); ++i)
        {
            painter.drawRect(m_list[i]);

        }
    }

    painter.end();                              //结束绘图


    QWidget::paintEvent(event);

}


//=======================================================
// ===按键响应函数
void Widget::keyPressEvent(QKeyEvent *event)
{
    auto ke = event->key();                     // 获取按键

    switch(ke)                                  // 判断按键
    {
    case Qt::Key_Up:                            // 方向向上键被按下
    {
        if(direction != Qt::Key_Down)           // 判断当前的方向是否和当前的按键方向是否相反
                                                // 如果当前的按键的方向和当前的方向是相反的则不做响应
        {
            direction = Qt::Key_Up;

            auto ae = m_list.first();           // 获取蛇身的头部
            m_list.push_front(QRect(ae.x(),ae.y()+10,10,10));   // 将蛇身相对向前移动一个方块
            m_list.removeLast();                // 去除蛇身的最后一个方块
                                                // 完成了上述 就相当于蛇身向前移动了一个位置
            update();                           // 强制更新绘图
        }
    } break;
    case Qt::Key_Left:
    {
        if(direction!=Qt::Key_Right)
        {
            direction = Qt::Key_Left;
            auto ae = m_list.first();
            m_list.push_front(QRect(ae.x()-10,ae.y(),10,10));
            m_list.removeLast();
            update();
        }
    } break;

    case Qt::Key_Right:
    {
        if(direction!=Qt::Key_Left)
        {
            direction = Qt::Key_Right;
            auto ae = m_list.first();
            m_list.push_front(QRect(ae.x()+10,ae.y(),10,10));
            m_list.removeLast();
            update();
        }
    } break;

    case Qt::Key_Down:
    {
        if(direction!= Qt::Key_Up)
        {
            direction = Qt::Key_Down;

            //  qDebug()<<" up press Key_Down ";
            auto ae = m_list.first();
            m_list.push_front(QRect(ae.x(),ae.y()-10,10,10));
            m_list.removeLast();
            update();
        }
    } break;

    case Qt::Key_Space:                         // 空格键被按下
    {
        if( m_timer.isActive())                // 判断当前是否在active状态
        {
            m_timer.stop();                    // 停止蛇身的移动
            setToolTip("暂停游戏");
        } else
        {
            setToolTip("");
            m_timer.start(getSpeed());        //恢复原来的速度
        }
    } break;

    default :
    {

    }break;
    }
    QWidget::keyPressEvent( event);         //调用父类的同名函数

}

void Widget::focusOutEvent(QFocusEvent *event)
{

    QWidget::focusOutEvent(event);
}

void Widget::focusInEvent(QFocusEvent *event)
{
    qDebug()<<" focusEvent();";
    if(sender() == &m_gbox)
    {
        m_timer.stop();;
    }

    QWidget::focusOutEvent(event);
}


//===================================================================
//===== 初始化界面
void Widget::initialUi()
{

    m_score.setGeometry(400,20,100,40);                     //标签位置
    m_score.setText("分数");
    m_lcdScore.setGeometry(400,80,100,40);
    m_music.setGeometry(400,200,70,40);
     m_vol.setGeometry(470,200,60,10);
    m_vol.setRange(0,100);
    m_music.setText("背景音乐");

    m_lcdScore.display(static_cast<int>(score));            //显示初始化的分数
}


//==========================================================================
//========设置速度 。。。。值越小速度越大
void Widget::setSpeed(int sp)
{
    speed= sp;
}

//======================================================================
//=== 初始化障碍物
void Widget::initialBlock()
{
    for( int i = 0 ; i < 380; ++i)
    {
        m_block.append(QPoint(i,0));
        m_block.append(QPoint(i,470));
    }
    for( int i = 0 ; i< 470; ++i)
    {
        m_block.append(QPoint(380,i));
        m_block.append(QPoint(0,i));
    }
}

//==========================================================================
//============随机获取食物 ps 食物有可能出现在蛇身上
void Widget::getFood()
{
    std::uniform_int_distribution <> x(10,370);
    std::uniform_int_distribution<>y(10,460);
    std::uniform_int_distribution<>color(1,469);
    m_food = QRect((x(e)/10)*10+10,(y(e)/10)*10+10,10,10);
    // qDebug()<<"get Food"<< m_food;

}

//==========================================================================
//===== 判断食物是否被吃了
bool Widget::isByEat()
{
    bool ret = false;
    if(m_food.x() == m_list.first().x() && m_food.y() == m_list.first().y()  )
    {
        ret = true;
        switch (direction) {
        case Qt::Key_Up:
        {
            m_list.push_front(QRect(m_food.x(),m_food.y()+10,10,10));

        }break;
        case Qt::Key_Down:
        {
            m_list.push_front(QRect(m_food.x(),m_food.y()-10,10,10));

        }break;
        case Qt::Key_Right:
        {
            m_list.push_front(QRect(m_food.x()+10,m_food.y(),10,10));

        }break;
        case Qt::Key_Left:
        {
            m_list.push_front(QRect(m_food.x()-10,m_food.y(),10,10));

        }break;

        default:
            break;
        }
    }

    return ret;
}


//========================================================
//============= 判断是否游戏结束
bool Widget::isDead()
{
    bool ret =false;

    auto ae = m_list.first();

    if( ae.y() == 0 || ae.y() == 470)
    {
        for( int i = 0 ; i< 380; ++i)
        {
            if(i == ae.x() || ae.x() ==i)
            {
                ret = true;
                return ret;
            }
        }
    }


    if( ae.x() == 0 || ae.x() == 380)
    {
        for( int i = 0 ; i< 470; ++i)
        {
            if( ae.y() == i || ae.y() == i)
            {
                ret =true;
                return ret;
            }
        }
    }
    return ret;
}


//===========================================================================
//===初始化groupBox
void Widget::initialGroupBox()
{
    m_gbox.setGeometry(400,300,200,90);
    m_gbox.setTitle("速度选择");
    mSpeed1.setParent(&m_gbox);
    mSpeed2.setParent(&m_gbox);
    mSpeed3.setParent(&m_gbox);
    mSpeed4.setParent(&m_gbox);
    mSpeed1.setGeometry(10,20,80,40);
    mSpeed2.setGeometry(100,20,80,40);
    mSpeed3.setGeometry(10,50,80,40);
    mSpeed4.setGeometry(100,50,80,40);
    mSpeed1.setChecked(true);
    mSpeed1.setText("慢速");
    mSpeed2.setText("中速");
    mSpeed3.setText("快速");
    mSpeed4.setText("极速");

}


//===========================================================================
//============初始化蛇身
void Widget::initialMList()
{
    if(!m_list.isEmpty())
    {
        m_list.clear();
    }
    m_list.append({QRect(10,10,10,10),QRect(10,20,10,10),QRect(10,30,10,10),QRect(10,40,10,10),});

}


// ==================================================================
// ====================返回速的的值
int Widget::getSpeed()
{
    return speed;
}

void Widget::setConnect()
{
    connect(&mSpeed1,SIGNAL(toggled(bool)),this,SLOT(speed1on()));
    connect(&mSpeed2,SIGNAL(toggled(bool)),this,SLOT(speed2on()));
    connect(&mSpeed3,SIGNAL(toggled(bool)),this,SLOT(speed3on()));
    connect(&mSpeed4,SIGNAL(toggled(bool)),this,SLOT(speed4on()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    connect(&m_gbox,SIGNAL(toggled(bool)),this,SLOT(checkBoxChange()));
    connect(&m_music,SIGNAL(toggled(bool)),this,SLOT(onBGM()));
    connect(&m_vol,SIGNAL(valueChanged(int)),this,SLOT(onValueChange(int)));
}


// =================================================================
//===============重新开始游戏
void Widget::restartGame()
{
    score=0;                                    // 将分数值为0
    m_lcdScore.display(static_cast<int>(score));//强制刷新显示的分数
    initialMList();                              //初始化蛇身
    direction= Qt::Key_Up;                      //重置方向
}

void Widget::initialBGM()
{

    auto filePath=QApplication::applicationDirPath()+QString("/bgm.mp3");
    auto playList= new QMediaPlaylist(this);
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    playList->addMedia(QMediaContent(QUrl(filePath)));
    m_bgm.setPlaylist(playList);
    qDebug()<<"playList    ： "<<playList;
}

void Widget::setVol(int vol)
{

}


//================================================================
//=============计时器的时间用完的响应函数
void Widget::onTimeOut()
{
    auto me = m_list.first();
    switch (direction) {
    case Qt::Key_Up:
    {
        m_list.push_front(QRect(me.x(),me.y()+10,10,10));
        m_list.removeLast();
    }

        break;
    case Qt::Key_Down:
    {
        m_list.push_front(QRect(me.x(),me.y()-10,10,10));
        m_list.removeLast();
    } break;

    case Qt::Key_Left:
    {
        m_list.push_front(QRect(me.x()-10,me.y(),10,10));
        m_list.removeLast();
    } break;
    case Qt::Key_Right:
    {
        m_list.push_front(QRect(me.x()+10,me.y(),10,10));
        m_list.removeLast();
    } break;
    default:
        break;
    }

    if(isByEat())
    {
        getFood();
        score++;
        m_lcdScore.display(static_cast<int>(score));
    }

    if(isDead   ())
    {
        QMessageBox::critical(this,"gameOVer","游戏结束",QMessageBox::Ok);
        restartGame();
    }
    update();
}

void Widget::checkBoxChange()
{
    setFocus();
}


//============================================================================
//=========================选择速度一
void Widget::speed1on()
{
    setFocus();
    setSpeed(800);
    m_timer.start(getSpeed());
}


//============================================================================
//=========================选择速度2
void Widget::speed2on()
{
    setFocus();
    setSpeed(600);
    m_timer.start(getSpeed());
}


//============================================================================
//=========================选择速度3
void Widget::speed3on()
{
    setFocus();
    setSpeed(300);
    m_timer.start(getSpeed());
}


//============================================================================
//=========================选择速度4
void Widget::speed4on()
{
    setFocus();
    setSpeed(150);
    m_timer.start(getSpeed());

}


//=======================================================================
//==========背景音乐
void Widget::onBGM()
{
    if(m_music.isChecked())
    {

        m_bgm.play();
    }else
    {
        m_bgm.pause();
    }

    setFocus();
}


//===========================================================================
// =====重新设置音量
void Widget::onValueChange(int vol)
{
    m_bgm.setVolume(vol);
    m_vol.setToolTip(QString::number(vol));
    setFocus();
}
