/*
 * ClusterAnalyserBasicTest.cpp
 *
 *  Created on: 14 Oct 2011
 *      Author: niall
 */

#include "ClusterAnalyserBasicTest.h"
#include "manipulators/ClusterAnalyserBasic.h"
#include "manipulators/ClusterArchitect.h"
#include "structures/Cluster.h"

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
	common::TimeKeeper::getTimeKeeper().update();
const double FORCED_ENERGY = 0.5;
	const double DELTA = 0.0000001;
	structures::Cluster cluster;
	cluster.setEnergy(FORCED_ENERGY);
	ClusterArchitect cluster_architect(cluster);
	cluster_architect.runAnalysis();

	// Check current ClusterAnalysisData
	{
		const ClusterAnalysisData & cad = cluster_architect.getCurrentClusterAnalysisData();
		const double energy = cad.getClusterRangeEnergy().energy;
		const common::Cycle start_cyc = cad.getClusterRangeEnergy().startCycle;
		const common::Cycle end_cyc = cad.getClusterRangeEnergy().endCycle;

		ASSERT_EQUAL_DELTA(FORCED_ENERGY, energy, DELTA);
		ASSERT(start_cyc>0);
		ASSERT(end_cyc>=start_cyc);

	}
	ASSERTM("TODO", false);
}

void ClusterAnalyserBasicTest::calculateRangeEnergies() {
	ASSERTM("TODO", false);
}
} /* namespace manipulators */
} /* namespace cryomesh */
