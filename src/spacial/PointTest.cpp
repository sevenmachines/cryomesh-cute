#include "PointTest.h"

namespace cryomesh{
namespace spacial{

void PointTest::runSuite(){
	cute::suite s;
	s.push_back(CUTE(PointTest::testPoint_SphericalPoint));
	s.push_back(CUTE(PointTest::testPoint_CC));
	s.push_back(CUTE(PointTest::testgetDistance));
	s.push_back(CUTE(PointTest::testoperators));
	s.push_back(CUTE(PointTest::testgetNormalisedVector));
cute::ide_listener lis;
	cute::makeRunner(lis)(s, "PointTest");
}
void PointTest::testPoint_SphericalPoint()
	{
		// good point
		{
			SphericalPoint sp( 2.3, Radian(0.8), Radian(1.2));
			Point p(sp);
			double x = 0.597860947;
			double y = 1.537789005;
			double z = 1.602425431;
			ASSERT_EQUAL_DELTA(x, p.getX(), 0.0001);
			ASSERT_EQUAL_DELTA(y, p.getY(), 0.0001);
			ASSERT_EQUAL_DELTA(z, p.getZ(), 0.0001);
		}

	}
	void PointTest::testPoint_CC()
	{
		// good point
		{
			Point sp1( 2.3, 0.8, 1.2);
			Point sp2(sp1);
			bool equal = sp1 == sp2;

			ASSERT(equal);
		}
	}
	void PointTest::testgetDistance()
	{
		Point p1(1,2,3);
		Point p2(3,2,1);
		double dist1 = p2.getDistance(p1);
		double dist2 = p1.getDistance(p2);
		ASSERT_EQUAL_DELTA(2.828427125, dist1, 0.00001);
		ASSERT_EQUAL_DELTA(dist1, dist2, 0.00001);

	}
	void PointTest::testoperators() {
		Point p1(1,1,1);
		Point p2(2,2,2);
		Point p3(3,3,3);

		bool plus = p1+p2 == p3;
		bool minus = p3-p2 == p1;
		ASSERT(plus);
		ASSERT(minus);

		//operator*
		{
			double mult = 2;
			Point p(1,1,1);
			Point pm = p * mult;
			bool eq = pm == Point(2,2,2);
			ASSERT(eq);
		}
	}
	void PointTest::testgetNormalisedVector() {
		Point p(3.232, 14.344, 66.34);
		Point norm_vector = p.getNormalisedVector();
		double dist = norm_vector.getDistance(Point(0,0,0));
		//std::cout<<"testgetNormalisedVector: dist: "<<dist<<std::endl;
		//std::cout << norm_vector<<std::endl;
		ASSERT_EQUAL_DELTA(1, dist, 0.0001);
	}

}//NAMESPACE
}//NAMESPACE

