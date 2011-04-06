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
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "FibreTest");
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
			Fibre::ClusterConnectionType clustertype1 = fibre.isConnected(cluster1);

			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::PrimaryInputFibre, fibretype);
			ASSERT_EQUAL(Fibre::OutputCluster, clustertype1);
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
			Fibre::ClusterConnectionType clustertype1 = fibre.isConnected(cluster1);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(0, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::PrimaryOutputFibre, fibretype);
			ASSERT_EQUAL(Fibre::InputCluster, clustertype1);
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
			Fibre::ClusterConnectionType clustertype1 = fibre.isConnected(cluster1);
			Fibre::ClusterConnectionType clustertype2 = fibre.isConnected(cluster2);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(1, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::IntermediateFibre, fibretype);
			ASSERT_EQUAL(Fibre::InputCluster, clustertype1);
			ASSERT_EQUAL(Fibre::OutputCluster, clustertype2);
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
			Fibre::ClusterConnectionType clustertype1 = fibre.isConnected(cluster1);
			int insz = fibre.getConnector().getInputs().size();
			int outsz = fibre.getConnector().getOutputs().size();
			ASSERT_EQUAL(1, insz);
			ASSERT_EQUAL(1, outsz);
			ASSERT_EQUAL(WIDTH, sz);
			ASSERT_EQUAL(Fibre::LoopbackFibre, fibretype);
			ASSERT_EQUAL(Fibre::LoopbackCluster, clustertype1);
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
		//cluster1->update();
		//cluster2->update();
		//fibre->update();
		// fibre->getMutableConnections().update();
		const state::Pattern actual_pattern(fibre->getConnections().getActivityPattern()->toPlusBooleanString());
		ASSERT_EQUAL(expected_pat, actual_pattern);
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

}//NAMESPACE

}//NAMESPACE
