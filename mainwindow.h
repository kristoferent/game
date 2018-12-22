#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include <QFile>
#include <QLabel>
#include "field.h"
#include <QPixmap>

class MainWindow : public QMainWindow {
  Q_OBJECT

  QWidget *centralWidget;
  QGridLayout *layout;
  QVector< QVector<Field *> > fields;
  QByteArray line;
  QLabel label;
  QString array[100][100];
  QString wumpa;
  int opened;
  int completed;
  int moves;
  int size;
  int op_row, op_col;
  QPixmap *texture[100][100];
  QPixmap *wumpa_texture;
public:
  MainWindow(QWidget *parent = NULL);
  void end_game();
  void new_game();
  ~MainWindow();

public slots:
  void movehandler(int row, int col);
  void exithandler();
  void restarthandler();
};

#endif
