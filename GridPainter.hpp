//
// Created by anton on 17.09.18.
//

#ifndef QT_GRIDPAINTER_HPP
#define QT_GRIDPAINTER_HPP

#include <QWidget>

class Life : public QWidget
{
Q_OBJECT
private:
  int _width = 1080;
  int _height = 720;
  int g_width = 0;
  int g_height = 0;
  int* grid = nullptr;
  bool is_avaliable = true;
  double x_step = 0.0, y_step  = 0.0;
  void step();
public:
  Life(QWidget *parent = nullptr);
  void setGrid( int width, int height );
  void setPoint( int x, int y ) { grid[x+y*g_width] = 1;}
  void nextStep();
protected:
  void paintEvent(QPaintEvent *event) override;
};


#endif //QT_GRIDPAINTER_HPP
