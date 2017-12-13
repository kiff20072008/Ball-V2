#include "gameball.hpp"


GameBall::GameBall(QWidget *parent) :
  QGraphicsScene(parent)
{
  firstInitial();
}

void GameBall::start()
{
  restartSignal();
}



void GameBall::firstInitial()
{
  setSceneRect(0, 0, 500, 700);
  lifes_.arr_of_lifes_.reserve(NUMBER_OF_LIFES);

  for (auto i = 0; i < NUMBER_OF_LIFES; ++i)
  {
    lifes_.arr_of_lifes_.push_back(addPixmap(QPixmap(":/img/life.png")));
    lifes_.arr_of_lifes_[i]->setVisible(true);
    lifes_.arr_of_lifes_[i]->setPos((lifes_.arr_of_lifes_[i]->pixmap().width() + 5)*i - 10, 0);
  }

  life_ = addPixmap(QPixmap(":/img/life.png"));

  ball_.setPixmap(addPixmap(QPixmap(":/img/ball.png")));
  platforms_.arr_platforms_.reserve(3 * NUMBER_OF_PREPS / 2);

  for (int i = 0; i < NUMBER_OF_PREPS; ++i)
  {
    platforms_.arr_platforms_.push_back(Platform_t(addPixmap(QPixmap(":/img/prep.png"))));
  }
  for (int i = 0; i < NUMBER_OF_PREPS / 2; ++i)
  {
    platforms_.arr_platforms_.push_back(Platform_t(addPixmap(QPixmap(":/img/prep_2.png")), true));
  }

  block_timer_ = new QTimer(this);
  connect(block_timer_, SIGNAL(timeout()), SLOT(moveBlocksSignal()));
  ball_timer_ = new QTimer(this);
  connect(ball_timer_, SIGNAL(timeout()), SLOT(moveBallSignal()));
}

void GameBall::generateNewPrep()
{
  static int num_of_prep_without_ship = 0;
  Platform_t * ptr_to_unused = NULL;
  if (qrand() % 2)
  {
    if (qrand() % 2 && num_of_prep_without_ship > 2)
    {
      ptr_to_unused = findFirstUnusedDeathPlatform();
      num_of_prep_without_ship = 0;
    }
    else
    {
      ptr_to_unused = findFirstUnusedPlatfom();
      ++num_of_prep_without_ship;
    }
    setPlatformCoord(qrand() % (static_cast<int>(width()) - ptr_to_unused->platform_img->pixmap().width()), ptr_to_unused);
  }
  else
  {
    ptr_to_unused = findFirstUnusedPlatfom();
    setPlatformCoord(qrand() % (static_cast<int>(width()) - 2 * ptr_to_unused->platform_img->pixmap().width() - 2 * ball_.getPixmapWidth()), ptr_to_unused);
    int temp = ptr_to_unused->platform_img->x() + 2 * ptr_to_unused->platform_img->pixmap().width() + 2 * ball_.getPixmapWidth();
    ptr_to_unused = findFirstUnusedPlatfom();
    setPlatformCoord(qrand() % (static_cast<int>(width()) - temp) + temp - ptr_to_unused->platform_img->pixmap().width(), ptr_to_unused);
    ++num_of_prep_without_ship;
  }
  if ((!life_->isVisible()) && qrand() % 2 && (!ptr_to_unused->is_death))
  {
    life_->setVisible(true);
    life_->setPos(ptr_to_unused->platform_img->x() + qrand() % (ptr_to_unused->platform_img->pixmap().width() - 2 * life_->pixmap().width()) + life_->pixmap().width(), ptr_to_unused->platform_img->y() - life_->pixmap().height());
  }
}

void GameBall::setPlatformCoord(int width, Platform_t *platform)
{
  platform->platform_img->setPos(width, platforms_.coord_of_new_platf_);
  platform->is_used = true;
}


void GameBall::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    ball_.setSpeed(5, true);
  }
  else
  {
    ball_.setSpeed(10, true);
  }
  switch (event->key())
  {
  case Qt::Key_Left:
  case Qt::Key_A:
  {
    ball_.moveBallHoriz(false);
    if (checkCollide())
    {
      ball_.moveBallHoriz(true, width());
      ball_.setSpeed(1, true);
      return;
    }
    break;
  }
  case Qt::Key_Right:
  case Qt::Key_D:
  {
    ball_.moveBallHoriz(true, width());
    if (checkCollide())
    {
      ball_.moveBallHoriz(false);
      ball_.setSpeed(1, true);
      return;
    }
    break;
  }
  }

}

void GameBall::changeNumLifes(bool add)
{
  if (add)
  {
    if (lifes_.num_of_lifes < 3)
    {
      lifes_.arr_of_lifes_.at(++lifes_.num_of_lifes - 1)->setVisible(true);
    }
  }
  else
  {
    if (lifes_.num_of_lifes == 0)
    {
      gameOver();
    }
    else
    {
      lifes_.arr_of_lifes_.at(--lifes_.num_of_lifes)->setVisible(false);
    }
  }
}

void GameBall::gameOver()
{
  ball_timer_->stop();
  block_timer_->stop();
  bool n = static_cast<bool>(QMessageBox::warning(0,
    "Warning",
    "Game over!,"
    "\n Do you want to restart?",
    "Yes",
    "No",
    QString(),
    false,
    true
  ));
  if (!n) {
    restartSignal();
  }
  else
    emit pQuitApp();
}

void GameBall::setBackground()
{
  switch (qrand() % 5)
  {
  case 0:
  {
    setBackgroundBrush(QPixmap(":/img/fon_1.png"));
    break;
  }
  case 1:
  {
    setBackgroundBrush(QPixmap(":/img/fon_2.png"));
    break;
  }
  case 2:
  {
    setBackgroundBrush(QPixmap(":/img/fon_3.png"));
    break;
  }
  case 3:
  {
    setBackgroundBrush(QPixmap(":/img/fon_4.png"));
    break;
  }
  case 4:
  {
    setBackgroundBrush(QPixmap(":/img/fon_5.png"));
    break;
  }
  }
}



bool GameBall::checkCollide(bool is_reset)
{
  if (ball_.IsBallCollideWIthItem(life_))
  {
    changeNumLifes(true);
    resetLifeItem(life_);
  }

  for (auto &it : platforms_.arr_platforms_)
  {
    if (ball_.IsBallCollideWIthItem(it.platform_img))
    {
      if (it.is_death)
      {
        if (!is_reset)
        {
          resetSignal();
        }
        return false;
      }
      return true;
    }
  }
  return false;
}

void GameBall::resetSignal()
{
  changeNumLifes(false);
  ball_.resetBall();
  while (checkCollide(true))
  {
    ball_.moveBallVert(true, 1);
  }

}

void GameBall::restartSignal()
{
  setBackground();
  emit ChangeScore(static_cast<int>(Score_ = 0));
  ball_.resetBall();
  lifes_.num_of_lifes = NUMBER_OF_LIFES;
  platforms_.speed_of_preps_ = 20;
  std::for_each(lifes_.arr_of_lifes_.begin(), lifes_.arr_of_lifes_.end(), [](QGraphicsPixmapItem* item) {item->setVisible(true); });
  platforms_.coord_of_new_platf_ = width();

  std::for_each(platforms_.arr_platforms_.begin(), platforms_.arr_platforms_.end(), [&](Platform_t & obj) {resetPlatform(obj); });
  for (int i = 0; i < 7; ++i)
  {
    generateNewPrep();
    platforms_.coord_of_new_platf_ += qrand() % 50 + 150;
  }

  ball_timer_->start(SPEED_OF_TIMER_BALL);
  block_timer_->start(platforms_.speed_of_preps_);
  resetLifeItem(life_);
}

void GameBall::moveBlocksSignal()
{
  bool is_prep_add = false;
  for (auto &it : platforms_.arr_platforms_)
  {
    if (it.is_used)
    {
      it.platform_img->setPos(it.platform_img->x(), it.platform_img->y() - 1);
      if (ball_.IsBallCollideWIthItem(it.platform_img))
      {
        if (ball_.getX() + ball_.getPixmapWidth() / 2 > it.platform_img->x() + it.platform_img->pixmap().width() && it.platform_img->x() + it.platform_img->pixmap().width() + ball_.getPixmapWidth() < width())
        {
          ball_.setSpeed(1, true);
          ball_.moveBallHoriz(true, width());
        }
        if (ball_.getX() + ball_.getPixmapWidth() / 2 < it.platform_img->x() && it.platform_img->x() > ball_.getPixmapWidth())
        {
            ball_.setSpeed(1, true);
          ball_.moveBallHoriz(false, width());
        }
        ball_.moveBallVert(false, 1);
      }
      if (it.platform_img->y() < -it.platform_img->pixmap().height() - 1)
      {
        resetPlatform(it);
        is_prep_add = true;
      }
    }
  }
  life_->setPos(life_->x(), life_->y() - 1);
  if (life_->y() < 0)
  {
    resetLifeItem(life_);
  }
  if (is_prep_add)
  {
    emit ChangeScore(static_cast<int>(++Score_));
    if (!(Score_ % 10))
    {
      if (platforms_.speed_of_preps_ > 1)
      {
        --platforms_.speed_of_preps_;
        block_timer_->setInterval(platforms_.speed_of_preps_);
      }
    }

    platforms_.coord_of_new_platf_ = findTheLowestPrep()->platform_img->y() + qrand() % 50 + 150;
    generateNewPrep();
  }

}

void GameBall::moveBallSignal()
{
  static int temp = 0;
  ++temp;
  if (checkCollide())
  {
    return;
  }

  ball_.moveBallVert();
  if (checkCollide())
  {
    ball_.moveBallVert(false);
    ball_.setSpeed(1);
    return;
  }
  if (temp % 2)
  {
    ball_.incFallSpeed();
  }
  if (ball_.ifOnBorder(height()))
  {
    resetSignal();
  }
}


