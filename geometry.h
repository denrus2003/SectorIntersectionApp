#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPointF>
#include <QVector>
#include <QtMath>

namespace Geometry {

// Векторное произведение (скалярный аналог для 2D)
constexpr inline qreal cross(const QPointF &a, const QPointF &b) noexcept {
    return a.x() * b.y() - a.y() * b.x();
}

// Скалярное произведение
constexpr inline qreal dot(const QPointF &a, const QPointF &b) noexcept {
    return a.x() * b.x() + a.y() * b.y();
}

// Поворот вектора на заданный угол (радианы)
constexpr inline QPointF rotate(const QPointF &v, qreal angle) noexcept {
    const qreal ca = qCos(angle);
    const qreal sa = qSin(angle);
    return {v.x() * ca - v.y() * sa, v.x() * sa + v.y() * ca};
}

// Описание половинной плоскости (граница + внутренняя область)
struct HalfPlane {
    QPointF p;       // Точка на линии
    QPointF normal;  // Единичный вектор внутрь области

    [[nodiscard]] bool contains(const QPointF &point) const noexcept {
        return dot(point - p, normal) >= 0;
    }
};

// Пересечение двух линий p + t*r и q + u*s. Возвращает true, если пересечение найдено.
inline bool lineIntersection(const QPointF &p, const QPointF &r,
                             const QPointF &q, const QPointF &s,
                             QPointF &intersection) noexcept {
    const qreal rxs = cross(r, s);
    if (qFuzzyCompare(rxs, 0.0)) return false; // Линии параллельны

    const QPointF qp = q - p;
    const qreal t = cross(qp, s) / rxs;
    intersection = p + r * t;
    return true;
}

// Отсечение многоугольника половинной плоскостью (Sutherland–Hodgman)
inline QVector<QPointF> clipPolygon(const QVector<QPointF> &poly, const HalfPlane &hp) {
    if (poly.isEmpty()) return {};

    QVector<QPointF> newPoly;
    newPoly.reserve(poly.size());

    QPointF prev = poly.last();
    bool prevInside = hp.contains(prev);

    for (const auto &current : poly) {
        const bool currentInside = hp.contains(current);
        if (prevInside != currentInside) {
            const QPointF direction = current - prev;
            const qreal denom = dot(direction, hp.normal);
            if (!qFuzzyCompare(denom, 0.0)) { // Избегаем деления на 0
                const qreal t = -dot(prev - hp.p, hp.normal) / denom;
                newPoly.append(prev + direction * t);
            }
        }
        if (currentInside) newPoly.append(current);
        prev = current;
        prevInside = currentInside;
    }
    return newPoly;
}

// Площадь многоугольника (метод Гаусса)
inline qreal polygonArea(const QVector<QPointF> &poly) noexcept {
    if (poly.size() < 3) return 0.0; // Многоугольник должен содержать минимум 3 точки

    qreal area = 0.0;
    for (int i = 0, n = poly.size(); i < n; ++i) {
        area += cross(poly[i], poly[(i + 1) % n]);
    }
    return qAbs(area) * 0.5;
}

} // namespace Geometry

#endif
