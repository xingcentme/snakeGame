/*****************************************************************************************
 *  作者: xingcent
 *  版本 1.0
 *  完成时间:2017年8月11日
 *  联系我:xingcent@hotmail.com
 *  说明 : 没有做文件配置的文件, 重开游戏默认初始化, 不做游戏的记录和用户的设置作为记录
 *         背景音乐需要添加到可执行文件的路径, 音频文件名是 bgm.mp3
 *
 *
 * **************************************************************************************/
//========================================================================================
// 重要说明 : 蛇身存储直接就是 用一个矩形来存贮的。 在蛇身移动的过程中，移动的像素就是以 10 作为
// 一个基本单位的，并且是以矩形的左上角作为判断是否碰到食物或者障碍物的准则
// 在游戏的过程中有可能会碰到一个方向上吃不了食物的操作。。。
// 判断是否碰到障碍物的过程有错误

#ifndef _WIDGET_H
#define _WIDGET_H

#include <QWidget>
#include<QCheckBox>
#include<QLabel>
#include<QKeyEvent>
#include<QPaintEvent>
#include<QList>
#include<QRect>
#include<QLCDNumber>
#include<QTimer>
#include<QRadioButton>
#include<QGroupBox>
#include <QMediaPlayer>
#include<QSlider>
#include<QTime>
#include<QMessageBox>


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
protected :
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event)override;
    void focusOutEvent(QFocusEvent *event)override;
    void focusInEvent(QFocusEvent *event)override;

 private:
    void initialUi();                                  // 初始化ui
    void setSpeed( int sp);                            // 设置速度
    void initialBlock();                               //绘制障碍物
    void getFood();                                    //初始化一个食物
    bool isByEat();                                     // 判断食物是否被吃了
    bool isDead();                                     // 判断是否游戏结束
    void initialGroupBox();                            // 初始化
    void initialMList();                               // 生成一个蛇身
    int getSpeed();                                    // 返回一个速度 越小越快
    void setConnect();                                 // 链接响应函数
    void restartGame();                                // 重新开始
    void initialBGM();                                 //初始化背景音乐
    void setVol(int vol);
    int getVol();
private slots:
    void onTimeOut();                                  // 计时器时间用完的响应
    void checkBoxChange();                             //选择速度
    void speed1on();                                    // 选择速度等级 一
    void speed2on();                                   //选择速度等级二
    void speed3on();                                   // 选择速度等级 三
    void speed4on();                                   // 选择速度等级四
    void onBGM();                                       //选择播放背景音乐
    void onValueChange(int vol);                        //音量变量

protected:
    QGroupBox m_gbox;                                   // 存放radioBox
    QRadioButton mSpeed1;                               // 存放速度一处
    QRadioButton mSpeed2;                               // 速度等级 二
    QRadioButton mSpeed3;                               //速度等级 三
    QRadioButton mSpeed4;                               // 速度等级四
    QMediaPlayer m_bgm;                                 // 背景音乐句柄
    QSlider m_vol;                                      // 背景音乐音量控制
    QRadioButton m_music;                               // 是否开启背景音乐

    QLabel m_score;                                     //显示分数 标签
    QTimer m_timer;                                     //计时器
    std::size_t score {0};                              //存贮分数
    Qt::Key direction {Qt::Key_Up,};                    // 控制方向
    QLCDNumber m_lcdScore;                              // lcd 屏幕
    QList<QPoint> m_block ;                             // 障碍物
    QRect m_food;                                       // 食物
    int speed;                                          // 移动速度
    QList<QRect> m_list;                                // 存贮蛇身
    // 随机数引擎
    std::default_random_engine e {unsigned(QTime::currentTime().msec()+QTime::currentTime().minute()),} ;

};

#endif // _WIDGET_H
