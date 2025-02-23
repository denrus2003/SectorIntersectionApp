#ifndef SECTOR_H
#define SECTOR_H

#include <QPointF>
#include <QtMath>
#include "geometry.h"

class Sector {
public:
    Sector(QPointF origin, QPointF target, qreal angleDegrees) noexcept
        : m_origin(std::move(origin)),
        m_target(std::move(target)),
        m_angleRadians(qDegreesToRadians(angleDegrees)) {}

    [[nodiscard]] constexpr QPointF origin() const noexcept { return m_origin; }
    [[nodiscard]] constexpr QPointF target() const noexcept { return m_target; }
    [[nodiscard]] constexpr qreal angle() const noexcept { return m_angleRadians; }

    // Центральное направление сектора (нормализованный вектор от origin к target)
    [[nodiscard]] QPointF centralDirection() const noexcept {
        const QPointF d = m_target - m_origin;
        const qreal len = qSqrt(Geometry::dot(d, d));
        return (qFuzzyCompare(len, 0.0)) ? QPointF(1, 0) : d / len;
    }

    // Векторы направлений границ сектора
    [[nodiscard]] QPointF leftBoundaryDirection() const noexcept {
        return Geometry::rotate(centralDirection(), m_angleRadians * 0.5);
    }

    [[nodiscard]] QPointF rightBoundaryDirection() const noexcept {
        return Geometry::rotate(centralDirection(), -m_angleRadians * 0.5);
    }

    // Построение половинной плоскости для заданного направления границы
    [[nodiscard]] Geometry::HalfPlane halfPlane(const QPointF& boundaryDirection) const noexcept {
        Geometry::HalfPlane hp{m_origin, Geometry::rotate(boundaryDirection, M_PI_2)};
        if (Geometry::dot(hp.normal, m_target - m_origin) < 0) {
            hp.normal = -hp.normal;
        }
        hp.normal /= qSqrt(Geometry::dot(hp.normal, hp.normal)); // Нормализация
        return hp;
    }

    [[nodiscard]] Geometry::HalfPlane leftHalfPlane() const noexcept {
        return halfPlane(leftBoundaryDirection());
    }

    [[nodiscard]] Geometry::HalfPlane rightHalfPlane() const noexcept {
        return halfPlane(rightBoundaryDirection());
    }

private:
    QPointF m_origin;
    QPointF m_target;
    qreal m_angleRadians;
};

#endif // SECTOR_H
