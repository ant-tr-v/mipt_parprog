//
// Created by anton on 17.09.18.
//

#include "GridPainter.hpp"
#include <QtWidgets>


Life::Life(QWidget *parent)
    : QWidget(parent)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT( nextStep()));
  timer->start(10);

  setWindowTitle(("Life"));
  resize(_width, _height);
}

void Life::paintEvent(QPaintEvent *)
{

  QColor color(0, 0, 0);

  double side = qMin(width(), height());

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(width() / 2, height() / 2);
  painter.scale(side /_width, side / _height);

  painter.setPen(Qt::NoPen);
  painter.setBrush( color);

  //painter.save();
  //painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
  //painter.drawConvexPolygon(hourHand, 3);
  //painter.restore();
  int *ptr = grid;
  for (int i = 0  ; i < g_width; i++)
  {
    for (int j = 0; j < g_height; j++, ++ptr)
    {
      if (*ptr)
      {
        painter.drawEllipse(QPointF(i * x_step, j * y_step),
                            x_step * 0.4, y_step * 0.4);
      }
    }
  }
}

void Life::setGrid(int width, int height)
{
  if ( grid)
    delete[] grid;
  g_width = width;
  g_height = height;
  x_step = double(_width)/width;
  y_step = static_cast<double>(_height)/height;
  grid = new int[width*height];
  for (int i = 0; i < g_width*g_height; i++)
  {
    grid[i] = 0;
  }
}

void Life::nextStep()
{
  update();
}

void Life::step() {

}


