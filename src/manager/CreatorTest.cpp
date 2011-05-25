/*
 * CreatorTest.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "CreatorTest.h"
#include "manager/Creator.h"
#include "config/ConfigEntry.h"
#include "config/ConfigTranslator.h"
#include <boost/shared_ptr.hpp>
#include "structures/Cluster.h"

using namespace cryomesh::structures;

namespace cryomesh {

namespace manager {

const std::string CreatorTest::CONFIG_FILE = "Data/test1.config";
const std::string CreatorTest::AUTOCONNECT_CONFIG_FILE = "Data/autoconnect_test1.config";
const std::string CreatorTest::NULL_CONFIG_FILE = "Data/null.config";
const std::string CreatorTest::DATAFILE = "TestData/sequences_3x2x3.xml";

void CreatorTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(CreatorTest::testCheckConfigEntry));
	s.push_back(CUTE(CreatorTest::testCheckConfigStructure));
	s.push_back(CUTE(CreatorTest::testCreation));
	s.push_back(CUTE(CreatorTest::testAutoConnect));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "CreatorTest");
}

void CreatorTest::testCreation() {
	Creator creator(CreatorTest::CONFIG_FILE);
	boost::shared_ptr<structures::Bundle> bundle = creator.getBundle();
	ASSERT(bundle !=0);
	//3 clusters
	int cluster_count = bundle->getClusters().getSize();
	ASSERT_EQUAL(3, cluster_count);

	// 3 interconnecting fibres
	int fibre_count = bundle->getFibres().getSize();
	ASSERT_EQUAL(3, fibre_count);

	// test connecting to loaded pattern channels, 3 in, 3 out
	int patternmap_count = creator.getPatternChannelIDMap().size();
	ASSERT_EQUAL(6, patternmap_count);

	int infib_sz = creator.getBundle()->getInputFibres().getSize();
	int infibmap_sz = creator.getBundle()->getRealInputChannelsMap().getSize();
	ASSERT_EQUAL(3, infib_sz);
	ASSERT_EQUAL(3, infibmap_sz);

	int outfib_sz = creator.getBundle()->getOutputFibres().getSize();
	int outfibmap_sz = creator.getBundle()->getRealOutputChannelsMap().getSize();
	ASSERT_EQUAL(3, outfib_sz);
	ASSERT_EQUAL(3, outfibmap_sz);

}
void CreatorTest::testCheckConfigEntry() {
	Creator creator(CreatorTest::NULL_CONFIG_FILE);

	// good entry, 2 option entry
	{
		std::string entry_str = "connect-primary-input id=1 outputid=1";
		config::ConfigEntry entry(entry_str);
		ASSERT(creator.checkConfigEntry(entry) == true);
	}

	// bad entry, unknown command
	{
		std::string entry_str = "not-a-command id=4";
		config::ConfigEntry entry(entry_str);
		ASSERT(creator.checkConfigEntry(entry) == false);
	}
	// bad entry, no options
	{
		std::string entry_str = "connect-primary-input";
		config::ConfigEntry entry(entry_str);
		ASSERT(creator.checkConfigEntry(entry) == false);
	}

	// bad entry, missing option
	{
		std::string entry_str = "create-cluster id=1 connectivity=10";
		config::ConfigEntry entry(entry_str);
		ASSERT(creator.checkConfigEntry(entry) == false);
	}
}

void CreatorTest::testCheckConfigStructure() {
	Creator creator(CreatorTest::CONFIG_FILE);
	// good
	{
		config::ConfigTranslator translator(CreatorTest::CONFIG_FILE);
		bool success = creator.checkConfigStructure(translator.getEntries());
		ASSERT(success == true);
	}

	// bad, no cluster create
	{
		config::ConfigTranslator translator("Data/test-noclustercreate");
		bool success = creator.checkConfigStructure(translator.getEntries());
		ASSERT(success == false);
	}

	// bad, no loaddata
	{
		config::ConfigTranslator translator("Data/test-noloaddata");
		bool success = creator.checkConfigStructure(translator.getEntries());
		ASSERT(success == false);
	}
}

void CreatorTest::testAutoConnect() {
	Creator creator(CreatorTest::AUTOCONNECT_CONFIG_FILE);
	std::cout << "CreatorTest::testAutoConnect: " << "" << std::endl;
	boost::shared_ptr<structures::Bundle> bundle = creator.getBundle();
	ASSERT(bundle !=0);
	//3 clusters
	int cluster_count = bundle->getClusters().getSize();
	ASSERT_EQUAL(2, cluster_count);

	// 1 interconnecting fibres
	int fibre_count = bundle->getFibres().getSize();
	ASSERT_EQUAL(1, fibre_count);

	// test connecting to loaded pattern channels, 3 in, 3 out
	int patternmap_count = creator.getPatternChannelIDMap().size();
	ASSERT_EQUAL(6, patternmap_count);

	int infib_sz = creator.getBundle()->getInputFibres().getSize();
	int infibmap_sz = creator.getBundle()->getRealInputChannelsMap().getSize();
	ASSERT_EQUAL(3, infib_sz);
	ASSERT_EQUAL(3, infibmap_sz);

	int outfib_sz = creator.getBundle()->getOutputFibres().getSize();
	int outfibmap_sz = creator.getBundle()->getRealOutputChannelsMap().getSize();
	ASSERT_EQUAL(3, outfib_sz);
	ASSERT_EQUAL(3, outfibmap_sz);

	std::cout << "CreatorTest::testAutoConnect: " << "**********" << std::endl << *bundle << std::endl;
	// check 3 into 2 inputs
	{
		boost::shared_ptr<Cluster> first_cluster = bundle->getClusters().getCollection().begin()->second;
		boost::shared_ptr<Cluster> second_cluster = bundle->getClusters().getCollection().rbegin()->second;

		// check pattern channel creation
		{
			int act_fib_map_sz = bundle->getActualFibrePatternChannelMap().size();
			int real_fib_map_sz = bundle->getRealFibrePatternChannelMap().size();
			int real_disconnected_input_channels = bundle->getDisconnectedRealInputPatternChannels().size();
			int real_disconnected_output_channels = bundle->getDisconnectedRealInputPatternChannels().size();
			ASSERT_EQUAL(6, act_fib_map_sz);
			ASSERT_EQUAL(6, real_fib_map_sz);
			ASSERT_EQUAL(0, real_disconnected_input_channels);
			ASSERT_EQUAL(0, real_disconnected_output_channels);

		}
		ASSERT_EQUAL(2,bundle->getClusters().getSize());
		const std::map<boost::uuids::uuid, boost::shared_ptr<Fibre> > & all_fibs =
				bundle->getInputFibres().getCollection();
		// forall in all_fibs
		{
			int first_cluster_input_count = 0;
			int second_cluster_input_count = 0;
			int first_cluster_output_count = 0;
			int second_cluster_output_count = 0;
			std::map<boost::uuids::uuid, boost::shared_ptr<Fibre> >::const_iterator it_all_fibs = all_fibs.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Fibre> >::const_iterator it_all_fibs_end =
					all_fibs.end();
			while (it_all_fibs != it_all_fibs_end) {
				if (it_all_fibs->second->isConnected(first_cluster) & Fibre::ClusterConnectionType::InputCluster) {
					++first_cluster_input_count;
				}
				if (it_all_fibs->second->isConnected(second_cluster) & Fibre::ClusterConnectionType::InputCluster) {
					++second_cluster_input_count;
				}
				if (it_all_fibs->second->isConnected(first_cluster) & Fibre::ClusterConnectionType::OutputCluster) {
					++first_cluster_output_count;
				}
				if (it_all_fibs->second->isConnected(second_cluster) & Fibre::ClusterConnectionType::OutputCluster) {
					++second_cluster_output_count;
				}
				++it_all_fibs;
			}
			std::cout << "CreatorTest::testAutoConnect: " << first_cluster_input_count << " "
					<< second_cluster_input_count << " " << first_cluster_output_count << " "
					<< second_cluster_output_count << " " << std::endl;

			int actual_first_cluster_input_count = 0;
			int actual_second_cluster_input_count = 0;
			int actual_first_cluster_output_count = 0;
			int actual_second_cluster_output_count = 0;
			if (first_cluster_input_count > 0) {
				actual_first_cluster_input_count = first_cluster_input_count;
				actual_first_cluster_output_count = first_cluster_output_count;
				actual_second_cluster_input_count = second_cluster_input_count;
				actual_second_cluster_output_count = second_cluster_output_count;
			} else {
				actual_first_cluster_input_count = second_cluster_input_count;
				actual_first_cluster_output_count = second_cluster_output_count;
				actual_second_cluster_input_count = first_cluster_input_count;
				actual_second_cluster_output_count = first_cluster_output_count;
			}
			ASSERT_EQUAL(2, actual_first_cluster_input_count);

			ASSERT_EQUAL(1, actual_second_cluster_input_count);
			ASSERT_EQUAL(0, actual_first_cluster_output_count);
			ASSERT_EQUAL(3, actual_second_cluster_output_count);
		}
	}

}

}//NAMESPACE

}//NAMESPACE
