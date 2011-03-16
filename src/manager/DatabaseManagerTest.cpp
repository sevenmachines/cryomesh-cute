/*
 * DatabaseManagerTest.cpp
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DatabaseManagerTest.h"
#include "manager/DatabaseManager.h"
#include "components/Node.h"
#include "components/Connection.h"
#include <iostream>
#include <fstream>

namespace cryomesh {

namespace manager {

void DatabaseManagerTest::printCount(DatabaseManager & dbm) {
	dbm.countNodes();
	dbm.countConnections();
	//dbm.printHistory(std::cout, 2);
}
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
	dbm.clearTables();
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	boost::shared_ptr<components::Node> node1 = components::Node::getRandom();
	boost::shared_ptr<components::Node> node2 = components::Node::getRandom();
	boost::shared_ptr<components::Connection> con1(new components::Connection);
	boost::shared_ptr<components::Connection> con2(new components::Connection);
	con2->getMutableConnector().connectInput(node1);
	con2->getMutableConnector().connectOutput(node2);
	node1->getMutableConnector().connectOutput(con2);
	node2->getMutableConnector().connectInput(con2);
	node->setActivity(0.5);
	common::TimeKeeper::getTimeKeeper().update();
	node->update();

	ASSERT_EQUAL(0, dbm.countNodes());
	ASSERT_EQUAL(0, dbm.countConnections());

	dbm.insertNode(*(node->getDatabaseObject()));
	ASSERT_EQUAL(1, dbm.countNodes());
	ASSERT_EQUAL(0, dbm.countConnections());

	dbm.insertConnection(*(con1->getDatabaseObject()));
	ASSERT_EQUAL(1, dbm.countNodes());
	ASSERT_EQUAL(1, dbm.countConnections());

	dbm.insertConnection(*(con2->getDatabaseObject()));
	ASSERT_EQUAL(1, dbm.countNodes());
	ASSERT_EQUAL(2, dbm.countConnections());

	dbm.insertNode(*(node1->getDatabaseObject()));
	ASSERT_EQUAL(2, dbm.countNodes());
	ASSERT_EQUAL(2, dbm.countConnections());

	dbm.insertNode(*(node2->getDatabaseObject()));
	ASSERT_EQUAL(3, dbm.countNodes());
	ASSERT_EQUAL(2, dbm.countConnections());

	common::TimeKeeper::getTimeKeeper().update();

	//	dbm.selectNodes();
	//dbm.selectConnections();
	//	dbm.printHistory(std::cout, 10);
}

}//NAMESPACE

}//NAMESPACE
