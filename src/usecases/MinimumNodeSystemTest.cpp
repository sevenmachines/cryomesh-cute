/*
 * OneNodeSystemTest.cpp
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */
#define MINIMUMNODESYSTEMTEST_DEBUG

#include "MinimumNodeSystemTest.h"
#include "structures/Bundle.h"
#include "components/Node.h"
#include "components/Connection.h"
#include "manager/CryoManager.h"

using namespace cryomesh::components;
using namespace cryomesh::structures;

namespace cryomesh {

namespace usecases {

void MinimumNodeSystemTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(MinimumNodeSystemTest::testMinimumNodeSystem));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "OneNodeSystemTest");
}

void MinimumNodeSystemTest::testMinimumNodeSystem() {
	manager::CryoManager manager;
	manager.create("TestData/basic-1c-2n.config");
	boost::shared_ptr<structures::Bundle> bundle = manager.getMutableBundle();

	// test structure
	{
		int cluster_count = bundle->getClusters().getSize();
		ASSERT_EQUAL(1, cluster_count);

		boost::shared_ptr<Cluster> cluster = bundle->getClusters().begin()->second;
		int node_count = cluster->getNodes().size();
		ASSERT_EQUAL(2, node_count);

		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > nodes =
				cluster->getMutableNodeMap().getCollection();
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::iterator it_nodes = nodes.begin();
		boost::shared_ptr<Node> node1 = it_nodes->second;
		++it_nodes;
		boost::shared_ptr<Node> node2 = it_nodes->second;

		ASSERT(node1->isInputIsolated()==false);
		ASSERT(node1->isOutputIsolated()==false);
		ASSERT(node2->isInputIsolated()==false);
		ASSERT(node2->isOutputIsolated()==false);

		// check for primaries
		{
			int pins = 0;
			int pouts = 0;
			if (node1->isPrimaryInputAttachedNode()) {
				++pins;
			}
			if (node1->isPrimaryOutputAttachedNode()) {
				++pouts;
			}
			if (node2->isPrimaryInputAttachedNode()) {
				++pins;
			}
			if (node2->isPrimaryOutputAttachedNode()) {
				++pouts;
			}

#ifdef MINIMUMNODESYSTEMTEST_DEBUG
			std::cout << "MinimumNodeSystemTest::testMinimumNodeSystem: " << "NODE1 *******************" << std::endl;
			std::cout << *node1 << std::endl << std::endl;
			std::cout << "MinimumNodeSystemTest::testMinimumNodeSystem: " << "NODE2 *******************" << std::endl;
			std::cout << *node2 << std::endl << std::endl;
#endif
			ASSERT_EQUAL(1, pins);
			ASSERT_EQUAL(1, pouts);
		}

	}

	// start propagating the input pattern
	{
		ASSERTM("TODO: Start propagating pattern and test it", false);
	}
}
}//NAMESPACE

}//NAMESPACE
