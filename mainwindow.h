#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "visualizationwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCalculateClicked();

private:
    void setupUi();
    [[nodiscard]] QVector<QPointF> getInputPoints() const;
    [[nodiscard]] qreal getAngleInput() const;
    [[nodiscard]] qreal getValidatedInput(QLineEdit *lineEdit, const QString &fieldName) const;

    QWidget *centralWidget;
    QLineEdit *lineEditAx;
    QLineEdit *lineEditAy;
    QLineEdit *lineEditBx;
    QLineEdit *lineEditBy;
    QLineEdit *lineEditTx;
    QLineEdit *lineEditTy;
    QLineEdit *lineEditAngle;
    QLineEdit *lineEditCx;
    QLineEdit *lineEditCy;

    QPushButton *buttonCalculate;
    QLabel *labelResult;
    VisualizationWidget *vizWidget;
};

#endif // MAINWINDOW_H
