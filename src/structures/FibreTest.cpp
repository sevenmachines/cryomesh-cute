/*
 * FibreTest.cpp
 *
 *  Created on: 16 Feb 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "FibreTest.h"
#include "structures/Fibre.h"
#include "structures/Cluster.h"
#include "manager/Creator.h"
#include "components/Node.h"
#include "state/Pattern.h"

namespace cryomesh {

namespace structures {

void FibreTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE( FibreTest::testCreation));
	s.push_back(CUTE( FibreTest::testTrigger));
	s.push_back(CUTE( FibreTest::testGetNodesPattern));
	s.push_back(CUTE( FibreTest::testCountConnections));
	s.push_back(CUTE( FibreTest::testImpulsePropagation));
	s.push_back(CUTE( FibreTest::testIsConnected));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "FibreTest");
}

void FibreTest::testImpulsePropagation() {
	//create start nodes and end nodes
	const int WIDTH = 1;
	const boost::shared_ptr<state::Pattern> full_pat(new state::Pattern("1"));
	const boost::shared_ptr<state::Pattern> null_pat(new state::Pattern("0"));
	boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
	boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
	boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, cluster2, 1));

	boost::shared_ptr<components::Node> node1 = fibre->getInputNodes().begin()->second;
	boost::shared_ptr<components::Node> node2 = fibre->getOutputNodes().begin()->second;
	boost::shared_ptr<components::Connection> connection1 = fibre->getConnections().getCollection().begin()->second;

	ASSERT(node1 !=0);
	ASSERT(node2 !=0);
	ASSERT(connection1 !=0);

	//node1->setDebug(true);
	//node2->setDebug(true);
	//connection1->setDebug(true);

	node1->addImpulse(components::Impulse::getTriggerImpulse());

	std::cout << "FibreTest::testImpulsePropagation: " << "NODE1 " << "cycle: "
			<< common::TimeKeeper::getTimeKeeper().getCycle() << std::endl << *node1 << std::endl << std::endl;
	std::cout << "FibreTest::testImpulsePropagation: " << "NODE2 " << "cycle: "
			<< common::TimeKeeper::getTimeKeeper().getCycle() << std::endl << *node2 << std::endl << std::endl;
	// check start structure
	{
		int node1_inputs = node1->getConnector().getInputs().size();
		int node1_outputs = node1->getConnector().getOutputs().size();
		int node2_inputs = node2->getConnector().getInputs().size();
		int node2_outputs = node2->getConnector().getOutputs().size();

		int connection_inputs = connection1->getConnector().getInputs().size();
		int connection_outputs = connection1->getConnector().getOutputs().size();

		ASSERT(node1_outputs>0);
		ASSERT(node2_inputs>0);
		ASSERT_EQUAL(1, connection_inputs);
		ASSERT_EQUAL(1, connection_outputs);
	}
	// check start state
	{
		ASSERT_EQUAL(false, node1->isTriggered());
		ASSERT_EQUAL(true,node1->isActive());
		ASSERT_EQUAL(true,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(0, connection1->getImpulses().getSize());
		ASSERT_EQUAL(*null_pat, *(fibre->getInputNodesPattern()));
		ASSERT_EQUAL(*null_pat, *(fibre->getOutputNodesPattern()));

	}
	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	//std::cout << "ConnectionTest::testImpulsePropagation: 2" << *node1 << std::endl;

	// check  state
	{
		ASSERT_EQUAL(true, node1->isTriggered());
		ASSERT_EQUAL(false,node1->isActive());
		ASSERT_EQUAL(false,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(1, connection1->getImpulses().getSize());
		ASSERT_EQUAL(*full_pat, *(fibre->getInputNodesPattern()));
		ASSERT_EQUAL(*null_pat, *(fibre->getOutputNodesPattern()));

	}
	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	//std::cout << "ConnectionTest::testImpulsePropagation: 3" << *node1 << std::endl;

	{
		ASSERT_EQUAL(false, node1->isTriggered());
		ASSERT_EQUAL(false,node1->isActive());
		ASSERT_EQUAL(false,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(1, connection1->getImpulses().getSize());
		ASSERT_EQUAL(*null_pat, *(fibre->getInputNodesPattern()));
		ASSERT_EQUAL(*null_pat, *(fibre->getOutputNodesPattern()));

	}

	// update until we pass on the connections impulse
	bool impulse_propagated = false;
	int count_limit = 1000;
	while (impulse_propagated == false && count_limit > 0) {
		common::TimeKeeper::getTimeKeeper().update();
		connection1->update();
		node1->update();
		node2->update();
		if (connection1->getImpulses().getSize() < 1) {
			impulse_propagated = true;
			std::cout << "ConnectionTest::testImpulsePropagation: " << "NODE2: " << "cycle: "
					<< common::TimeKeeper::getTimeKeeper().getCycle() << std::endl << *node2 << std::endl;
			ASSERT_EQUAL(1, node2->getImpulses().getSize());

			// adjust for delay
			boost::shared_ptr<components::Impulse> node2_imp = node2->getImpulses().begin()->second;
			int count = 0;
			int delay = node2_imp->getActivityDelay();
			while (common::TimeKeeper::getTimeKeeper().getCycle() < node2_imp->getFirstActiveCycle()) {
				common::TimeKeeper::getTimeKeeper().update();
				++count;
			}

			ASSERT_EQUAL(count, delay);
			ASSERT_EQUAL(false, node2->isTriggered());
			ASSERT_EQUAL(true,node2->isActive());
			ASSERT_EQUAL(true,node2->isLive());

		} else {
			ASSERT_EQUAL(false, node1->isTriggered());
			ASSERT_EQUAL(false,node1->isActive());
			ASSERT_EQUAL(false,node1->isLive());
			ASSERT_EQUAL(false, node2->isTriggered());
			ASSERT_EQUAL(false,node2->isActive());
			ASSERT_EQUAL(false,node2->isLive());
			ASSERT_EQUAL(*null_pat, *(fibre->getInputNodesPattern()));
			ASSERT_EQUAL(*null_pat, *(fibre->getOutputNodesPattern()));

		}
		--count_limit;
	}
	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	ASSERT_EQUAL(1, node2->getImpulses().getSize());
	ASSERT_EQUAL(false, node2->isTriggered());
	ASSERT_EQUAL(true,node2->isActive());
	ASSERT_EQUAL(true,node2->isLive());
}

void FibreTest::testCreation() {

	const int WIDTH = 10;
	// test primary in
	{

		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		Fibre fibre(cluster1, Fibre::PrimaryInputFibre, WIDTH);

		// test fibre is connected
		{
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(0, insz);
			ASSERT_EQUAL(1, outsz);
			//test width
			int sz = fibre.getConnections().getSize();
			// test cluster types
			Fibre::FibreType fibretype = fibre.getType();
			bool output_cluster_connected = Fibre::OutputCluster & fibre.isConnected(cluster1);

			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::PrimaryInputFibre, fibretype);
			ASSERT(output_cluster_connected);
		}

		//DEPRECATED
		/*
		 //test cluster is connected
		 {
		 int insz = cluster1->getConnector().getInputs().size();
		 int outsz = fibre.getConnector().getOutputs().size();
		 ASSERT_EQUAL(1, insz);
		 ASSERT_EQUAL(0, outsz);
		 }
		 */

	}
	// test primary out
	{
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		Fibre fibre(cluster1, Fibre::PrimaryOutputFibre, WIDTH);

		// test fibre is connected
		{
			//test width
			int sz = fibre.getConnections().getSize();
			// test cluster types
			Fibre::FibreType fibretype = fibre.getType();
			int clustertype1 = Fibre::InputCluster & fibre.isConnected(cluster1);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(0, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::PrimaryOutputFibre, fibretype);
			ASSERT(clustertype1);
		}

		//DEPRECATED
		/*
		 //test cluster is connected
		 {
		 int insz = cluster1->getConnector().getInputs().size();
		 int outsz = fibre.getConnector().getOutputs().size();
		 ASSERT_EQUAL(0, insz);
		 ASSERT_EQUAL(1, outsz);
		 }
		 */
	}
	// test intermediate
	{
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		Fibre fibre(cluster1, cluster2, WIDTH);

		// test fibre is connected
		{
			//test width
			int sz = fibre.getConnections().getSize();
			// test cluster types
			Fibre::FibreType fibretype = fibre.getType();
			int clustertype1 = Fibre::InputCluster & fibre.isConnected(cluster1);
			int clustertype2 = Fibre::OutputCluster & fibre.isConnected(cluster2);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(1, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::IntermediateFibre, fibretype);
			ASSERT(clustertype1);
			ASSERT( clustertype2);
		}

		//DEPRECATED
		/*
		 //test cluster is connected
		 {
		 int insz = cluster1->getConnector().getInputs().size();
		 int outsz = fibre.getConnector().getOutputs().size();
		 ASSERT_EQUAL(0, insz);
		 ASSERT_EQUAL(1, outsz);
		 }
		 */

		//DEPRECATED
		/*
		 //test cluster is connected
		 {
		 int insz = cluster2->getConnector().getInputs().size();
		 int outsz = fibre.getConnector().getOutputs().size();
		 ASSERT_EQUAL(1, insz);
		 ASSERT_EQUAL(0, outsz);
		 }
		 */

	}
	//test loop
	{
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		Fibre fibre(cluster1, cluster1, WIDTH);

		// test fibre is connected
		{
			//test width
			int sz = fibre.getConnections().getSize();
			// test cluster types
			Fibre::FibreType fibretype = fibre.getType();
			int clustertype1 = Fibre::LoopbackCluster & fibre.isConnected(cluster1);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(1, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::LoopbackFibre, fibretype);
			ASSERT(clustertype1);
		}

		//DEPRECATED
		/*
		 //test cluster is connected
		 {
		 int insz = cluster1->getConnector().getInputs().size();
		 int outsz = fibre.getConnector().getOutputs().size();
		 ASSERT_EQUAL(1, insz);
		 ASSERT_EQUAL(1, outsz);
		 }
		 */
	}
}

void FibreTest::testTrigger() {
	// trigger using a pattern
	{
		const int WIDTH = 10;
		const state::Pattern expected_pat("1100110011");

		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, cluster2, WIDTH));
		fibre->trigger(expected_pat);
		common::TimeKeeper::getTimeKeeper().update();
		cluster1->update();
		cluster2->update();
		fibre->update();
		 fibre->getMutableConnections().update();
		const boost::shared_ptr<state::Pattern> actual_pattern = fibre->getOutputNodesPattern();
		ASSERT_EQUAL(expected_pat, *actual_pattern);
	}

	// trigger a full blow out
	{
		const int WIDTH = 10;

		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, cluster2, WIDTH));
		fibre->trigger();
		const state::Pattern actual_pattern(fibre->getConnections().getActivityPattern()->toPlusBooleanString());
		const state::Pattern expected_pat("1111111111");
		ASSERT_EQUAL(expected_pat, actual_pattern);
	}

	// trigger using a line of impulses
	{
		ASSERTM("TODO", false);
	}
}

void FibreTest::testGetNodesPattern() {
	const int WIDTH = 3;
	boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
	boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
	boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, cluster2, WIDTH));

	ASSERT(fibre != 0);
	boost::shared_ptr<state::Pattern> empty_pattern(new state::Pattern("000"));
	boost::shared_ptr<state::Pattern> exp_pattern(new state::Pattern("101"));
	// test input nodes pattern setting
	{
		// set the  fibres input nodes to force fire a pattern
		fibre->forceFireInputNodes(*exp_pattern);
		//	std::cout<<"FibreTest::testGetNodesPattern: "<<*cluster1<<std::endl;
		// update to actual fire nodes
		common::TimeKeeper::getTimeKeeper().update();
		cluster1->update();
		//std::cout<<"FibreTest::testGetNodesPattern: "<<*cluster1<<std::endl;
		// get the actual fibre firing pattern
		boost::shared_ptr<state::Pattern> act_pattern = fibre->getInputNodesPattern();
		// should be the same as the forced node firing pattern
		std::cout << "FibreTest::testGetINodesPattern: " << "exp: " << *exp_pattern << "\tact: " << *act_pattern
				<< std::endl;
		ASSERT(*exp_pattern== *act_pattern);
		// update again
		common::TimeKeeper::getTimeKeeper().update();
		cluster1->update();
		// fibre firing pattern should be zeroed
		boost::shared_ptr<state::Pattern> end_pattern = fibre->getInputNodesPattern();
		std::cout << "FibreTest::testGetINodesPattern: " << "end: " << *end_pattern << std::endl;
		ASSERT(*empty_pattern == *end_pattern);
	}

	// test input nodes pattern setting
	{
		// set the  fibres input nodes to force fire a pattern
		fibre->forceFireOutputNodes(*exp_pattern);
		//std::cout<<"FibreTest::testGetNodesPattern: "<<*cluster2<<std::endl;
		// update to actual fire nodes
		common::TimeKeeper::getTimeKeeper().update();
		cluster1->update();
		//std::cout<<"FibreTest::testGetNodesPattern: "<<*cluster2<<std::endl;
		// get the actual fibre firing pattern
		boost::shared_ptr<state::Pattern> act_pattern = fibre->getOutputNodesPattern();
		// should be the same as the forced node firing pattern
		std::cout << "FibreTest::testGetINodesPattern: " << "exp: " << *exp_pattern << "\tact: " << *act_pattern
				<< std::endl;
		ASSERT(*exp_pattern== *act_pattern);
		// update again
		common::TimeKeeper::getTimeKeeper().update();
		cluster1->update();
		// fibre firing pattern should be zeroed
		boost::shared_ptr<state::Pattern> end_pattern = fibre->getInputNodesPattern();
		std::cout << "FibreTest::testGetINodesPattern: " << "end: " << *end_pattern << std::endl;
		ASSERT(*empty_pattern == *end_pattern);
	}
}

void FibreTest::testCountConnections() {
	const int WIDTH = 3;
	// simple intermediate
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> > clusters;
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		clusters[cluster1->getUUID()] = cluster1;
		clusters[cluster2->getUUID()] = cluster2;

		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, cluster2, WIDTH));
		std::pair<int, int> concount = fibre->countConnections(clusters);

		ASSERT_EQUAL(1, concount.first);
		ASSERT_EQUAL(1, concount.second);
	}

	// input
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> > clusters;
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		clusters[cluster1->getUUID()] = cluster1;
		clusters[cluster2->getUUID()] = cluster2;

		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, Fibre::PrimaryInputFibre, WIDTH));
		std::pair<int, int> concount = fibre->countConnections(clusters);

		ASSERT_EQUAL(0, concount.first);
		ASSERT_EQUAL(1, concount.second);
	}

	// output
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> > clusters;
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		clusters[cluster1->getUUID()] = cluster1;
		clusters[cluster2->getUUID()] = cluster2;

		boost::shared_ptr<Fibre> fibre(new Fibre(cluster2, Fibre::PrimaryOutputFibre, WIDTH));
		std::pair<int, int> concount = fibre->countConnections(clusters);

		ASSERT_EQUAL(1, concount.first);
		ASSERT_EQUAL(0, concount.second);
	}

}

void FibreTest::testIsConnected() {
	const int WIDTH=10;
	// as input
	{
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, Fibre::PrimaryInputFibre, WIDTH));
		int type_int = fibre->isConnected(cluster1);
		bool type = type_int & Fibre::ClusterConnectionType::OutputCluster;
		std::cout<<"FibreTest::testIsConnected: "<<"type_int: "<<type_int<<" type: "<<type<<std::endl;
		ASSERT(type);
	}
	// as output
	{
		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Fibre> fibre(new Fibre(cluster1, Fibre::PrimaryOutputFibre, WIDTH));
		bool type = fibre->isConnected(cluster1) & Fibre::ClusterConnectionType::InputCluster;
		ASSERT(type);
	}
}
}//NAMESPACE

}//NAMESPACE
