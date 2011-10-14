/*
 * ClusterArchitectTest.cpp
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

//#define CLUSTERARCHITECTTEST_DEBUG

#include "ClusterArchitectTest.h"
#include "manipulators/ClusterArchitect.h"
#include "structures/Cluster.h"

using namespace cryomesh::structures;

namespace cryomesh {
namespace manipulators {

void ClusterArchitectTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterArchitectTest::testCreateRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testCreateRandomConnections));
	s.push_back(CUTE(ClusterArchitectTest::testGetRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testGetRandomConnections));
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomConnections));
	s.push_back(CUTE(ClusterArchitectTest::testAddHistoryEntry));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterArchitectTest");
}

void ClusterArchitectTest::testCreateRandomNodes() {

// test self connect strategy
	{
		Cluster cluster;
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		std::set<boost::shared_ptr<cryomesh::components::Node> > new_nodes = cluster_arch->createRandomNodes(NEW_SIZE,
				NEW_CONNECTIVITY, ClusterArchitect::ENABLE_SELF_CONNECT);
		ASSERT_EQUAL(NEW_SIZE, new_nodes.size());

		int variation_low_insz = 0;
		int variation_high_insz = 0;
		int variation_low_outsz = 0;
		int variation_high_outsz = 0;

		// forall in new_nodes
		{

			int count = 0;
			std::set<boost::shared_ptr<cryomesh::components::Node> >::const_iterator it_new_nodes = new_nodes.begin();
			const std::set<boost::shared_ptr<cryomesh::components::Node> >::const_iterator it_new_nodes_end =
					new_nodes.end();
			while (it_new_nodes != it_new_nodes_end) {
				const int insz = (*it_new_nodes)->getConnector().getInputs().size();
				const int outsz = (*it_new_nodes)->getConnector().getOutputs().size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
				std::cout << "ClusterArchitectTest::testCreateRandomNodes: " << "insz: " << insz << " outsz: " << outsz
						<< std::endl;
#endif
				ASSERT( insz >=(0.5*NEW_CONNECTIVITY));
				ASSERT( insz <= (2*NEW_CONNECTIVITY));
				ASSERT( outsz >=(0.5*NEW_CONNECTIVITY));
				ASSERT( outsz <=(2*NEW_CONNECTIVITY));

				if (count == 0) {
					variation_low_insz = insz;
					variation_high_insz = insz;
					variation_low_outsz = outsz;
					variation_high_outsz = outsz;
				}
				variation_low_insz = (insz < variation_low_insz ? insz : variation_low_insz);
				variation_high_insz = (insz > variation_high_insz ? insz : variation_high_insz);
				variation_low_outsz = (outsz < variation_low_outsz ? outsz : variation_low_outsz);
				variation_high_outsz = (insz > variation_high_outsz ? outsz : variation_high_outsz);

#ifdef CLUSTERARCHITECTTEST_DEBUG
				std::cout << "ClusterArchitectTest::testCreateRandomNodes: " << "insz: " << insz << " outsz: " << outsz
						<< " variation_insz: " << variation_low_insz << "/" << variation_high_insz
						<< " variation_outsz: " << variation_low_outsz << "/" << variation_high_outsz << std::endl;
#endif

				// Run check on all inputs
				{
					const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> > & all_cons =
							(*it_new_nodes)->getConnector().getInputs();

#ifdef CLUSTERARCHITECTTEST_DEBUG
					//	std::cout<<"ClusterArchitectTest::testCreateRandomNodes: "<<(*it_new_nodes)->getConnector()<<std::endl;
#endif
					int self_connects = 0;
					int multiple_connects = 0;
					int max_multiple_connect = 0;

					std::map<boost::uuids::uuid, int> node_map_count;

					std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator it_all_cons =
							all_cons.begin();
					const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator it_all_cons_end =
							all_cons.end();
					while (it_all_cons != it_all_cons_end) {
						boost::uuids::uuid other_end =
								it_all_cons->second->getConnector().getInputs().begin()->second->getUUID();
						std::map<boost::uuids::uuid, int>::iterator it_found = node_map_count.find(other_end);
						if (it_found != node_map_count.end()) {
							it_found->second += 1;
							++multiple_connects;
							if (it_found->second > max_multiple_connect) {
								max_multiple_connect = it_found->second;
							}
						} else {
							node_map_count[other_end] = 1;
						}

						if (other_end == (*it_new_nodes)->getUUID()) {
							++self_connects;
						}
						++it_all_cons;
					}

#ifdef CLUSTERARCHITECTTEST_DEBUG
					std::cout << "ClusterArchitectTest::testCreateRandomNodes: Check inputs... " << "self_connects: "
							<< self_connects << " multiple_connects: " << multiple_connects << " max_multiple_connect: "
							<< max_multiple_connect << std::endl;
#endif
				}

				// Run check on all outputs
				{
					const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> > & all_cons =
							(*it_new_nodes)->getConnector().getOutputs();

					int self_connects = 0;
					int multiple_connects = 0;
					int max_multiple_connect = 0;

					std::map<boost::uuids::uuid, int> node_map_count;

					std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator it_all_cons =
							all_cons.begin();
					const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator it_all_cons_end =
							all_cons.end();
					while (it_all_cons != it_all_cons_end) {
						boost::uuids::uuid other_end =
								it_all_cons->second->getConnector().getOutputs().begin()->second->getUUID();
						std::map<boost::uuids::uuid, int>::iterator it_found = node_map_count.find(other_end);
						if (it_found != node_map_count.end()) {
							it_found->second += 1;
							++multiple_connects;
							if (it_found->second > max_multiple_connect) {
								max_multiple_connect = it_found->second;
							}
						} else {
							node_map_count[other_end] = 1;
						}

						if (other_end == (*it_new_nodes)->getUUID()) {
							++self_connects;
						}
						++it_all_cons;
					}

#ifdef CLUSTERARCHITECTTEST_DEBUG
					std::cout << "ClusterArchitectTest::testCreateRandomNodes: Check outputs... " << "self_connects: "
							<< self_connects << " multiple_connects: " << multiple_connects << " max_multiple_connect: "
							<< max_multiple_connect << std::endl;
#endif
				}

				++count;
				++it_new_nodes;
			}ASSERT_EQUAL_DELTA(0, variation_high_insz - variation_low_insz, (0.1*NEW_CONNECTIVITY));
			ASSERT_EQUAL_DELTA(0, variation_high_outsz - variation_low_outsz, (0.1*NEW_CONNECTIVITY));
		}

	}

}

void ClusterArchitectTest::testCreateRandomConnections() {

	const int CON_COUNT = 20;
	const int NEW_SIZE = 10;
	const int NEW_CONNECTIVITY = 20;
	Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);

	boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

	const int PRE_CONN_COUNT = cluster.getConnections().size();
	cluster_arch->createRandomConnections(CON_COUNT);
	const int POST_CONN_COUNT = cluster.getConnections().size();

	ASSERT_EQUAL(PRE_CONN_COUNT+CON_COUNT, POST_CONN_COUNT);

}

void ClusterArchitectTest::testDestroyRandomNodes() {

	// test normal kill
	{
		const int COUNT = 2;

		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		const int PRE_REAL_COUNT = cluster.getNodeMap().getSize();
		const int PRE_REAL_COUNT_CONNS = cluster.getConnections().size();
		cluster_arch->destroyRandomNodes(COUNT);
		const int POST_REAL_COUNT = cluster.getNodeMap().getSize();
		const int POST_REAL_COUNT_CONNS = cluster.getConnections().size();

		ASSERT_EQUAL(PRE_REAL_COUNT - COUNT, POST_REAL_COUNT);
		ASSERT(PRE_REAL_COUNT_CONNS>POST_REAL_COUNT_CONNS);
	}

	// test over kill
	{
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		const int COUNT = 2 * NEW_SIZE;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		//const int PRE_REAL_COUNT = cluster.getNodeMap().getSize();
		//const int PRE_REAL_COUNT_CONNS = cluster.getConnections().size();
		cluster_arch->destroyRandomNodes(COUNT);
		const int POST_REAL_COUNT = cluster.getNodeMap().getSize();
		const int POST_REAL_COUNT_CONNS = cluster.getConnections().size();

		ASSERT_EQUAL(0, POST_REAL_COUNT);
		ASSERT_EQUAL(0, POST_REAL_COUNT_CONNS);
	}

}

void ClusterArchitectTest::testDestroyRandomConnections() {
	// test normal kill
	{
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		const int COUNT = 2;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		const int PRE_REAL_COUNT = cluster.getNodeMap().getSize();
		const int PRE_REAL_COUNT_CONNS = cluster.getConnections().size();
		cluster_arch->destroyRandomConnections(COUNT);
		const int POST_REAL_COUNT = cluster.getNodeMap().getSize();
		const int POST_REAL_COUNT_CONNS = cluster.getConnections().size();

		ASSERT_EQUAL(PRE_REAL_COUNT, POST_REAL_COUNT);
		ASSERT_EQUAL(PRE_REAL_COUNT_CONNS - COUNT, POST_REAL_COUNT_CONNS);
	}
	// test over kill
	{
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		const int COUNT = 2 * NEW_SIZE * NEW_CONNECTIVITY;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		const int PRE_REAL_COUNT = cluster.getNodeMap().getSize();
		//const int PRE_REAL_COUNT_CONNS = cluster.getConnections().size();
		cluster_arch->destroyRandomConnections(COUNT);
		const int POST_REAL_COUNT = cluster.getNodeMap().getSize();
		const int POST_REAL_COUNT_CONNS = cluster.getConnections().size();

		ASSERT_EQUAL(PRE_REAL_COUNT, POST_REAL_COUNT);
		ASSERT_EQUAL(0, POST_REAL_COUNT_CONNS);
	}
}

void ClusterArchitectTest::testGetRandomNodes() {
	// test non prime
	{
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		const int COUNT = 2;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > random_nodes = cluster_arch->getRandomNodes(
				COUNT, false);
		const int random_node_count = random_nodes.size();
		ASSERT_EQUAL(COUNT, random_node_count);
		//ASSERT_EQUAL(COUNT+NEW_SIZE, cluster.getNodeMap().getSize());

		// forall in random_nodes
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_random_nodes =
					random_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_random_nodes_end =
					random_nodes.end();
			while (it_random_nodes != it_random_nodes_end) {
				ASSERT_EQUAL(false, it_random_nodes->second->isPrimaryInputAttachedNode());
				ASSERT_EQUAL(false, it_random_nodes->second->isPrimaryOutputAttachedNode());
				++it_random_nodes;
			}
		}

	}

}

void ClusterArchitectTest::testGetRandomConnections() {
	// test non prime
	{
		const int NEW_SIZE = 10;
		const int NEW_CONNECTIVITY = 20;
		const int COUNT = 20;
		Cluster cluster(NEW_SIZE, NEW_CONNECTIVITY);
		boost::shared_ptr<ClusterArchitect> cluster_arch = cluster.getMutableClusterArchitect();

		//const int PRE_CONNS_COUNT = cluster.getConnections().size();
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Connection> > random_conns =
				cluster_arch->getRandomConnections(COUNT, false);
		const int random_conns_count = random_conns.size();
		ASSERT_EQUAL(COUNT, random_conns_count);
		//const int POST_CONNS_COUNT = cluster.getConnections().size();
		//ASSERT_EQUAL(PRE_CONNS_COUNT+COUNT, POST_CONNS_COUNT);

		// forall in random_conns
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Connection> >::const_iterator it_random_conns =
					random_conns.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Connection> >::const_iterator it_random_conns_end =
					random_conns.end();
			while (it_random_conns != it_random_conns_end) {
				ASSERT_EQUAL(false, it_random_conns->second->isPrimaryInputConnection());
				ASSERT_EQUAL(false, it_random_conns->second->isPrimaryOutputConnection());
				++it_random_conns;
			}
		}

	}
}

void ClusterArchitectTest::testAddHistoryEntry() {
	const int STEPPING = 2;
	const int MAX_ENTRIES = 10;

	Cluster cluster;
	ClusterArchitect cluster_architect(cluster, MAX_ENTRIES, STEPPING);
	const std::map<int, std::list<ClusterAnalysisData> > & histories = cluster_architect.getHistories();
	const std::list<ClusterAnalysisData> & current_history = cluster_architect.getCurrentHistory();

	int count = 1;

	// almost fill up current history the first time
	for (int i = 0; i < MAX_ENTRIES - 1; i++) {
		cluster.setEnergy(i);
		cluster_architect.runAnalysis();
		const int histories_sz = histories.size();
		const int current_history_sz = current_history.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
				<< current_history_sz << " histories_sz: " << histories_sz << std::endl;
#endif
		ASSERT_EQUAL(0, histories_sz);
		ASSERT_EQUAL(i+1, current_history_sz);
		++count;
		common::TimeKeeper::getTimeKeeper().update();
}

	// fill up the current history the first time

	for (int i = 0; i < 1; i++) {
		cluster.setEnergy(i);
		cluster_architect.runAnalysis();
		const int histories_sz = histories.size();
		const int current_history_sz = current_history.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
	std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
				<< current_history_sz << " histories_sz: " << histories_sz << std::endl;
#endif
		ASSERT_EQUAL(1, histories_sz);
		ASSERT_EQUAL(MAX_ENTRIES, current_history_sz);
		++count;
		common::TimeKeeper::getTimeKeeper().update();
	}

	std::map<int,  std::list<ClusterAnalysisData> >::const_iterator it_step1_history = histories.find(MAX_ENTRIES);
	ASSERT(it_step1_history != histories.end());

	// almost fill up step1 history the first time
	for (int i = 0; i < MAX_ENTRIES -1 ; i++) {
		cluster.setEnergy(i);
		cluster_architect.runAnalysis();
		const int histories_sz = histories.size();
		const int current_history_sz = current_history.size();
		const int step1_history_sz = it_step1_history->second.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
				<< current_history_sz << " histories_sz: " << histories_sz << " step1_history_sz: " << step1_history_sz
				<< std::endl;
#endif
		ASSERT_EQUAL(1, histories_sz);
		ASSERT_EQUAL(MAX_ENTRIES, current_history_sz);
		ASSERT_EQUAL(1, step1_history_sz);
		++count;
		common::TimeKeeper::getTimeKeeper().update();
	}

	//  fill up step1 history the first time
	for (int i = 0; i < 1; i++) {
		cluster.setEnergy(i);
		cluster_architect.runAnalysis();
		const int histories_sz = histories.size();
		const int current_history_sz = current_history.size();
		const int step1_history_sz = it_step1_history->second.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
	std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
				<< current_history_sz << " histories_sz: " << histories_sz << " step1_history_sz: " << step1_history_sz
				<< std::endl;
#endif
		ASSERT_EQUAL(2, histories_sz);
		ASSERT_EQUAL(MAX_ENTRIES, current_history_sz);
		ASSERT_EQUAL(2, step1_history_sz);
		++count;
		common::TimeKeeper::getTimeKeeper().update();
	}

	const int STEP2_KEY =MAX_ENTRIES*STEPPING;
	std::map<int,  std::list<ClusterAnalysisData> >::const_iterator it_step2_history = histories.find(STEP2_KEY);
		ASSERT(it_step2_history != histories.end());

		// almost fill up step2 history the first time
		for (int i = 0; i < MAX_ENTRIES -1 ; i++) {
			cluster.setEnergy(i);
			cluster_architect.runAnalysis();
			const int histories_sz = histories.size();
			const int current_history_sz = current_history.size();
			const int step2_history_sz = it_step2_history->second.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
			std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
					<< current_history_sz << " histories_sz: " << histories_sz << " step2_history_sz: " << step2_history_sz
					<< std::endl;
#endif
			ASSERT_EQUAL(2, histories_sz);
			ASSERT_EQUAL(MAX_ENTRIES, current_history_sz);
			ASSERT_EQUAL(1, step2_history_sz);
			++count;
			common::TimeKeeper::getTimeKeeper().update();
		}

		//  fill up step2 history the first time
		for (int i = 0; i < 1; i++) {
			cluster.setEnergy(i);
			cluster_architect.runAnalysis();
			const int histories_sz = histories.size();
			const int current_history_sz = current_history.size();
			const int step2_history_sz = it_step2_history->second.size();
#ifdef CLUSTERARCHITECTTEST_DEBUG
	std::cout << "ClusterArchitectTest::testAddHistoryEntry: " << "count: " << count << " current_history_sz: "
					<< current_history_sz << " histories_sz: " << histories_sz << " step2_history_sz: " << step2_history_sz
					<< std::endl;
#endif
			ASSERT_EQUAL(3, histories_sz);
			ASSERT_EQUAL(MAX_ENTRIES, current_history_sz);
			ASSERT_EQUAL(2, step2_history_sz);
			++count;
			common::TimeKeeper::getTimeKeeper().update();
		}

}
} /* namespace manipulators */
} /* namespace cryomesh */
