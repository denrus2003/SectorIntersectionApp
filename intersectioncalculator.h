#ifndef INTERSECTIONCALCULATOR_H
#define INTERSECTIONCALCULATOR_H

#include <QVector>
#include <QPointF>
#include "geometry.h"
#include "sector.h"

class IntersectionCalculator {
public:
    [[nodiscard]] static QVector<QPointF> computeIntersection(const QVector<Sector> &sectors) {
        if (sectors.isEmpty()) return {}; // Если секторов нет, пересечение пустое

        // Собираем половинные плоскости
        QVector<Geometry::HalfPlane> halfPlanes;
        halfPlanes.reserve(sectors.size() * 2);
        for (const auto &sector : sectors) {
            halfPlanes.append(sector.leftHalfPlane());
            halfPlanes.append(sector.rightHalfPlane());
        }

        // Начальный большой ограничивающий многоугольник
        constexpr qreal BOUNDARY = 1e4; // Достаточно большое число
        QVector<QPointF> poly = {
            {-BOUNDARY, -BOUNDARY},
            {BOUNDARY, -BOUNDARY},
            {BOUNDARY, BOUNDARY},
            {-BOUNDARY, BOUNDARY}
        };

        // Последовательно отсекать многоугольник по каждой половинной плоскости
        for (const auto &hp : halfPlanes) {
            poly = Geometry::clipPolygon(poly, hp);
            if (poly.isEmpty()) return {}; // Если отсекли всё, выходим сразу
        }

        return poly;
    }
};

#endif // INTERSECTIONCALCULATOR_H
