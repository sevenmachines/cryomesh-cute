/*
 * ClusterAnalyserDataTest.h
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#ifndef CLUSTERANALYSERDATATEST_H_
#define CLUSTERANALYSERDATATEST_H_

#include "ICuteSuite.h"

namespace cryomesh {
namespace manipulators {

class ClusterAnalyserDataTest {
public:
	ClusterAnalyserDataTest(){}
	virtual ~ClusterAnalyserDataTest(){}
	static void runSuite();
	static void testRangeEnergy();
};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERANALYSERDATATEST_H_ */
