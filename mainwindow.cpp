#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QTest>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent) {
  resize(500, 500);
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  layout = new QGridLayout(centralWidget);
  label.resize(200, 200);
  size = 4;
  wumpa_texture = new QPixmap("pictures/base.png");
  fields.resize(size);
  for (int i = 0; i < size; ++i) {
    fields[i].resize(size);
    for (int j = 0; j < size; ++j) {
      Field *newfield = new Field(i, j, this);
      fields[i][j] = newfield;
      layout->addWidget(newfield, i, j);
      connect(newfield, SIGNAL(moved(int, int)), this, SLOT(movehandler(int, int)));
    }
  }
  new_game();
}

void MainWindow::new_game() {
    int pairs = size * size / 2;
    int *used = new int[pairs];
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for (int i = 0; i < pairs; i++) {
        used[i] = 0;
    }
    int pic;
    for (int i = 0; i < pairs * 2; i++) {
        do
            pic = qrand() % pairs;
        while (used[pic] == 2);
        used[pic]++;
        array[i / 4][i % 4] = 'A' + pic;
        texture[i / 4][i % 4] = new QPixmap("pictures/" + array[i / 4][i % 4] + ".jpg");
    }
    QIcon WumpaIcon(*wumpa_texture);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fields[i][j]->setText("");
            fields[i][j]->setEnabled(true);
            fields[i][j]->setIcon(WumpaIcon);
            fields[i][j]->setIconSize(QSize(120, 120));
            
        }
    }
    completed = 0;
    opened = 0;
    moves = 0;
    free(used);
}

MainWindow::~MainWindow() {
  for (int i = 0; i < fields.size(); ++i) {
    for (int j = 0; j < fields.size(); ++j) {
      delete fields[i][j];
    }
    fields[i].clear();
  }
  fields.clear();
  delete layout;
  delete centralWidget;
}

void MainWindow::end_game() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fields[i][j] -> setIcon(QIcon());
        }
    }
    fields[size / 2 - 1][size / 2 - 1]->setText("YOU");
    fields[size / 2 - 1][size / 2]->setText("WON!!!");
    fields[size / 2][size / 2 - 1]->setText("RESTART");
    fields[size / 2][size / 2]->setText("EXIT");
    fields[size / 2][size / 2 - 1]->setEnabled(true);
    fields[size / 2][size / 2]->setEnabled(true);
    fields[size / 2 + 1][size / 2 - 1] -> setText("Moves:");
    fields[size / 2 + 1][size / 2]->setText(QString::number(moves));
    disconnect(fields[size / 2][size / 2 - 1], SIGNAL(moved(int, int)), this, SLOT(movehandler(int, int)));
    disconnect(fields[size / 2][size / 2], SIGNAL(moved(int, int)), this, SLOT(movehandler(int, int)));
    connect(fields[size / 2][size / 2 - 1], SIGNAL(clicked()), this, SLOT(restarthandler()));
    connect(fields[size / 2][size / 2], SIGNAL(clicked()), this, SLOT(exithandler()));
}

void MainWindow::restarthandler() {
    disconnect(fields[size / 2][size / 2 - 1], SIGNAL(clicked()), this, SLOT(restarthandler()));
    disconnect(fields[size / 2][size / 2], SIGNAL(clicked()), this, SLOT(exithandler()));
    connect(fields[size / 2][size / 2 - 1], SIGNAL(moved(int, int)), this, SLOT(movehandler(int, int)));
    connect(fields[size / 2][size / 2], SIGNAL(moved(int, int)), this, SLOT(movehandler(int, int)));
    new_game(); 
}

void MainWindow::exithandler() {
    QApplication::quit();
}



void MainWindow::movehandler(int row, int col) {
  qDebug() << row << col;
  if (opened > 1) {
    return;
  }
  QIcon ButtonIcon(*texture[row][col]);
  fields[row][col]->setIcon(ButtonIcon);
  fields[row][col]->setIconSize(QSize(120, 120));
  fields[row][col]->setEnabled(false);
  if (opened) {
    opened++;
    moves++;
    qDebug() << "MOVES: " << moves;
    if (array[row][col] != array[op_row][op_col]) {
        QTest::qWait(1000);
        QIcon WumpaIcon(*wumpa_texture);
        fields[row][col]->setIcon(WumpaIcon);
        fields[op_row][op_col]->setIcon(WumpaIcon);
        fields[row][col]->setIconSize(QSize(120, 120));
        fields[op_row][op_col]->setIconSize(QSize(120, 120));
        fields[row][col] -> setEnabled(true);
        fields[op_row][op_col] -> setEnabled(true);
    } else {
        completed++;
        if (completed == size * size / 2) {
            end_game();
        }
    }
    opened = 0;
  } else {
    op_row = row;
    op_col = col;
    opened = 1;
  }
}
