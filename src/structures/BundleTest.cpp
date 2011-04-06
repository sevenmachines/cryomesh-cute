/*
 * BundleTest.cpp
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#include "structures/Bundle.h"
#include "BundleTest.h"

namespace cryomesh {

namespace structures {

void BundleTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE( BundleTest::testCreateConnect));
	s.push_back(CUTE( BundleTest::testPropagation));
	s.push_back(CUTE( BundleTest::testLoadData));
	s.push_back(CUTE( BundleTest::testConnectPatternChannels));
	s.push_back(CUTE( BundleTest::testCheckStructure));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "BundleTest");
}

void BundleTest::testCreateConnect() {
	Bundle bun;
	boost::shared_ptr<Cluster> cluster1 = bun.createCluster(10, 2);
	boost::shared_ptr<Cluster> cluster2 = bun.createCluster(20, 4);
	ASSERT_EQUAL(2, bun.getClusters().getSize());

	boost::shared_ptr<Fibre> fibre1 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), 10);
	boost::shared_ptr<Fibre> fibre2 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), 20);

	ASSERT(fibre1 !=0);
	ASSERT_EQUAL(10, fibre1->getConnections().getSize());
	ASSERT(fibre2 !=0);
	ASSERT_EQUAL(20, fibre2->getConnections().getSize());

	ASSERT_EQUAL(2, bun.getFibres().getSize());
}

void BundleTest::testPropagation() {

	const int FIBRE_SZ = 10;
	const int CLUSTER1_SZ = 10;
	const int CLUSTER2_SZ = 20;
	const int CLUSTER1_CONNECTIVITY = 20;
	const int CLUSTER2_CONNECTIVITY = 40;

	// fibrein->cluster1->fibre1->cluster2->fibreout
	Bundle bun;
	boost::shared_ptr<Cluster> cluster1 = bun.createCluster(CLUSTER1_SZ, CLUSTER1_CONNECTIVITY);
	boost::shared_ptr<Cluster> cluster2 = bun.createCluster(CLUSTER2_SZ, CLUSTER2_CONNECTIVITY);
	ASSERT( cluster1!=0);
	ASSERT( cluster2!=0);

	boost::shared_ptr<Fibre> fibre1 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), FIBRE_SZ);
	boost::shared_ptr<Fibre> fibrein = bun.connectCluster(cluster1->getUUID(), Fibre::PrimaryInputFibre, FIBRE_SZ);
	boost::shared_ptr<Fibre> fibreout = bun.connectCluster(cluster2->getUUID(), Fibre::PrimaryOutputFibre, FIBRE_SZ);

	ASSERT( fibre1!=0);
	ASSERT( fibrein!=0);
	ASSERT( fibreout!=0);
	// check structure
	{
		// general sizes
		{
			int bun_cluster_sz = bun.getClusters().getSize();
			int bun_fibres_sz = bun.getFibres().getSize();

			//cluster1
			{
				int con_sz = cluster1->getConnections().size();
				int in_sz = cluster1->getConnector().getInputs().size();
				int out_sz = cluster1->getConnector().getOutputs().size();
				int node_sz = cluster1->getNodeMap().getSize();
				ASSERT_EQUAL(1, in_sz);
				ASSERT_EQUAL(1, out_sz);
				ASSERT_EQUAL(CLUSTER1_SZ, node_sz);
			}

			//cluster2
			{
				int con_sz = cluster2->getConnections().size();
				int in_sz = cluster2->getConnector().getInputs().size();
				int out_sz = cluster2->getConnector().getOutputs().size();
				int node_sz = cluster2->getNodeMap().getSize();
				ASSERT_EQUAL(1, in_sz);
				ASSERT_EQUAL(1, out_sz);
				ASSERT_EQUAL(CLUSTER2_SZ, node_sz);
			}

		}

		// connectivity 	(fibrein->cluster1->fibre1->cluster2->fibreout)

		{
			bool fin_to_c1 = fibrein->isConnected(cluster1);
			bool f1_to_c1 = fibre1->isConnected(cluster1);
			bool f1_to_c2 = fibre1->isConnected(cluster2);
			bool fout_to_c2 = fibreout->isConnected(cluster2);
			ASSERT_EQUAL(true, fin_to_c1);
			ASSERT_EQUAL(true, f1_to_c1);
			ASSERT_EQUAL(true, f1_to_c2);
			ASSERT_EQUAL(true, fout_to_c2);
		}

	}
	//	std::cout << "BundleTest::testPropagation: " << "" << std::endl;
	//std::cout << bun << std::endl;
	// propogate
	{
		const int MAX_PROPS = 10;
		for (int i = 0; i < MAX_PROPS; i++) {
			//	std::cout<<"BundleTest::testPropagation: "<<i<<std::endl;
			fibrein->trigger(0.5);
			bun.update();
			//	std::cout << "BundleTest::testPropagation: " << "" << std::endl;
			//	std::cout << bun << std::endl;

		}
	}
	// check output fibre state
	{

	}
	ASSERTM("TODO", false);
}

void BundleTest::testLoadData() {
	// data file is 3 in/outs of 3x2
	const std::string DATAFILE = "TestData/sequences_3x2x3.xml";

	const int FIBRE_SZ = 10;
	const int CLUSTER1_SZ = 10;
	const int CLUSTER2_SZ = 20;
	const int CLUSTER1_CONNECTIVITY = 20;
	const int CLUSTER2_CONNECTIVITY = 40;

	// cluster1->fibre1->cluster2
	Bundle bun;
	boost::shared_ptr<Cluster> cluster1 = bun.createCluster(CLUSTER1_SZ, CLUSTER1_CONNECTIVITY);
	boost::shared_ptr<Cluster> cluster2 = bun.createCluster(CLUSTER2_SZ, CLUSTER2_CONNECTIVITY);
	boost::shared_ptr<Fibre> fibre1 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), FIBRE_SZ);

	bun.loadChannels(DATAFILE);

	ASSERT_EQUAL(3, bun.getInputChannelsMap().getSize());
	ASSERT_EQUAL(3, bun.getOutputChannelsMap().getSize());

}

void BundleTest::testConnectPatternChannels() {
	// data file is 3 in/outs of 3x2
	const std::string DATAFILE = "TestData/sequences_3x2x3.xml";

	const int FIBRE_SZ = 10;
	const int CLUSTER1_SZ = 10;
	const int CLUSTER2_SZ = 20;
	const int CLUSTER1_CONNECTIVITY = 20;
	const int CLUSTER2_CONNECTIVITY = 40;

	// cluster1->fibre1->cluster2
	Bundle bun;
	boost::shared_ptr<Cluster> cluster1 = bun.createCluster(CLUSTER1_SZ, CLUSTER1_CONNECTIVITY);
	boost::shared_ptr<Cluster> cluster2 = bun.createCluster(CLUSTER2_SZ, CLUSTER2_CONNECTIVITY);
	boost::shared_ptr<Fibre> fibre1 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), FIBRE_SZ);

	bun.loadChannels(DATAFILE);

	ASSERT_EQUAL(3, bun.getInputChannelsMap().getSize());
	ASSERT_EQUAL(3, bun.getOutputChannelsMap().getSize());

	// inrefs channels are 1, 2 3
	boost::uuids::uuid inchan1;
	boost::uuids::uuid inchan2;
	boost::uuids::uuid inchan3;
	// get the 3 input channel ids
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_input_channels_map =
				bun.getInputChannelsMap().begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator
				it_input_channels_map_end = bun.getInputChannelsMap().end();
		ASSERT_EQUAL(3, bun.getInputChannelsMap().getSize());
		inchan1 = it_input_channels_map->first;
		++it_input_channels_map;
		inchan2 = it_input_channels_map->first;
		++it_input_channels_map;
		inchan3 = it_input_channels_map->first;
	}

	// connect to the 2 clusters
	boost::shared_ptr<Fibre> fibrein1 = bun.connectPrimaryInputCluster(inchan1, cluster1->getUUID());
	boost::shared_ptr<Fibre> fibrein2 = bun.connectPrimaryInputCluster(inchan2, cluster1->getUUID());
	boost::shared_ptr<Fibre> fibrein3 = bun.connectPrimaryInputCluster(inchan3, cluster2->getUUID());

	// check total
	ASSERT_EQUAL(3, bun.getFibrePatternChannelMap().size());

	//our ref channels are 1001, 1002, 1003
	// get the 3 input channel ids
	boost::uuids::uuid outchan1;
	boost::uuids::uuid outchan2;
	boost::uuids::uuid outchan3;
	// get the 3 output channel ids
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_output_channels_map =
				bun.getOutputChannelsMap().begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator
				it_output_channels_map_end = bun.getOutputChannelsMap().end();
		ASSERT_EQUAL(3, bun.getOutputChannelsMap().getSize());
		outchan1 = it_output_channels_map->first;
		++it_output_channels_map;
		outchan2 = it_output_channels_map->first;
		++it_output_channels_map;
		outchan3 = it_output_channels_map->first;
	}

	// connect to the 2 clusters
	boost::shared_ptr<Fibre> fibreout1 = bun.connectPrimaryOutputCluster(outchan1, cluster1->getUUID());
	boost::shared_ptr<Fibre> fibreout2 = bun.connectPrimaryOutputCluster(outchan2, cluster2->getUUID());
	boost::shared_ptr<Fibre> fibreout3 = bun.connectPrimaryOutputCluster(outchan2, cluster2->getUUID());

	// check total
	ASSERT_EQUAL(6, bun.getFibrePatternChannelMap().size());

	// check fibres are all connected
	ASSERT(fibrein1->isConnected(cluster1) == Fibre::OutputCluster );
	ASSERT(fibrein2->isConnected(cluster1)== Fibre::OutputCluster );
	ASSERT(fibrein3->isConnected(cluster2)== Fibre::OutputCluster );

	ASSERT(fibreout1->isConnected(cluster1)== Fibre::InputCluster );
	ASSERT(fibreout2->isConnected(cluster2)== Fibre::InputCluster );
	ASSERT(fibreout3->isConnected(cluster2)== Fibre::InputCluster );

}

void BundleTest::testCheckStructure() {
	// cluster1->fibre1->cluster2
	Bundle bun;
	boost::shared_ptr<Cluster> cluster1 = bun.createCluster(1, 1);
	boost::shared_ptr<Cluster> cluster2 = bun.createCluster(2, 1);
	boost::shared_ptr<Cluster> cluster3 = bun.createCluster(3, 1);
	boost::shared_ptr<Fibre> fibre1 = bun.connectCluster(cluster1->getUUID(), cluster2->getUUID(), 2);
	boost::shared_ptr<Fibre> fibre2 = bun.connectCluster(cluster2->getUUID(), cluster3->getUUID(), 2);

	// check good structure
	{
		bool good = bun.checkFibreStructure();
		ASSERT(good);
	}

	// disconnect 1 to break this
	{
		fibre1->getMutableConnector().disconnectAllInputs();
		bool good = bun.checkFibreStructure();
		ASSERT(!good);
	}
	ASSERTM("TODO: Test channel structure", false);
}

}//NAMESPACE

}//NAMESPACE
