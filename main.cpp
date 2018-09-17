#include <QApplication>

#include "GridPainter.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Life game;
  game.setGrid(20,20);
  game.setPoint(0,0);
  game.setPoint(1,1);
  game.setPoint(3,4);
  game.setPoint(1,2);

  game.show();
  return app.exec();
}