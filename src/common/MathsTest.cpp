/**
 * MathsTest.cpp
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "MathsTest.h"
#include "common/Maths.h"
#include <ctime>

namespace cryomesh {

namespace common {

MathsTest::MathsTest() {

}

MathsTest::~MathsTest() {
}

void MathsTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(MathsTest::testGetRandomValue));
cute::ide_listener lis;
	cute::makeRunner(lis)(s, "MathsTest");
}


void MathsTest::testGetRandomValue() {
	srand( time(NULL));
	const int LOW_INT = -100;
	const int HIGH_INT = 50;
	const double LOW_DOUBLE = -50.3;
	const double HIGH_DOUBLE = 67.432;

	// test int
	{
		for (int i = 0; i < 10; ++i) {
			int val = Maths::getRandomInteger(LOW_INT, HIGH_INT);
			//std::cout << "MathsTest::testGetRandomValue: " << "val<int>:" << val << std::endl;
			ASSERT(val>LOW_INT);
			ASSERT(val<=HIGH_INT);
		}
	}

	// test double
	{
		for (int i = 0; i < 10; ++i) {
			double val = Maths::getRandomDouble(LOW_DOUBLE, HIGH_DOUBLE);
			//std::cout << "MathsTest::testGetRandomValue: " << "val<double>:" << val << std::endl;
			ASSERT(val>LOW_DOUBLE);
			ASSERT(val<HIGH_DOUBLE);
		}

	}
	// test unit double
	{
		for (int i = 0; i < 10; ++i) {
			double val = Maths::getRandomDouble(-1.0, 1.0);
			//std::cout << "MathsTest::testGetRandomValue: " << "val<double>:" << val << std::endl;
			ASSERT(val>-1);
			ASSERT(val<1);
		}

	}

	//test bool
	{
		int count_true = 0;
		int count_false = 0;
		for (int i = 0; i< 100; i++) {
			bool ranboo = Maths::getRandomBool();
			if (ranboo == true){
				++count_true;
			}else if (ranboo == false){
				++count_false;
			}
		}

		ASSERT_EQUAL_DELTA(50, count_true, 10);
		ASSERT_EQUAL_DELTA(50, count_false, 10);
	}
}
}//NAMESPACE

}//NAMESPACE
