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

	// Check current ClusterAnalysisData
	{
		common::TimeKeeper::getTimeKeeper().update();
		const double FORCED_ENERGY = 0.5;
		const double DELTA = 0.0000001;
		structures::Cluster cluster;
		cluster.setEnergy(FORCED_ENERGY);
		ClusterArchitect cluster_architect(cluster);
		cluster_architect.runAnalysis();
		const ClusterAnalysisData & cad = cluster_architect.getCurrentClusterAnalysisData();
		const double energy = cad.getClusterRangeEnergy().energy;
		const common::Cycle start_cyc = cad.getClusterRangeEnergy().startCycle;
		const common::Cycle end_cyc = cad.getClusterRangeEnergy().endCycle;

		ASSERT_EQUAL_DELTA(FORCED_ENERGY, energy, DELTA);
		ASSERT(start_cyc>0);
		ASSERT(end_cyc>=start_cyc);

	}
}

void ClusterAnalyserBasicTest::calculateRangeEnergies() {

	ClusterAnalyserBasic cab;
	std::list<ClusterAnalysisData> history;
	const double DELTA = 0.000001;
	// ClusterAnalysisData(RangeEnergy clusterRangeEnergy, double node_creation_weight, double node_destruction_weight,
	//	double conn_creation_weight, double conn_destruction_weight, int node_create, int nodes_destroy,
	//	int conn_create, int conn_destroy);
	ClusterAnalysisData::RangeEnergy re1(1.0, common::Cycle(1), common::Cycle(10), 0.1, 0.11);
	ClusterAnalysisData::RangeEnergy re2(2.0, common::Cycle(2), common::Cycle(20), 0.2, 0.22);
	ClusterAnalysisData::RangeEnergy re3(3.0, common::Cycle(3), common::Cycle(30), 0.3, 0.33);
	ClusterAnalysisData::RangeEnergy re4(4.0, common::Cycle(4), common::Cycle(40), 0.4, 0.44);
	ClusterAnalysisData cad1(re1, 0.1, 0.11, 0.111, 0.1111, 1, 11, 111, 1111);
	ClusterAnalysisData cad2(re2, 0.2, 0.22, 0.222, 0.2222, 2, 22, 222, 2222);
	ClusterAnalysisData cad3(re3, 0.3, 0.33, 0.333, 0.3333, 3, 33, 333, 3333);
	ClusterAnalysisData cad4(re4, 0.4, 0.44, 0.444, 0.4444, 4, 44, 444, 4444);

	history.push_back(cad1);
	history.push_back(cad2);
	history.push_back(cad3);
	history.push_back(cad4);

	ClusterAnalysisData cad_final = cab.calculateRangeEnergies(history);

	// Check averages, energy and weights
	{
		double energy_exp = (1.0 + 2.0 + 3.0 + 4.0) / 4.0;
		common::Cycle cycstart_exp = common::Cycle(1);
		common::Cycle cycend_exp = common::Cycle(40);

		ASSERT_EQUAL_DELTA(energy_exp, cad_final.getClusterRangeEnergy().energy, DELTA);
		ASSERT_EQUAL(cycstart_exp, cad_final.getClusterRangeEnergy().startCycle);
		ASSERT_EQUAL(cycend_exp, cad_final.getClusterRangeEnergy().endCycle);

		double min_exp = (0.1);
		double max_exp = (0.44);
		ASSERT_EQUAL(min_exp, cad_final.getClusterRangeEnergy().energyMin);
		ASSERT_EQUAL(max_exp, cad_final.getClusterRangeEnergy().energyMax);

		double node_create_wt_exp = (0.1 + 0.2 + 0.3 + 0.4) / 4.0;
		double node_des_wt_exp = (0.11 + 0.22 + 0.33 + 0.44) / 4.0;
		double com_create_wt_exp = (0.111 + 0.222 + 0.333 + 0.444) / 4.0;
		double com_des_wt_exp = (0.1111 + 0.2222 + 0.3333 + 0.4444) / 4.0;
		ASSERT_EQUAL_DELTA(node_create_wt_exp, cad_final.getNodeCreationWeight(), DELTA);
		ASSERT_EQUAL_DELTA(node_des_wt_exp, cad_final.getNodeDestructionWeight(), DELTA);
		ASSERT_EQUAL_DELTA(com_create_wt_exp, cad_final.getConnectionCreationWeight(), DELTA);
		ASSERT_EQUAL_DELTA(com_des_wt_exp, cad_final.getConnectionDestructionWeight(), DELTA);

		int nodes_created_exp = 1 + 2 + 3 + 4;
		int nodes_des_exp = 11 + 22 + 33 + 44;
		int con_created_exp = 111 + 222 + 333 + 444;
		int con_des_exp = 1111 + 2222 + 3333 + 4444;

		ASSERT_EQUAL(nodes_created_exp, cad_final.getNodesToCreate());
		ASSERT_EQUAL(nodes_des_exp, cad_final.getNodesToDestroy());
		ASSERT_EQUAL(con_created_exp, cad_final.getConnectionsToCreate());
		ASSERT_EQUAL(con_des_exp, cad_final.getConnectionsToDestroy());

	}
}

boost::shared_ptr<ClusterArchitect> ClusterAnalyserBasicTest::createTestClusterArchitect(structures::Cluster & clus) {
	const int history_sz = 2;
	const int step_factor = 3;
	// set up 3 length history
	boost::shared_ptr<ClusterArchitect> cluster_architect(new ClusterArchitect(clus, history_sz, step_factor));

	// need 3 current,
	{
		ClusterAnalysisData data1(0.01);
		ClusterAnalysisData data2(0.02);
		ClusterAnalysisData data3(0.03);

		std::list<ClusterAnalysisData> temp_current_history;
		temp_current_history.push_back(data1);
		temp_current_history.push_back(data2);
		temp_current_history.push_back(data3);
		cluster_architect->setCurrentHistory(temp_current_history);
	}
	// and in histories,3 short, 3 medium, 3 long
	{
		ClusterAnalysisData data11(0.011);
		ClusterAnalysisData data12(0.012);
		ClusterAnalysisData data13(0.013);

		std::list<ClusterAnalysisData> temp_history1;
		temp_history1.push_back(data11);
		temp_history1.push_back(data12);
		temp_history1.push_back(data13);

		ClusterAnalysisData data21(0.021);
		ClusterAnalysisData data22(0.022);
		ClusterAnalysisData data23(0.023);

		std::list<ClusterAnalysisData> temp_history2;
		temp_history2.push_back(data21);
		temp_history2.push_back(data22);
		temp_history2.push_back(data23);

		ClusterAnalysisData data31(0.031);
		ClusterAnalysisData data32(0.032);
		ClusterAnalysisData data33(0.033);

		std::list<ClusterAnalysisData> temp_history3;
		temp_history3.push_back(data31);
		temp_history3.push_back(data32);
		temp_history3.push_back(data33);

		std::map<int, std::list<ClusterAnalysisData> > histories_map;
		histories_map[3] = temp_history1;
		histories_map[6] = temp_history2;
		histories_map[12] = temp_history3;

		cluster_architect->setHistories(histories_map);
	}

	return cluster_architect;
}

} /* namespace manipulators */
} /* namespace cryomesh */