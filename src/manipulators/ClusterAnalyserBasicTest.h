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
#include "manipulators/ClusterAnalysisData.h"

namespace cryomesh {
namespace manipulators {

class ClusterAnalyserBasicTest {
public:
	ClusterAnalyserBasicTest() {
	}
	virtual ~ClusterAnalyserBasicTest() {
	}

	static void runSuite();
	static void testAnalyseCluster();
	static void testCalculateRangeEnergies();
	static void testRestructuringEnabling();

	static boost::shared_ptr<ClusterArchitect> createTestClusterArchitect(structures::Cluster & clus);
	static bool assertStructuring(const ClusterAnalysisData & cad, const int node_creates = 0, const int node_destroys =
			0, const int conn_creates = 0, const int conn_destroys = 0);
	static bool assertHistoriesStructure(const std::map<int, std::list<ClusterAnalysisData> > & hist
			, const unsigned int hist_sz, const unsigned int step_sz, const unsigned int total_sz , const unsigned int dhort_sz
			, const unsigned  int medium_sz, const unsigned  int long_sz);
};

} /* namespace manipulators */
} /* namespace cryomesh */
#endif /* CLUSTERANALYSERBASICTEST_H_ */
