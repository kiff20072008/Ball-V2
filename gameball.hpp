#ifndef GAMEBALL_HPP
#define GAMEBALL_HPP

#include <vector>
#include <algorithm>
#include "ball.hpp"
#include "struct.hpp"
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsScene>


class GameBall : public QGraphicsScene
{

  Q_OBJECT

public:

  explicit GameBall(QWidget *parent = 0);
  void start();

private:
  Ball ball_;
  Platforms_t platforms_; // array of platforms used and unused
  QGraphicsPixmapItem* life_; // ptr to life img
  Life_t lifes_; // array of 3 lifes shows up left

  size_t Score_; // game score

  QTimer* block_timer_; // timer to move platforms
  QTimer* ball_timer_; // timer to ball fall

  void firstInitial();
  void generateNewPrep(); // add new platform/s to screen
  void setPlatformCoord(int width, Platform_t *platform);

  inline Platform_t* findFirstUnusedPlatfom();
  inline Platform_t* findFirstUnusedDeathPlatform();

  inline Platform_t *findTheLowestPrep(); // find lowest platform to calculate new platform y coordinate

  void keyPressEvent(QKeyEvent *event);
  void changeNumLifes(bool add); // add or remove lifes

  bool checkCollide(bool is_reset = false); // check if ball collide with any items

  void gameOver();
  void setBackground(); //set random background
  inline void resetPlatform(Platform_t &platform); // move platform to coordinate 700 0 to move it from screen
  inline void resetLifeItem(QGraphicsPixmapItem* life); // -||- but with life img

signals:
  void pQuitApp();
  void ChangeScore(int);

  protected slots:
  void resetSignal();
  void restartSignal();
  void moveBlocksSignal();
  void moveBallSignal();
};

inline Platform_t* GameBall::findFirstUnusedPlatfom()
{
  return std::find_if(platforms_.arr_platforms_.begin(), platforms_.arr_platforms_.end(), [](Platform_t obj) {return (!(obj.is_used || obj.is_death)); });
}

inline Platform_t* GameBall::findFirstUnusedDeathPlatform()
{
  return std::find_if(platforms_.arr_platforms_.begin(), platforms_.arr_platforms_.end(), [](Platform_t obj) {return ((!obj.is_used) && obj.is_death); });
}

inline Platform_t* GameBall::findTheLowestPrep()
{
  return std::min_element(platforms_.arr_platforms_.begin(), platforms_.arr_platforms_.end(), [](const Platform_t &lhs, const Platform_t &rhs) {return (lhs.platform_img->y() > rhs.platform_img->y()); });
}

inline void GameBall::resetPlatform(Platform_t &platform)
{
  platform.platform_img->setPos(700, 0);
  platform.is_used = false;
}

inline void GameBall::resetLifeItem(QGraphicsPixmapItem *life)
{
  life->setVisible(false);
  life->setPos(100, 800);
}

#endif // GAMEBALL_HPP
