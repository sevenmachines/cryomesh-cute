/*
 * ClusterAnalyserBasicTest.h
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#ifndef CLUSTERANALYSERBASICTEST_H_
#define CLUSTERANALYSERBASICTEST_H_

#include "ICuteSuite.h"
#include <boost/shared_ptr.hpp>
#include "manipulators/ClusterArchitect.h"
#include "structures/Cluster.h"

namespace cryomesh {
namespace manipulators {

class ClusterAnalyserBasicTest {
public:
	ClusterAnalyserBasicTest(){}
	virtual ~ClusterAnalyserBasicTest(){}

	static void runSuite();
	static void analyseCluster();
	static void calculateRangeEnergies();

	static boost::shared_ptr< ClusterArchitect > createTestClusterArchitect(structures::Cluster & clus);
};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERANALYSERBASICTEST_H_ */
