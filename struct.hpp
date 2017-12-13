#ifndef STRUCT_HPP
#define STRUCT_HPP
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsScene>

#define NUMBER_OF_LIFES 3
#define NUMBER_OF_PREPS 14
#define SPEED_OF_TIMER_BALL 50
#define SPEED_OF_PREP 20


// each platform can be used or not, death or not, and has each img
struct Platform_t
{
public:
  QGraphicsPixmapItem* platform_img;
  bool is_used, is_death;
  Platform_t(QGraphicsPixmapItem * img = 0, bool death = false, bool used = false) :platform_img(img), is_used(used), is_death(death) {}
};

//struct with arr of platforms and some var
struct Platforms_t
{
  QVector<Platform_t> arr_platforms_;
  int speed_of_preps_;
  int coord_of_new_platf_;
};

//arr of lifes img and amount of them
struct Life_t
{
  QVector<QGraphicsPixmapItem*> arr_of_lifes_;
  int num_of_lifes;
};

#endif // STRUCT_HPP
