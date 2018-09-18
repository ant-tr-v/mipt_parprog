#include "GridPainter.hpp"
#include <iostream>
#include <QtWidgets>
#include <omp.h>

/*TODO:
 * 2) assert for wrong usage
*/
Life::Life(QWidget *parent)
	: QWidget(parent)
{

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(nextStep()));
	timer->start(1);
	omp_set_num_threads(1);
	setWindowTitle(("Life"));
	resize( win_width, win_height);
}

void Life::paintEvent(QPaintEvent *)
{
	QColor color(0, 240, 0);

	double side = qMin(width(), height());
	win_width = width();
	win_height = height();

	x_step = double(win_width) / g_width;
	y_step = static_cast<double>(win_height) / g_height;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(width() / 2, height() / 2);
	painter.scale(side / win_width, side / win_height);


	painter.setBrush(color);
	painter.setPen(color);

	int *ptr = grid[active_index];

	for (int i = 0; i < g_width; i++)
	{
		for (int j = 0; j < g_height; j++, ++ptr)
		{
			if (*ptr)
			{
				if ( x_step/2 < 1) // sometimes circles looks better :)
				{
					painter.drawPoint(
						static_cast<int>(i * x_step - win_width / 2. + 0.5 * x_step),
						static_cast<int>(j * y_step - win_height / 2. + 0.5 * y_step));
					
				}
				else
				{
					painter.drawEllipse( QPointF(i * x_step - win_width/2.+ 0.5*x_step,
                                    			     j * y_step - win_height/2.+0.5*y_step),
					                      x_step * 0.4, y_step * 0.4);
				}
			}
		}
	}
	painter.setPen(QColor(0,0,0));
// draw FPS
	painter.drawText(QPoint( win_width/2 - 200, +win_height / 2 - 10), QString(("FPS: "+ std::to_string(1.e6/time_delta_beween_frames)
                                          +" Process time:" + std::to_string(time_delta_in_frame ) + "mks"
                                         ).c_str()));
}
void Life::setGrid(int width, int height)
{
	if (grid[1])
		delete[] grid[1];
	if (grid[0])
		delete[] grid[0];
	g_width = width;
	g_height = height;
	x_step = double( win_width) / width;
	y_step = static_cast<double>( win_height) / height;
	grid[0] = new int[width*height];
	grid[1] = new int[width*height];
	for (int i = 0; i < g_width*g_height; i++)
	{
		grid[0][i] = 0;
		grid[1][i] = 0;
	}
}

void Life::setNumThreads(int x)
{
	omp_set_num_threads( x );
}

void Life::nextStep()
{
/*this is necessary because  we donn't want to 
* execute next step before before the previous one was completed
*/
	if (is_avaliable)
	{
		step();
		update();
	}
}

inline int neybours(int i, int j, int m, int n, int* grind)
{
  int x0 = (i + m - 1) % m, x2 = (i + 1) % m;
  int y0 = (j + n - 1) % n, y2 = (j + 1) % n;
  return  grind[x0 + y0 * m] + grind[x2 + y2 * m] +
          grind[i + y0 * m] + grind[i + y2 * m] +
          grind[x0 + j * m] + grind[x2 + j * m] +
          grind[x2 + y0 * m] + grind[x0 + y2 * m];
}

void Life::step()
{
	is_avaliable = false;
	auto start = std::chrono::high_resolution_clock::now();
	int next_index = (active_index + 1) % 2;
#pragma omp parallel for
	for (int j = 0; j < g_width; j++)
	{
		for (int i = 0; i < g_height; i++)
		{
      //std::cout << omp_get_thread_num() << std::endl;
      int *ptr = grid[active_index] + i + j*g_width;
      int *new_ptr = grid[next_index] + i + j*g_width;
      int ney = neybours(i, j, g_width, g_height, grid[active_index]);
		  if(*ptr == 0)
		  {
        if (ney == 3)
          *new_ptr = 1;
        else
          *new_ptr = 0;
      }
      else
      {
        if (ney != 3 && ney != 2)
        {
          *new_ptr = 0;
        }
        else
        {
          *new_ptr = 1;
        }
      }
    }
}
// this is time counting, do not look at this :)
  active_index = next_index;
  time_delta_in_frame = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
  time_delta_beween_frames = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_frame).count();
  last_frame = std::chrono::high_resolution_clock::now();
	
  is_avaliable = true;
}
