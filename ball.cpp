#ifndef BALL_CPP
#define BALL_CPP
#include "ball.hpp"




// calculate direction of fall and value to move ball
void Ball::moveBallVert(bool is_fall, int step)
{
  int dir = (is_fall)? 1:-1;
  int value =(step)?step:ball_fall_speed;
  moveBallCoord(ball_img_->x(),ball_img_->y()+dir*value);
}

//check for collisison with borders and move
void Ball::moveBallHoriz(bool is_right, int width)
{
  if (is_right)
  {
      if (ball_img_->x() > width - 3 * ball_img_->pixmap().width() / 2)
      {
        ball_horizontal_speed_ = 1; // if ball near border speed inc
      }
      if (ball_img_->x() <= width - ball_img_->pixmap().width() + 15)
      {
        moveBallCoord(ball_img_->x() + ball_horizontal_speed_, ball_img_->y());
      }
  }
  else
  {
      if (ball_img_->x() < ball_img_->pixmap().width() / 2)
      {
        ball_horizontal_speed_ = 1;// if ball near border speed inc
      }
      if (ball_img_->x() > -15)
      {
        moveBallCoord(ball_img_->x() - ball_horizontal_speed_, ball_img_->y());
      }
  }
}



void Ball::setSpeed(int speed, bool is_horiz)
{
  if (speed < 0)
  {
    return;
  }
  if (is_horiz)
  {
    ball_horizontal_speed_ = speed;
  }
  else
  {
    ball_fall_speed = speed;
  }
}



// move ball to start point and reset speedes
void Ball::resetBall()
{
  ball_horizontal_speed_ = 1;
  ball_fall_speed = 1;
  moveBallCoord(250, 100);
}



#endif // BALL_CPP

