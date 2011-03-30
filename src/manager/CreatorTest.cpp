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

const std::string CreatorTest::CONFIG_FILE = "Data/example.config";

void CreatorTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(CreatorTest::testCheckConfigEntry));
	s.push_back(CUTE(CreatorTest::testCheckConfigStructure));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "CreatorTest");
}

void CreatorTest::testCheckConfigEntry() {
	Creator creator(CreatorTest::CONFIG_FILE);

	// good entry, 3 option entry
	{
		std::string entry_str = "create-cluster id=1 size=100 connectivity=10";
		config::ConfigEntry entry(entry_str);
		ASSERT(creator.checkConfigEntry(entry) == true);
	}

	// good entry, 1 option entry
	{
		std::string entry_str = "set-primary-out id=4";
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
		std::string entry_str = "set-primary-out";
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