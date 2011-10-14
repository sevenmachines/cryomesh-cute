/*
 * ClusterAnalyserBasicTest.cpp
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#include "ClusterAnalyserBasicTest.h"

namespace cryomesh {
namespace manipulators {
void ClusterAnalyserBasicTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterAnalyserBasicTest::analyseCluster));
	s.push_back(CUTE(ClusterAnalyserBasicTest::calculateRangeEnergies));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterAnalyserBasicTest");
}

void ClusterAnalyserBasicTest::analyseCluster() {
	ASSERTM("TODO", false);
}

void ClusterAnalyserBasicTest::calculateRangeEnergies() {
	ASSERTM("TODO", false);
}
} /* namespace manipulators */
} /* namespace cryomesh */
