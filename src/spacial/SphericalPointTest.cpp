#include "SphericalPointTest.h"
#include "spacial/SphericalPoint.h"
#include "spacial/Point.h"
#include <iostream>
namespace cryomesh {
namespace spacial {

void SphericalPointTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(SphericalPointTest::testSphericalPoint_Point));
	s.push_back(CUTE(SphericalPointTest::testPoint_CC));
	s.push_back(CUTE(SphericalPointTest::testgetArcLength));
	s.push_back(CUTE(SphericalPointTest::testoperators));
	s.push_back(CUTE(SphericalPointTest::testgetOpposite));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SphericalPointTest");
}

void SphericalPointTest::testSphericalPoint_Point() {
	// test point
	{
		Point p1(2.2, 0.4, 5.4);
		double r = 5.844655678;
		double th = 0.39259076;
		double ph = 0.1798535;
		SphericalPoint sp1(p1);
		ASSERT_EQUAL_DELTA(r, sp1.getRadius(), 0.0001);
		ASSERT_EQUAL_DELTA(th, sp1.getTheta().getValue(), 0.0001);
		ASSERT_EQUAL_DELTA(ph, sp1.getPhi().getValue(), 0.0001);

	}
	// test bad point
	{
		Point p1(0, 0, 0);
		Point p2(SphericalPoint::MIN_ALLOWED_X, 0, 0);

		SphericalPoint sp1(p1);
		SphericalPoint sp2(p2);
		bool equal = sp1 == sp2;
		ASSERT(equal);

	}

	//test point ->sphere transformation
	{
		SphericalPoint sp1(2, Degree(45), Degree(30));
		Point p1(sp1);
		SphericalPoint sp2(p1);
		bool equal = sp1.equals(sp2);
		//std::cout<<"sp1: "<<sp1<<std::endl;
		//std::cout<<"sp2: "<<sp2<<std::endl;
		ASSERT(equal);
	}
}
void SphericalPointTest::testPoint_CC() {
	// good point
	{
		SphericalPoint sp1(2.3, Radian(0.8), Radian(1.2));
		SphericalPoint sp2(sp1);
		bool equal = sp1 == sp2;

		ASSERT(equal);
	}
}
void SphericalPointTest::testgetArcLength() {
	//some simple ones
	// same point, arc length 0
	{
		SphericalPoint p1(1, Radian(0), Radian(0));
		double arc = p1.getArcLength(p1);
		ASSERT_EQUAL_DELTA(0, arc, 0.0001);

	}
	// opposite point, arc length r
	{
		SphericalPoint p1(2, Degree(20), Degree(40));
		SphericalPoint p2 = p1.getOpposite();
		double expected = 2 * (SphericalPoint::PI * 1);
		double arc = p1.getArcLength(p2);
		ASSERT_EQUAL_DELTA(expected, arc, 0.0001);

	}
	// arbitrary point
	{
		double pre_calc_arc = 2886.45;
		SphericalPoint p1(6371.01, Radian(0.6304), Radian(0. - 1.5127));
		SphericalPoint p2(6371.01, Radian(0.5924), Radian(-2.0665));
		double arc1 = p1.getArcLength(p2);
		double arc2 = p2.getArcLength(p1);
		ASSERT_EQUAL_DELTA(pre_calc_arc, arc1, 0.1);
		ASSERT_EQUAL_DELTA(arc1, arc2, 0.1);

	}
}
void SphericalPointTest::testoperators() {
	const double DELTA = 0.0001;
	Point p1(1, 1, 1);
	Point p2(2, 2, 2);
	Point p3(3, 3, 3);

	SphericalPoint sp1(p1);
	SphericalPoint sp2(p2);
	SphericalPoint sp3(p3);
	bool plus = sp1 + sp2 == sp3;
	//std::cout<<"testoperators: minus"<<" "<<std::endl;
	//SphericalPoint sp4 = sp1+sp2;
	//std::cout<< sp4 <<std::endl;
	//std::cout<< Point(sp4) <<std::endl;
	//std::cout<<"testoperators: minus"<<" "<<std::endl;
	//SphericalPoint sp5 = sp3-sp2;
	//std::cout<< sp5 <<std::endl;
	//std::cout<< sp1 <<std::endl;
	//std::cout<< Point(sp5) <<std::endl;
	//std::cout<< Point(sp1) <<std::endl;
	//ASSERT_EQUAL_DELTA((sp3-sp2).getRadius(), sp1.getRadius(), DELTA);
	//ASSERT_EQUAL_DELTA((sp3-sp2).getTheta(), sp1.getTheta(), DELTA);
	//ASSERT_EQUAL_DELTA((sp3-sp2).getPhi(), sp1.getPhi(), DELTA);
	//ASSERT(plus);
	ASSERT((sp1 + sp2).equals(sp3));

	ASSERT((sp3 - sp2).equals(sp1));

}
void SphericalPointTest::testgetOpposite() {
	Point p1(1, 2, 2);
	Point p2 = Point(SphericalPoint(p1).getOpposite());

	double dist = p1.getDistance(p2);
	//std::cout<<"testgetOpposite: "<<" "<<std::endl;
	//std::cout <<p1<<std::endl;
	//std::cout <<p2<<std::endl;
	ASSERT_EQUAL_DELTA(6, dist, 0.0001);
}

}//NAMESPACE
}//NAMESPACE

