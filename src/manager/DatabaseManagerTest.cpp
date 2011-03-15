/*
 * DatabaseManagerTest.cpp
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DatabaseManagerTest.h"
#include "manager/DatabaseManager.h"
#include <iostream>
#include <fstream>

namespace cryomesh {

namespace manager {

void DatabaseManagerTest::runSuite(){
	cute::suite s;
	s.push_back(CUTE(DatabaseManagerTest::testCreation));
	s.push_back(CUTE(DatabaseManagerTest::testCommands));
cute::ide_listener lis;
	cute::makeRunner(lis)(s, "DatabaseManagerTest");
}

void DatabaseManagerTest::testCreation(){
	// default
	{
		DatabaseManager dbm;
		ASSERT(dbm.isDatabaseAccessable() == true);
		std::ifstream ifs(DatabaseManager::DEFAULT_DATABASE);
		ASSERT(ifs.is_open());
	}

	// default
	{
		DatabaseManager dbm("test.db");
		ASSERT(dbm.isDatabaseAccessable() == true);
		std::ifstream ifs("test.db");
		ASSERT(ifs.is_open());
	}
}

void DatabaseManagerTest::testCommands(){
	DatabaseManager dbm;
	dbm.insert();
	dbm.selectAll();
	ASSERTM("TODO", false);
}

}//NAMESPACE

}//NAMESPACE
