#include "mainwindow.h"
#include "sector.h"
#include "intersectioncalculator.h"

#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    //Поля ввода
    auto createInputField = [&](const QString &LabelText, QLineEdit *&lineEditX, QLineEdit  *&lineEditY) {
        QLabel *label = new QLabel(LabelText, this);
        lineEditX = new QLineEdit(this);
        lineEditY = new QLineEdit(this);
        return std::make_tuple(label, lineEditX, lineEditY);
    };

    auto [labelA, lineAx, lineAy] = createInputField ("Точка A (x, y):", lineEditAx, lineEditAy);
    auto [labelB, lineBx, lineBy] = createInputField("Точка B (x, y):", lineEditBx, lineEditBy);
    auto [labelT, lineTx, lineTy] = createInputField("Точка T (x, y):", lineEditTx, lineEditTy);
    auto [labelC, lineCx, lineCy] = createInputField("Точка C (x, y) (опционально):", lineEditCx, lineEditCy);

    QLabel *labelAngle = new QLabel("Угол q (в градусах):", this);
    lineEditAngle = new QLineEdit(this);

    buttonCalculate = new QPushButton("Рассчитать", this);
    labelResult = new QLabel("Результаты", this);
    labelResult->setWordWrap(true);

    vizWidget = new VisualizationWidget(this);
    vizWidget->setMinimumSize(400, 400);

    // Расположение элементов
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelA, 0, 0); gridLayout->addWidget(lineEditAx, 0, 1); gridLayout->addWidget(lineEditAy, 0, 2);
    gridLayout->addWidget(labelB, 1, 0); gridLayout->addWidget(lineEditBx, 1, 1); gridLayout->addWidget(lineEditBy, 1, 2);
    gridLayout->addWidget(labelT, 2, 0); gridLayout->addWidget(lineEditTx, 2, 1); gridLayout->addWidget(lineEditTy, 2, 2);
    gridLayout->addWidget(labelAngle, 3, 0); gridLayout->addWidget(lineEditAngle, 3, 1);
    gridLayout->addWidget(labelC, 4, 0); gridLayout->addWidget(lineEditCx, 4, 1); gridLayout->addWidget(lineEditCy, 4, 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(buttonCalculate);
    mainLayout->addWidget(labelResult);
    mainLayout->addWidget(vizWidget);

    connect(buttonCalculate, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
}

// Функция для безопасного чтения числа из QLineEdit
[[nodiscard]] qreal MainWindow::getValidatedInput(QLineEdit *lineEdit, const QString &fieldName) const {
    bool ok;
    qreal value = lineEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(nullptr, "Ошибка", QString("Неверное значение для %1").arg(fieldName));
        throw std::runtime_error("Некорректный ввод");
    }
    return value;
}

// Получение всех необходимых точек из полей ввода
[[nodiscard]] QVector<QPointF> MainWindow::getInputPoints() const {
    try {
        QPointF A(getValidatedInput(lineEditAx, "A.x"), getValidatedInput(lineEditAy, "A.y"));
        QPointF B(getValidatedInput(lineEditBx, "B.x"), getValidatedInput(lineEditBy, "B.y"));
        QPointF T(getValidatedInput(lineEditTx, "T.x"), getValidatedInput(lineEditTy, "T.y"));

        QVector<QPointF> points = {A, B, T};

        if (!lineEditCx->text().isEmpty() && !lineEditCy->text().isEmpty()) {
            QPointF C(getValidatedInput(lineEditCx, "C.x"), getValidatedInput(lineEditCy, "C.y"));
            points.append(C);
        }

        return points;
    } catch (const std::runtime_error &) {
        return {};
    }
}

// Получение и валидация угла
[[nodiscard]] qreal MainWindow::getAngleInput() const {
    try {
        qreal angle = getValidatedInput(lineEditAngle, "угла q");
        if (angle <= 0 || angle >= 180) {
            QMessageBox::warning(nullptr, "Ошибка", "Угол q должен быть в диапазоне (0, 180)");
            throw std::runtime_error("Некорректный угол");
        }
        return angle;
    } catch (const std::runtime_error &) {
        return -1;
    }
}

void MainWindow::onCalculateClicked() {
    QVector<QPointF> points = getInputPoints();
    if (points.isEmpty()) return; // Ошибка уже обработана

    qreal angleDeg = getAngleInput();
    if (angleDeg == -1) return;

    QVector<Sector> sectors;
    for (int i = 0; i < points.size() - 1; ++i) {
        sectors.append(Sector(points[i], points.last(), angleDeg));
    }

    QVector<QPointF> poly = IntersectionCalculator::computeIntersection(sectors);
    qreal area = Geometry::polygonArea(poly);

    // Формируем строку результата
    QString resultStr = QString("Площадь пересечения: %1\nКоординаты вершин:\n").arg(area);
    for (int i = 0; i < poly.size(); ++i) {
        resultStr += QString("Вершина %1: (%2, %3)\n").arg(i + 1).arg(poly[i].x()).arg(poly[i].y());
    }
    labelResult->setText(resultStr);

    // Передаем данные в виджет визуализации
    vizWidget->setData(sectors, poly, points);
}
