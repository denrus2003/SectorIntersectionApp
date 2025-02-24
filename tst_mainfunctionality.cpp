#include <gtest/gtest.h>
#include "intersectioncalculator.h"
#include "geometry.h"
#include "sector.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QtTest/QTest>

using namespace testing;

class TestSectorIntersection : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TestSectorIntersection, IntersectionCalculator) {
    QPointF A(0, 0);
    QPointF B(10, 0);
    QPointF T(5, 5);
    qreal angle = 30.0;

    Sector sectorA(A, T, angle);
    Sector sectorB(B, T, angle);
    QVector<Sector> sectors = {sectorA, sectorB};

    QVector<QPointF> result = IntersectionCalculator::computeIntersection(sectors);
    ASSERT_GE(result.size(), 3);
}

TEST_F(TestSectorIntersection, PolygonArea) {
    QVector<QPointF> polygon = { {0,0}, {4,0}, {4,3}, {0,3} };
    qreal expectedArea = 12.0;
    qreal calculatedArea = Geometry::polygonArea(polygon);
    ASSERT_DOUBLE_EQ(calculatedArea, expectedArea);
}

TEST_F(TestSectorIntersection, InvalidInputHandling) {
    QVector<QPointF> emptySectors;
    qreal area = Geometry::polygonArea(emptySectors);
    ASSERT_EQ(area, 0.0);
}

class TestMainWindow : public ::testing::Test {
protected:
    QApplication *app;
    MainWindow *window;

    void SetUp() override {
        int argc = 0;
        char *argv[] = {nullptr};
        app = new QApplication(argc, argv);
        window = new MainWindow();
    }

    void TearDown() override {
        delete window;
        delete app;
    }
};

TEST_F(TestMainWindow, UIComponentsExist) {
    ASSERT_NE(window->findChild<QLineEdit*>("lineEditAx"), nullptr);
    ASSERT_NE(window->findChild<QLineEdit*>("lineEditAy"), nullptr);
    ASSERT_NE(window->findChild<QPushButton*>("buttonCalculate"), nullptr);
    ASSERT_NE(window->findChild<QLabel*>("labelResult"), nullptr);
}

TEST_F(TestMainWindow, CalculationFunctionality) {
    auto lineEditAx = window->findChild<QLineEdit*>("lineEditAx");
    auto lineEditAy = window->findChild<QLineEdit*>("lineEditAy");
    auto lineEditBx = window->findChild<QLineEdit*>("lineEditBx");
    auto lineEditBy = window->findChild<QLineEdit*>("lineEditBy");
    auto lineEditTx = window->findChild<QLineEdit*>("lineEditTx");
    auto lineEditTy = window->findChild<QLineEdit*>("lineEditTy");
    auto lineEditAngle = window->findChild<QLineEdit*>("lineEditAngle");
    auto buttonCalculate = window->findChild<QPushButton*>("buttonCalculate");
    auto labelResult = window->findChild<QLabel*>("labelResult");

    ASSERT_NE(lineEditAx, nullptr);
    ASSERT_NE(lineEditAy, nullptr);
    ASSERT_NE(lineEditBx, nullptr);
    ASSERT_NE(lineEditBy, nullptr);
    ASSERT_NE(lineEditTx, nullptr);
    ASSERT_NE(lineEditTy, nullptr);
    ASSERT_NE(lineEditAngle, nullptr);
    ASSERT_NE(buttonCalculate, nullptr);
    ASSERT_NE(labelResult, nullptr);

    lineEditAx->setText("0");
    lineEditAy->setText("0");
    lineEditBx->setText("10");
    lineEditBy->setText("0");
    lineEditTx->setText("5");
    lineEditTy->setText("5");
    lineEditAngle->setText("30");
    QTest::mouseClick(buttonCalculate, Qt::LeftButton);

    ASSERT_FALSE(labelResult->text().isEmpty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
