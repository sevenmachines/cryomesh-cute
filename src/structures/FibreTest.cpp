/*
 * FibreTest.cpp
 *
 *  Created on: 16 Feb 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "FibreTest.h"
#include "structures/Fibre.h"
#include "structures/Cluster.h"

namespace cryomesh {

namespace structures {

void FibreTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE( FibreTest::testCreation));
	s.push_back(CUTE( FibreTest::testTrigger));
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
		boost::shared_ptr< Fibre > fibre( new Fibre(cluster1, cluster2, WIDTH));
		fibre->trigger(expected_pat);
		const state::Pattern actual_pattern(fibre->getConnections().getActivityPattern()->toPlusBooleanString());
		ASSERT_EQUAL(expected_pat, actual_pattern);
	}

	// trigger a full blow out
	{
		const int WIDTH = 10;

		boost::shared_ptr<Cluster> cluster1(new Cluster(10, 1));
		boost::shared_ptr<Cluster> cluster2(new Cluster(20, 2));
		boost::shared_ptr< Fibre > fibre( new Fibre(cluster1, cluster2, WIDTH));
		fibre->trigger();
		const state::Pattern actual_pattern (fibre->getConnections().getActivityPattern()->toPlusBooleanString());
		const state::Pattern expected_pat("1111111111");
		ASSERT_EQUAL(expected_pat, actual_pattern);
	}

	// trigger using a line of impulses
	{
		ASSERTM("TODO", false);
	}
}

}//NAMESPACE

}//NAMESPACE
