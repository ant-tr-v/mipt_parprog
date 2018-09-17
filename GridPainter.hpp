//
// Created by anton on 17.09.18.
//

#ifndef QT_GRIDPAINTER_HPP
#define QT_GRIDPAINTER_HPP

#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <chrono>

class Life : public QWidget
{
Q_OBJECT
private:
  int win_width = 720;
  int win_height = 720;
  int g_width = 0;
  int g_height = 0;
  int* grid[2] = {nullptr, nullptr};
  int active_index = 0;
  bool is_avaliable = true;
  double x_step = 0.0, y_step = 0.0;
  void step();
  long time_delta_in_frame = 2; //mks
  long time_delta_beween_frames = 2;
  std::chrono::time_point<std::chrono::high_resolution_clock> last_frame = std::chrono::high_resolution_clock::now();
public:
  Life(QWidget *parent = nullptr);
  void setGrid(int width, int height);
  void setPoint(int x, int y) { grid[active_index][x + y*g_width] = 1; }

  int getGridWidht() { return g_width; }
  int getGridHeight() { return g_height; }
public slots:
  void nextStep();
protected:
  void paintEvent(QPaintEvent *event) override;
};



#endif //QT_GRIDPAINTER_HPP
