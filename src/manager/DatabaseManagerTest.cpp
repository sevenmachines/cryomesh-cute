/*
 * DatabaseManagerTest.cpp
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DatabaseManagerTest.h"
#include "manager/DatabaseManager.h"
#include "components/Node.h"
#include <iostream>
#include <fstream>

namespace cryomesh {

namespace manager {

void DatabaseManagerTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(DatabaseManagerTest::testCreation));
	s.push_back(CUTE(DatabaseManagerTest::testCommands));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "DatabaseManagerTest");
}

void DatabaseManagerTest::testCreation() {
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

void DatabaseManagerTest::testCommands() {
	DatabaseManager dbm;
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	node->setActivity(0.5);
	common::TimeKeeper::getTimeKeeper().update();
	node->update();
	double x = node->getPosition().getX();
	double y = node->getPosition().getY();
	double z = node->getPosition().getZ();
	double activity = node->getActivity();
	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();
	for (int i = 0; i < 10; i++) {
		dbm.insertNode(*(node->getDatabaseObject()));
		common::TimeKeeper::getTimeKeeper().update();
	}
	dbm.selectAll();
	dbm.printHistory(std::cout, 2);
	dbm.printHistory(std::cout);
	ASSERTM("TODO", false);
}

}//NAMESPACE

}//NAMESPACE
