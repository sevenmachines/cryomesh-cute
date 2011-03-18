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
	const common::Cycle START_CYCLE = common::TimeKeeper::getTimeKeeper().getCycle();
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	boost::shared_ptr<components::Node> node1 = components::Node::getRandom();
	boost::shared_ptr<components::Node> node2 = components::Node::getRandom();
	node->setActivity(-1);
	node1->setActivity(1);
	node2->setActivity(2);
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
	common::TimeKeeper::getTimeKeeper().update();

	dbm.insertConnection(*(con1->getDatabaseObject()));
	ASSERT_EQUAL(1, dbm.countNodes());
	ASSERT_EQUAL(1, dbm.countConnections());
	common::TimeKeeper::getTimeKeeper().update();

	dbm.insertConnection(*(con2->getDatabaseObject()));
	ASSERT_EQUAL(1, dbm.countNodes());
	ASSERT_EQUAL(2, dbm.countConnections());

	dbm.insertNode(*(node1->getDatabaseObject()));
	ASSERT_EQUAL(2, dbm.countNodes());
	ASSERT_EQUAL(2, dbm.countConnections());
	common::TimeKeeper::getTimeKeeper().update();
	dbm.insertConnection(*(con2->getDatabaseObject()));
	ASSERT_EQUAL(2, dbm.countNodes());
	ASSERT_EQUAL(3, dbm.countConnections());

	dbm.insertNode(*(node1->getDatabaseObject()));
	ASSERT_EQUAL(3, dbm.countNodes());
	ASSERT_EQUAL(3, dbm.countConnections());
	common::TimeKeeper::getTimeKeeper().update();
	dbm.insertNode(*(node2->getDatabaseObject()));
	ASSERT_EQUAL(4, dbm.countNodes());
	ASSERT_EQUAL(3, dbm.countConnections());

	//get value
	{
		std::string id = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "id");
		std::string x = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "x");
		std::string y = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "y");
		std::string z = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "z");
		std::string cycle = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "cycle");
		std::string activity = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "activity");

		std::string exp_id = node1->getUUIDString();
		std::stringstream exp_x;
		exp_x << node1->getPosition().getX();
		std::stringstream exp_y;
		exp_y << node1->getPosition().getY();
		std::stringstream exp_z;
		exp_z << node1->getPosition().getZ();
		std::stringstream exp_cycle;
		exp_cycle << common::TimeKeeper::getTimeKeeper().getCycle() - 2;
		std::stringstream exp_activity;
		exp_activity << node1->getActivity();

		ASSERT_EQUAL(exp_id, id);
		ASSERT_EQUAL(exp_x.str(), x);
		ASSERT_EQUAL(exp_y.str(), y);
		ASSERT_EQUAL(exp_z.str(), z);
		ASSERT_EQUAL_DELTA(atof(exp_activity.str().c_str()), atof(activity.c_str()), 0.0001);
		//	ASSERT_EQUAL(160, common::TimeKeeper::getTimeKeeper().getCycle().toLInt());
		ASSERT_EQUAL(exp_cycle.str(), cycle);
	}

	// update
	{
		dbm.updateNode(node1->getUUIDString(), START_CYCLE + 3, "x=10");
		std::string result = dbm.selectNodeValue(node1->getUUIDString(), START_CYCLE + 3, "x");
		//dbm.printHistory(std::cout, 1);
		ASSERT_EQUAL(10, atoi(result.c_str()) );
		//dbm.selectNodes();
	}

	//test deletion;
	{
		//node
		{
			std::stringstream ss;
			ss << "id=" << "\'" << node1->getUUIDString() << "\'";
			dbm.deleteNodes(ss.str());
			ASSERT_EQUAL(2, dbm.countNodes());
			ASSERT_EQUAL(3, dbm.countConnections());
		}

		//connection
		{
			dbm.deleteConnection(con1->getUUIDString());
			ASSERT_EQUAL(2, dbm.countNodes());
			ASSERT_EQUAL(2, dbm.countConnections());
		}
	}

	//delete by cycle
	{
		dbm.selectNodes();
		dbm.selectConnections();
		long int now = common::TimeKeeper::getTimeKeeper().getCycle().toLInt();
		dbm.deleteAllByCycle(now, -1);
		//ASSERT_EQUAL(common::TimeKeeper::getTimeKeeper().getCycle(), common::Cycle(6));
		ASSERT_EQUAL(1, dbm.countNodes());
		ASSERT_EQUAL(0, dbm.countConnections());
	}
}

}//NAMESPACE

}//NAMESPACE
