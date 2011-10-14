/*
 * ClusterAnalyserBasicTest.h
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#ifndef CLUSTERANALYSERBASICTEST_H_
#define CLUSTERANALYSERBASICTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {
namespace manipulators {

class ClusterAnalyserBasicTest {
public:
	ClusterAnalyserBasicTest(){}
	virtual ~ClusterAnalyserBasicTest(){}

	static void runSuite();
	static void analyseCluster();
	static void calculateRangeEnergies();
};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERANALYSERBASICTEST_H_ */
