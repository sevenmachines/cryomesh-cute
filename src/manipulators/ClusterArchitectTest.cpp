/*
 * ClusterArchitectTest.cpp
 *
 *  Created on: 30 Sep 2011
 *      Author: niall
 */

#define CLUSTERARCHITECTTEST_DEBUG

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
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomNodes));
	s.push_back(CUTE(ClusterArchitectTest::testDestroyRandomConnections));
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
		std::list<boost::shared_ptr<cryomesh::components::Node> > new_nodes = cluster_arch->createRandomNodes(NEW_SIZE,
				NEW_CONNECTIVITY, ClusterArchitect::ENABLE_SELF_CONNECT);
		ASSERT_EQUAL(NEW_SIZE, new_nodes.size());

		int variation_low_insz = 0;
		int variation_high_insz = 0;
		int variation_low_outsz = 0;
		int variation_high_outsz = 0;

		// forall in new_nodes
		{

			int count = 0;
			std::list<boost::shared_ptr<cryomesh::components::Node> >::const_iterator it_new_nodes = new_nodes.begin();
			const std::list<boost::shared_ptr<cryomesh::components::Node> >::const_iterator it_new_nodes_end =
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
					std::cout << "ClusterArchitectTest::testCreateRandomNodes: Check inputs... " << "self_connects: " << self_connects
							<< " multiple_connects: " << multiple_connects << " max_multiple_connect: "
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
					std::cout << "ClusterArchitectTest::testCreateRandomNodes: Check outputs... " << "self_connects: " << self_connects
							<< " multiple_connects: " << multiple_connects << " max_multiple_connect: "
							<< max_multiple_connect << std::endl;
#endif
				}

				++count;
				++it_new_nodes;
			}ASSERT_EQUAL_DELTA(0, variation_high_insz - variation_low_insz, (0.1*NEW_CONNECTIVITY));
			ASSERT_EQUAL_DELTA(0, variation_high_outsz - variation_low_outsz, (0.1*NEW_CONNECTIVITY));
		}

	}
// test evenly distributed strategy
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testCreateRandomConnections() {
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testDestroyRandomNodes() {
	ASSERTM("TODO", false);
}

void ClusterArchitectTest::testDestroyRandomConnections() {
	ASSERTM("TODO", false);
}

} /* namespace manipulators */
} /* namespace cryomesh */
