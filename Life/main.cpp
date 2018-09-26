#include <QApplication>

#include "GridPainter.hpp"

void setRandomGrid(Life& game)
{
  int h = game.getGridHeight();
  int w = game.getGridWidht();
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if ((rand() % 3) == 0)
        game.setPoint(i, j);
    }
  }
}

int main(int argc, char *argv[])
{
  srand(static_cast<unsigned int>(time(nullptr)));
  QApplication app(argc, argv);
  Life game;
  game.setGrid(1500, 1500);
  game.setNumThreads(4);
  setRandomGrid(game);


  game.show();
  return app.exec();
}