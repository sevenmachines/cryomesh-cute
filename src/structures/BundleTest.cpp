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
	s.push_back(CUTE( BundleTest::testAutoConnection));
	s.push_back(CUTE( BundleTest::testPatternChannelsSetup));
	s.push_back(CUTE( BundleTest::testGetDisconnectedChannels));
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

	ASSERT_EQUAL(3, bun.getRealInputChannelsMap().getSize());
	ASSERT_EQUAL(3, bun.getRealOutputChannelsMap().getSize());

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

	ASSERT_EQUAL(3, bun.getRealInputChannelsMap().getSize());
	ASSERT_EQUAL(3, bun.getRealOutputChannelsMap().getSize());

	// inrefs channels are 1, 2 3
	boost::uuids::uuid inchan1;
	boost::uuids::uuid inchan2;
	boost::uuids::uuid inchan3;
	// get the 3 input channel ids
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_input_channels_map =
				bun.getRealInputChannelsMap().begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator
				it_input_channels_map_end = bun.getRealInputChannelsMap().end();
		ASSERT_EQUAL(3, bun.getRealInputChannelsMap().getSize());
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
	ASSERT_EQUAL(3, bun.getRealFibrePatternChannelMap().size());
	ASSERT_EQUAL(3, bun.getActualFibrePatternChannelMap().size());

	//our ref channels are 1001, 1002, 1003
	// get the 3 input channel ids
	boost::uuids::uuid outchan1;
	boost::uuids::uuid outchan2;
	boost::uuids::uuid outchan3;
	// get the 3 output channel ids
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_output_channels_map =
				bun.getRealOutputChannelsMap().begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator
				it_output_channels_map_end = bun.getRealOutputChannelsMap().end();
		ASSERT_EQUAL(3, bun.getRealOutputChannelsMap().getSize());
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
	ASSERT_EQUAL(6, bun.getRealFibrePatternChannelMap().size());
	ASSERT_EQUAL(6, bun.getActualFibrePatternChannelMap().size());
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

void BundleTest::testPatternChannelsSetup() {
	// data file is 3 in/outs of 3x2
	const std::string DATAFILE = "TestData/sequences_3x2x3.xml";
	const int PATTERN_CHANNEL_WIDTH = 3;
	const int PATTERN_CHANNEL_DEPTH = 2;
	const int PATTERN_CHANNEL_COUNT = 3;

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

	const state::PatternChannelMap & real_input_channels = bun.getRealInputChannelsMap();
	const state::PatternChannelMap & real_output_channels = bun.getRealOutputChannelsMap();
	const state::PatternChannelMap & actual_input_channels = bun.getActualInputChannelsMap();
	const state::PatternChannelMap & actual_output_channels = bun.getActualOutputChannelsMap();
	const std::map<boost::uuids::uuid, boost::uuids::uuid> real_fibre_map = bun.getRealFibrePatternChannelMap();
	const std::map<boost::uuids::uuid, boost::uuids::uuid> actual_fibre_map = bun.getActualFibrePatternChannelMap();
	//Check channels and maps
	{
		ASSERT_EQUAL(3, real_input_channels.getSize());
		ASSERT_EQUAL(3, real_output_channels.getSize());
		ASSERT_EQUAL(0, actual_input_channels.getSize());
		ASSERT_EQUAL(0, actual_output_channels.getSize());
		ASSERT_EQUAL(0, real_fibre_map.size());
		ASSERT_EQUAL(0, actual_fibre_map.size());
	}

	// connect up the fibres and pattern channels
	bun.autoConnectPrimaryInputClusters(std::vector<boost::shared_ptr<Cluster> >( { cluster1 }));
	bun.autoConnectPrimaryOutputClusters(std::vector<boost::shared_ptr<Cluster> >( { cluster2 }));

	// check initial channels size
	{
		ASSERT_EQUAL(2*PATTERN_CHANNEL_COUNT, bun.getRealFibrePatternChannelMap().size());
		ASSERT_EQUAL(2*PATTERN_CHANNEL_COUNT, bun.getActualFibrePatternChannelMap().size());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getRealInputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getRealOutputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getActualInputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getActualOutputChannelsMap().getSize());
		std::cout << "BundleTest::testPatternChannelsSetup: " << "INITIAL MAPPINGS" << std::endl;
		std::cout << bun.printChannels(std::cout);
		std::cout << std::endl;
		BundleTest::printFibreMaps(bun);
		std::cout << std::endl;
	}

	// count channels at start
	{
		std::cout << "BundleTest::testPatternChannelsSetup: " << "Real: BEGIN" << std::endl;
		ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealInputChannelsMap(), PATTERN_CHANNEL_DEPTH));
		ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealOutputChannelsMap(), PATTERN_CHANNEL_DEPTH));
		std::cout << "BundleTest::testPatternChannelsSetup: " << "Actual: BEGIN" << std::endl;
		ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualInputChannelsMap(), 0));
		ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualOutputChannelsMap(), 0));
	}

	// count channels as we update
	{
		for (int i = 0; i < PATTERN_CHANNEL_DEPTH; i++) {
			bun.update();
			std::cout << "BundleTest::testPatternChannelsSetup: " << "Real:" << i << std::endl;
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealInputChannelsMap(), PATTERN_CHANNEL_DEPTH));
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealOutputChannelsMap(), PATTERN_CHANNEL_DEPTH));
			std::cout << "BundleTest::testPatternChannelsSetup: " << "Actual:" << i << std::endl;
			//ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualInputChannelsMap(), i));
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualOutputChannelsMap(), i));
		}
	}

	// count channels reach max size
	{
		for (int i = 0; i < 2; i++) {
			bun.update();
			std::cout << "BundleTest::testPatternChannelsSetup: " << "Real:" << i << std::endl;
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealInputChannelsMap(), PATTERN_CHANNEL_DEPTH));
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getRealOutputChannelsMap(), PATTERN_CHANNEL_DEPTH));
			std::cout << "BundleTest::testPatternChannelsSetup: " << "Actual:" << i << std::endl;
			//ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualInputChannelsMap(), PATTERN_CHANNEL_DEPTH));
			ASSERT(BundleTest::checkChannelsMapDepth(bun.getActualOutputChannelsMap(), PATTERN_CHANNEL_DEPTH));
		}
	}
}

void BundleTest::testGetDisconnectedChannels(){
	ASSERTM("TODO", false);
}
void BundleTest::testAutoConnection() {
	// data file is 3 in/outs of 3x2
	const std::string DATAFILE = "TestData/sequences_3x2x3.xml";
	const int PATTERN_CHANNEL_WIDTH = 3;
	const int PATTERN_CHANNEL_DEPTH = 2;
	const int PATTERN_CHANNEL_COUNT = 3;

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

	const state::PatternChannelMap & real_input_channels = bun.getRealInputChannelsMap();
	const state::PatternChannelMap & real_output_channels = bun.getRealOutputChannelsMap();
	const state::PatternChannelMap & actual_input_channels = bun.getActualInputChannelsMap();
	const state::PatternChannelMap & actual_output_channels = bun.getActualOutputChannelsMap();
	const std::map<boost::uuids::uuid, boost::uuids::uuid> real_fibre_map = bun.getRealFibrePatternChannelMap();
	const std::map<boost::uuids::uuid, boost::uuids::uuid> actual_fibre_map = bun.getActualFibrePatternChannelMap();

	// 3 Real input channels created from file
	// 3 Real output channels created from file
	// No actual input or output channels created yet
	// No real fibre map, no fibres have been linked yet
	// No actual fibre map, no linked fibres yet

	//Check channels and maps
	{
		ASSERT_EQUAL(3, real_input_channels.getSize());
		ASSERT_EQUAL(3, real_output_channels.getSize());
		ASSERT_EQUAL(0, actual_input_channels.getSize());
		ASSERT_EQUAL(0, actual_output_channels.getSize());
		ASSERT_EQUAL(0, real_fibre_map.size());
		ASSERT_EQUAL(0, actual_fibre_map.size());
	}

	// connect up the fibres and pattern channels
	std::vector<boost::shared_ptr< Fibre > > new_ins = bun.autoConnectPrimaryInputClusters(std::vector<boost::shared_ptr<Cluster> >( { cluster1 }));
	std::vector<boost::shared_ptr<Fibre> > new_outs = bun.autoConnectPrimaryOutputClusters(std::vector<boost::shared_ptr<Cluster> >( { cluster2 }));

	ASSERT_EQUAL(3, new_ins.size());
	ASSERT_EQUAL(3, new_outs.size());

	// 3 Real input channels created from file
	// 3 Real output channels created from file
	// 3 actual input channels created automatically
	// 3 actual output channels created automatically
	// 6 Real fibre map has all 3 real inputs matched to 3  fibres and 3 real outputs muctched to 3 fibres
	// 6 Real fibre map has all 3 actual inputs matched to 3  fibres and 3 actual outputs muctched to 3 fibres

	// check initial channels size
	{
		ASSERT_EQUAL(2*PATTERN_CHANNEL_COUNT, bun.getRealFibrePatternChannelMap().size());
		ASSERT_EQUAL(2*PATTERN_CHANNEL_COUNT, bun.getActualFibrePatternChannelMap().size());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getRealInputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getRealOutputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getActualInputChannelsMap().getSize());
		ASSERT_EQUAL(PATTERN_CHANNEL_COUNT, bun.getActualOutputChannelsMap().getSize());
	}
}

bool BundleTest::checkChannelsMapDepth(const state::PatternChannelMap & map, const int depth) {
	const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> > & all_objs = map.getCollection();
	bool success = true;
	// forall in all_objs
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_all_objs =
				all_objs.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<state::PatternChannel> >::const_iterator it_all_objs_end =
				all_objs.end();
		while (it_all_objs != it_all_objs_end) {
			int pc_depth = it_all_objs->second->getPatternMap().size();
			if (pc_depth != depth) {
				success = false;
				std::cout << "BundleTest::checkChannelsMapDepth: " << pc_depth << "!=" << depth << std::endl;
			} else {
			//	std::cout << "BundleTest::checkChannelsMapDepth: " << pc_depth << "==" << depth << std::endl;

			}
			++it_all_objs;
		}
	}
	return success;
}

void BundleTest::printFibreMaps(const Bundle & bun) {

	std::cout << "BundleTest::printFibreMaps: " << "RealFibrePatternChannelMap" << std::endl;
	printFibreMap(bun.getRealFibrePatternChannelMap());
	std::cout << "BundleTest::printFibreMaps: " << "ActualFibrePatternChannelMap" << std::endl;
	printFibreMap(bun.getActualFibrePatternChannelMap());
}

void BundleTest::printFibreMap(const std::map<boost::uuids::uuid, boost::uuids::uuid> & fibre_map) {
	// forall in fibre_map
	{
		std::map<boost::uuids::uuid, boost::uuids::uuid>::const_iterator it_fibre_map = fibre_map.begin();
		const std::map<boost::uuids::uuid, boost::uuids::uuid>::const_iterator it_fibre_map_end = fibre_map.end();
		while (it_fibre_map != it_fibre_map_end) {
			std::cout << it_fibre_map->first << " : " << it_fibre_map->second << std::endl;
			++it_fibre_map;
		}
	}
}
}//NAMESPACE

}//NAMESPACE
