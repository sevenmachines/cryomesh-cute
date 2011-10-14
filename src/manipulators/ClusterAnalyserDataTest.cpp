/*
 * ClusterAnalyserDataTest.cpp
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#include "ClusterAnalyserDataTest.h"
#include "manipulators/ClusterAnalysisData.h"

namespace cryomesh {
namespace manipulators {

void ClusterAnalyserDataTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterAnalyserDataTest::testRangeEnergy));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterAnalyserDataTest");
}

void ClusterAnalyserDataTest::testRangeEnergy() {
	const double DELTA = 0.000001;
	const double ENERGY1 = 1;
	const common::Cycle CYCLE_START1 = 2;
	const common::Cycle CYCLE_END1 = 3;
	const int ENERGY_MIN1 = 10;
	const int ENERGY_MAX1 = 100;

	const double ENERGY2 = 2;
	const common::Cycle CYCLE_START2 = 1;
	const common::Cycle CYCLE_END2 = 4;
	const double ENERGY_MIN2 = 5;
	const double ENERGY_MAX2 = 200;

	ClusterAnalysisData::RangeEnergy re1(ENERGY1, CYCLE_START1, CYCLE_END1, ENERGY_MIN1, ENERGY_MAX1);
	ClusterAnalysisData::RangeEnergy re2(ENERGY2, CYCLE_START2, CYCLE_END2, ENERGY_MIN2, ENERGY_MAX2);

	ClusterAnalysisData::RangeEnergy re1re2 = re1 + re2;

	// re1re2
	{
		ASSERT_EQUAL_DELTA( (ENERGY1+ENERGY2), re1re2.energy, DELTA);
		ASSERT_EQUAL(CYCLE_START2, re1re2.startCycle);
		ASSERT_EQUAL(CYCLE_END2, re1re2.endCycle);
		ASSERT_EQUAL_DELTA(ENERGY_MIN2, re1re2.energyMin, DELTA);
		ASSERT_EQUAL_DELTA(ENERGY_MAX2, re1re2.energyMax, DELTA);

		ClusterAnalysisData::RangeEnergy re1re2div = re1re2 / 2;
		ASSERT_EQUAL_DELTA( (ENERGY1+ENERGY2)/2, re1re2div.energy, DELTA);
		ASSERT_EQUAL(CYCLE_START2, re1re2div.startCycle);
		ASSERT_EQUAL(CYCLE_END2, re1re2div.endCycle);
		ASSERT_EQUAL_DELTA(ENERGY_MIN2, re1re2div.energyMin, DELTA);
		ASSERT_EQUAL_DELTA(ENERGY_MAX2, re1re2div.energyMax, DELTA);

	}

}
} /* namespace manipulators */
} /* namespace cryomesh */
