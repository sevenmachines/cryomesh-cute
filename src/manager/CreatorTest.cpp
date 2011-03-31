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

namespace cryomesh {

namespace manager {

const std::string CreatorTest::CONFIG_FILE = "Data/test1.config";
const std::string CreatorTest::NULL_CONFIG_FILE = "Data/null.config";

void CreatorTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(CreatorTest::testCheckConfigEntry));
	s.push_back(CUTE(CreatorTest::testCheckConfigStructure));
	s.push_back(CUTE(CreatorTest::testCreation));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "CreatorTest");
}

void CreatorTest::testCreation() {
	Creator creator(CreatorTest::CONFIG_FILE);
	boost::shared_ptr< structures::Bundle > bundle = creator.getBundle();
	ASSERT(bundle !=0);
	//3 clusters
	int cluster_count = bundle->getClusters().getSize();
	ASSERT_EQUAL(3, cluster_count);

	// 3 interconnecting fibres
	int fibre_count = bundle->getFibres().getSize();
	ASSERT_EQUAL(3, fibre_count);

	// test connecting to loaded pattern channels
	ASSERTM("TODO: test connecting to loaded pattern channels", false);
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
	ASSERTM("TODO", false);

}

}//NAMESPACE

}//NAMESPACE
