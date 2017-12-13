#include "gameball.hpp"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
  //creation of window
  QApplication a(argc, argv);
  QWidget wgt;
  QPushButton* button_reset = new QPushButton("Reset");
  QPushButton* button_restart = new QPushButton("Start");
  GameBall obj;
  QGraphicsView* view = new QGraphicsView(&obj);
  QLabel* lbl = new QLabel("You score = ");
  QLabel* score_label = new QLabel("0");
  QVBoxLayout* layout = new QVBoxLayout;
  QHBoxLayout* layout1 = new QHBoxLayout;

  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
  a.setApplicationName("Ball ver.2.0");

  layout1->addWidget(lbl);
  layout1->addWidget(score_label);
  layout1->setAlignment(Qt::AlignLeft);
  layout1->addWidget(button_reset);
  layout1->addWidget(button_restart);
  layout->addLayout(layout1);
  layout->addWidget(view);

  wgt.setLayout(layout);
  wgt.setFixedSize(550, 770);

  QObject::connect(&obj, SIGNAL(pQuitApp()), &a, SLOT(quit()));
  QObject::connect(button_restart, SIGNAL(clicked()), &obj, SLOT(restartSignal()));
  QObject::connect(button_reset, SIGNAL(clicked()), &obj, SLOT(resetSignal()));
  QObject::connect(&obj, SIGNAL(ChangeScore(int)), score_label, SLOT(setNum(int)));

  wgt.show();
  obj.start();

  return a.exec();
}
