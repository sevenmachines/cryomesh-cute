/*
 * OneNodeSystemTest.cpp
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */
#define MINIMUMNODESYSTEMTEST_DEBUG

#include "MinimumNodeSystemTest.h"
#include "structures/Bundle.h"
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
	//const int SIZE = 2;
	manager::CryoManager manager;
	manager.create("TestData/basic-1c-5n.config");
	const int NODE_COUNT = 5;
	const int CLUSTER_COUNT = 1;
	const int PIN_COUNT = 1;
	const int POUT_COUNT = 1;
	boost::shared_ptr<structures::Bundle> bundle = manager.getMutableBundle();

	// check cluster
	int cluster_count = bundle->getClusters().getSize();
	ASSERT_EQUAL(CLUSTER_COUNT, cluster_count);

	boost::shared_ptr<Cluster> cluster = bundle->getClusters().begin()->second;

	// get node components
	int node_count = cluster->getNodes().size();
	ASSERT_EQUAL(NODE_COUNT, node_count);
	std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > nodes =
			cluster->getMutableNodeMap().getCollection();
	std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::iterator it_nodes = nodes.begin();
	boost::shared_ptr<Node> node1 = it_nodes->second;
	++it_nodes;
	boost::shared_ptr<Node> node2 = it_nodes->second;
	++it_nodes;
	boost::shared_ptr<Node> node3 = it_nodes->second;
	++it_nodes;
	boost::shared_ptr<Node> node4 = it_nodes->second;
	++it_nodes;
	boost::shared_ptr<Node> node5 = it_nodes->second;
	// test structure
	{
		int pins = 0;
		int pouts = 0;
		// forall in nodes
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_nodes = nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_nodes_end =
					nodes.end();
			while (it_nodes != it_nodes_end) {

				// check isolation
				ASSERT(it_nodes->second->isInputIsolated()==false);
				ASSERT(it_nodes->second->isOutputIsolated()==false);

				// check for primaries
				if (it_nodes->second->isPrimaryInputAttachedNode()) {
					++pins;
				}
				if (it_nodes->second->isPrimaryOutputAttachedNode()) {
					++pouts;
				}
				++it_nodes;
			}
		}

#ifdef MINIMUMNODESYSTEMTEST_DEBUG
		printAllNodes(nodes);
#endif
		ASSERT_EQUAL(PIN_COUNT, pins);
		ASSERT_EQUAL(POUT_COUNT, pouts);

	}

	//setup for running
	{
		//node1 ->setDebug(false);
		//node2->setDebug(false);
		setAllInputConnectionsDebug(node1, false);
		setAllOutputConnectionsDebug(node1, false);
		setAllInputConnectionsDebug(node2, false);
		setAllOutputConnectionsDebug(node2, false);
		//bundle->setDebug(false);
		//cluster->setDebug(false);
	}
	// input pattern sequence is 1x2x1{1,0}
	// start propagating the input pattern

	// update 1 - pattern applied to fibre
	{
		//	setAllInputConnectionsDebug(node1, true);
		bundle->update();
#ifdef MINIMUMNODESYSTEMTEST_DEBUG
		printAllNodes(nodes);
#endif
	}
	// update 2 - pattern moves through fibre connections
	{
		bundle->update();
#ifdef MINIMUMNODESYSTEMTEST_DEBUG
		printAllNodes(nodes);
#endif
	}
	// patterns begin to be applied to nodes
	{
		bundle->update();
#ifdef MINIMUMNODESYSTEMTEST_DEBUG
		printAllNodes(nodes);
#endif
	}
	// more updates
	{
		bundle->update();
#ifdef MINIMUMNODESYSTEMTEST_DEBUG
		printAllNodes(nodes);
#endif
	}
	// impulses build in nodes
	// check for node firings
	// check fired nodes send impulses to their out connections
	//check impulses move along nodes out connections
	// check impulses reach secondary nodes

	// a bunch of updates
	{
	//	node1->setDebug(true);
		const int UPDATE_COUNT = 10;
		for (int i = 0; i < UPDATE_COUNT; i++) {
			bundle->update();
#ifdef MINIMUMNODESYSTEMTEST_DEBUG
			printAllNodes(nodes);
#endif
		}
		//node1->setDebug(false);
	}
}

bool MinimumNodeSystemTest::checkNodeHasImpulses(const boost::shared_ptr<Node> & node) {
	bool success = false;

	return success;
}
bool MinimumNodeSystemTest::checkNodePassesImpulses(const boost::shared_ptr<Node> & node) {
	bool success = false;

	return success;
}
bool MinimumNodeSystemTest::checkConnectionHasImpulses(const boost::shared_ptr<Connection> & connection) {
	bool success = false;

	return success;
}
bool MinimumNodeSystemTest::checkConnectionPassesImpulses(const boost::shared_ptr<Connection> & connection) {
	bool success = false;

	return success;
}

void MinimumNodeSystemTest::setAllConnectionsDebug(std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & cons,
		bool debug) {
	// forall in cons
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::iterator it_cons = cons.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_cons_end = cons.end();
		while (it_cons != it_cons_end) {
			it_cons->second->enableDebug(debug);
			++it_cons;
		}
	}
}

void MinimumNodeSystemTest::setAllInputConnectionsDebug(boost::shared_ptr<Node> node, bool debug) {
	std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & cons =
			node->getMutableConnector().getMutableInputs();
	// forall in cons
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::iterator it_cons = cons.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_cons_end = cons.end();
		while (it_cons != it_cons_end) {
			it_cons->second->enableDebug(debug);
			++it_cons;
		}
	}
}

void MinimumNodeSystemTest::setAllOutputConnectionsDebug(boost::shared_ptr<Node> node, bool debug) {
	std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & cons =
			node->getMutableConnector().getMutableOutputs();
	// forall in cons
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::iterator it_cons = cons.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_cons_end = cons.end();
		while (it_cons != it_cons_end) {
			it_cons->second->enableDebug(debug);
			++it_cons;
		}
	}
}

void MinimumNodeSystemTest::printAllNodes(std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & nodes) {
	// forall in nodes
	{
		int node_num = 1;
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_nodes = nodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_nodes_end =
				nodes.end();
		while (it_nodes != it_nodes_end) {
			std::cout << node_num << ": " << *(it_nodes->second) << std::endl;
			++it_nodes;
			++node_num;
		}
	}
}
}//NAMESPACE

}//NAMESPACE
