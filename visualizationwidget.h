#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QPainter>
#include "sector.h"

class VisualizationWidget : public QWidget {
    Q_OBJECT
public:
    explicit VisualizationWidget(QWidget *parent = nullptr) noexcept
        : QWidget(parent) {}

    // Установка данных для визуализации
    void setData(QVector<Sector> sectors, QVector<QPointF> intersectionPoly, QVector<QPointF> points) {
        m_sectors = std::move(sectors);
        m_intersectionPoly = std::move(intersectionPoly);
        m_points = std::move(points);
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Вычисляем границы всех объектов
        QRectF boundingRect = computeBoundingRect().adjusted(-20, -20, 20, 20);

        // Настраиваем трансформацию: центрируем и масштабируем с инверсией Y
        QTransform transform;
        transform.translate(width() / 2.0, height() / 2.0);
        qreal scale = qMin(width() / boundingRect.width(), height() / boundingRect.height());
        transform.scale(scale, -scale);
        transform.translate(-boundingRect.center().x(), -boundingRect.center().y());
        painter.setTransform(transform);

        // Отрисовка секторов
        drawSectors(painter);

        // Отрисовка области пересечения
        drawIntersectionPolygon(painter);

        // Отрисовка дополнительных точек
        drawPoints(painter);
    }

private:
    QVector<Sector> m_sectors;
    QVector<QPointF> m_intersectionPoly;
    QVector<QPointF> m_points;

    // Вычисление границ для масштабирования
    [[nodiscard]] QRectF computeBoundingRect() const noexcept {
        QRectF rect;
        bool first = true;
        auto expandRect = [&](const QPointF &point) {
            if (first) {
                rect = QRectF(point, QSizeF(1, 1));
                first = false;
            } else {
                rect = rect.united(QRectF(point, QSizeF(1, 1)));
            }
        };

        for (const auto &sector : m_sectors) expandRect(sector.origin());
        for (const auto &pt : m_points) expandRect(pt);
        for (const auto &pt : m_intersectionPoly) expandRect(pt);

        return rect.isNull() ? QRectF(-100, -100, 200, 200) : rect;
    }

    // Отрисовка секторов
    void drawSectors(QPainter &painter) const {
        QPen sectorPen(Qt::blue, 0.5);
        painter.setPen(sectorPen);
        painter.setBrush(Qt::red);

        for (const auto &sector : m_sectors) {
            const QPointF origin = sector.origin();
            painter.drawEllipse(origin, 3, 3);

            const auto drawRay = [&](const QPointF &dir) {
                painter.drawLine(origin, origin + dir * 1000);
            };

            drawRay(sector.centralDirection());
            drawRay(sector.leftBoundaryDirection());
            drawRay(sector.rightBoundaryDirection());
        }
    }

    // Отрисовка многоугольника пересечения
    void drawIntersectionPolygon(QPainter &painter) const {
        if (m_intersectionPoly.isEmpty()) return;

        QPen polyPen(Qt::darkGreen, 1.0);
        painter.setPen(polyPen);
        painter.setBrush(QColor(0, 255, 0, 100)); // Полупрозрачный зеленый

        painter.drawPolygon(m_intersectionPoly.constData(), m_intersectionPoly.size());
    }

    // Отрисовка дополнительных точек
    void drawPoints(QPainter &painter) const {
        painter.setBrush(Qt::black);
        for (const auto &pt : m_points) {
            painter.drawEllipse(pt, 3, 3);
        }
    }
};

#endif // VISUALIZATIONWIDGET_H
