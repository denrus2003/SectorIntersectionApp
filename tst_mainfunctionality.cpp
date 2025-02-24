#include <gtest/gtest.h>
#include "intersectioncalculator.h"
#include "sector.h"

class TestSectorIntersection : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(TestSectorIntersection, ComputeIntersection) {
    QVector<Sector> sectors;
    sectors.append(Sector(QPointF(0, 0), QPointF(1, 0), 90));
    sectors.append(Sector(QPointF(1, 1), QPointF(2, 1), 135));

    QVector<QPointF> result = IntersectionCalculator::computeIntersection(sectors);
    EXPECT_FALSE(result.isEmpty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

