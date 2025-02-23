#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPointF>
#include <QVector>
#include <QtMath>

namespace Geometry {

// Вычисление векторного (псевдо)произведения двух векторов
inline qreal cross(const QPointF &a, const QPointF &b) {
    return a.x() * b.y() - a.y() * b.x();
}

// Скалярное произведение
inline qreal dot(const QPointF &a, const QPointF &b) {
    return a.x() * b.x() + a.y() * b.y();
}

// Поворот вектора v на угол angle (в радианах)
inline QPointF rotate(const QPointF &v, qreal angle) {
    qreal ca = qCos(angle);
    qreal sa = qSin(angle);
    return QPointF(v.x()*ca - v.y()*sa, v.x()*sa + v.y()*ca);
}

// Структура, описывающая половинную плоскость (линия + внутренняя сторона)
struct HalfPlane {
    QPointF p;       // Точка на линии (начало координат для сектора)
    QPointF normal;  // Единичный вектор, направленный во внутреннюю сторону
};

// Проверка: принадлежит ли точка данной половинной плоскости
inline bool isInside(const QPointF &point, const HalfPlane &hp) {
    return dot(point - hp.p, hp.normal) >= 0;
}

// Функция для вычисления точки пересечения двух линий,
// заданных в виде: p + t*r и q + u*s. Если линии параллельны – возвращается false.
inline bool lineIntersection(const QPointF &p, const QPointF &r,
                             const QPointF &q, const QPointF &s,
                             QPointF &intersection) {
    qreal rxs = cross(r, s);
    if (qFuzzyCompare(rxs, 0.0))
        return false; // Линии параллельны
    QPointF qp = q - p;
    qreal t = cross(qp, s) / rxs;
    intersection = p + r * t;
    return true;
}

// Алгоритм отсечения многоугольника половинной плоскостью
// (реализация по методу Sutherland–Hodgman)
inline QVector<QPointF> clipPolygonByHalfPlane(const QVector<QPointF> &poly, const HalfPlane &hp) {
    QVector<QPointF> newPoly;
    if(poly.isEmpty())
        return newPoly;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        QPointF current = poly[i];
        QPointF prev = poly[(i + n - 1) % n];
        bool currentInside = isInside(current, hp);
        bool prevInside = isInside(prev, hp);
        if (prevInside && currentInside) {
            newPoly.append(current);
        } else if (prevInside && !currentInside) {
            QPointF direction = current - prev;
            qreal t = - dot(prev - hp.p, hp.normal) / dot(direction, hp.normal);
            QPointF intersectPoint = prev + direction * t;
            newPoly.append(intersectPoint);
        } else if (!prevInside && currentInside) {
            QPointF direction = current - prev;
            qreal t = - dot(prev - hp.p, hp.normal) / dot(direction, hp.normal);
            QPointF intersectPoint = prev + direction * t;
            newPoly.append(intersectPoint);
            newPoly.append(current);
        }
    }
    return newPoly;
}

// Вычисление площади многоугольника по формуле Гаусса (метод шевроле)
inline qreal polygonArea(const QVector<QPointF> &poly) {
    qreal area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        area += cross(poly[i], poly[(i+1) % n]);
    }
    return qAbs(area) / 2.0;
}

}

#endif
