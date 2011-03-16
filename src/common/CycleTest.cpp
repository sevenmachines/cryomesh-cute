/*
 * CycleTest.cpp
 *
 *  Created on: 16 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "CycleTest.h"
#include "common/Cycle.h"

namespace cryomesh {

namespace common {

void CycleTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(CycleTest::testOperators));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "CycleTest");
}

void CycleTest::testOperators() {
	const Cycle cycle1(1);
	const Cycle cycle2(2);
	const Cycle cycle3(3);
	const Cycle cycle4(4);
	const Cycle cycle5(5);

	// add +
	{
		Cycle test = cycle2 + cycle3;
		ASSERT_EQUAL(cycle5, test);
	}
	// add +
	{
		Cycle test = cycle2;
		test += cycle3;
		ASSERT_EQUAL(cycle5, test);
	}

	// -
	{
		Cycle test = cycle3 - cycle2;
		ASSERT_EQUAL(cycle1, test);
	}
	// -
	{
		Cycle test = cycle2 - cycle3;
		Cycle cycle_1(-1);
		ASSERT_EQUAL(cycle_1, test);
	}
	// -
	{
		Cycle test = cycle3;
		test -= cycle2;
		ASSERT_EQUAL(cycle1, test);
	}
	// -
	{
		Cycle test = cycle1;
		test -= cycle5;
		Cycle cycle_4(-4);
		ASSERT_EQUAL(cycle_4, test);
	}
}

}//NAMESPACE
}//NAMESPACE
