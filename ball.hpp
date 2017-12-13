#ifndef BALL_HPP
#define BALL_HPP
#include "struct.hpp"
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>

// ball class
class Ball
{
public:

  Ball() :ball_img_(0), ball_horizontal_speed_(1), ball_fall_speed(1) {}

inline  void setPixmap(QGraphicsPixmapItem * item);// set ball image
inline  bool IsBallCollideWIthItem(QGraphicsPixmapItem * item); // check for collision

  void moveBallVert(bool is_fall = true, int step = 0); // move ball for falling
  void moveBallHoriz(bool is_right, int width = 0); // move ball when key pressed
inline  void moveBallCoord(qreal x, qreal y); // move ball when reset or restart

  void setSpeed(int speed, bool is_horiz = false);
 inline void incFallSpeed(); // increment ball falling speed

inline  bool ifOnBorder(int height)const; // if ball go out the windows border return true
  void resetBall(); // move ball to start position

inline  int getPixmapWidth()const; // width of ball image
inline  int getX()const;
inline  int getY()const;
private:
  QGraphicsPixmapItem * ball_img_;
  size_t ball_horizontal_speed_, ball_fall_speed;
};



inline int Ball::getPixmapWidth() const
{
  return ball_img_->pixmap().width();
}

inline int Ball::getX() const
{
  return ball_img_->x();
}

inline int Ball::getY() const
{
  return ball_img_->y();
}

inline void Ball::setPixmap(QGraphicsPixmapItem *item)
{
  ball_img_ = item;
}

inline bool Ball::IsBallCollideWIthItem(QGraphicsPixmapItem *item)
{
  return ball_img_->collidesWithItem(item);
}


inline void Ball::moveBallCoord(qreal x, qreal y)
{
  ball_img_->setPos(x, y);
}

inline void Ball::incFallSpeed()
{
  if (ball_fall_speed < 15)
  {
    ++ball_fall_speed;
  }
}


 inline bool Ball::ifOnBorder(int height) const
{
  if (ball_img_->y() + ball_img_->pixmap().height() > height || ball_img_->y() < 0)
  {
    return true;
  }
  return false;
}


#endif // BALL_HPP
