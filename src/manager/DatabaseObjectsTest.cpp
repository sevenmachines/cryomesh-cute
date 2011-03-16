/*
 * DatabaseObjectsTest.cpp
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DatabaseObjectsTest.h"
#include "manager/NodeDatabaseObject.h"
#include "components/Node.h"
#include "components/Connection.h"
#include "manager/ConnectionDatabaseObject.h"

using namespace cryomesh::components;

namespace cryomesh {

namespace manager {

void DatabaseObjectsTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertNode));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertConnection));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateFromDatabaseEntry));
cute::ide_listener lis;
	cute::makeRunner(lis)(s, "DatabaseObjectsTest");
}

void DatabaseObjectsTest::testCreateInsertNode() {
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	node->setActivity(0.5);
	common::TimeKeeper::getTimeKeeper().update();
	node->update();
	double x = node->getPosition().getX();
	double y = node->getPosition().getY();
	double z = node->getPosition().getZ();
	double activity = node->getActivity();
	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();

	std::string id_str = node->getUUIDString();
	std::string x_str = NodeDatabaseObject::toString<double>(x);
	std::string y_str = NodeDatabaseObject::toString<double>(y);
	std::string z_str = NodeDatabaseObject::toString<double>(z);
	std::string activity_str = NodeDatabaseObject::toString<double>(activity);
	std::string cycle_str = NodeDatabaseObject::toString<unsigned long int>(cycle);
	std::stringstream ss;
	ss << "insert into " << "nodesTable" << " (" << NodeDatabaseObject::ID_TAG << ", " << NodeDatabaseObject::X_TAG
			<< ", " << NodeDatabaseObject::Y_TAG << ", " << NodeDatabaseObject::Z_TAG << ", "
			<< NodeDatabaseObject::CYCLE_TAG << ", " << NodeDatabaseObject::ACTIVITY_TAG << ") ";
	ss << " values (" << "\'" << id_str << "\'" << ", " << x_str << ", " << y_str << ", " << z_str << ", " << cycle_str
			<< ", " << activity_str << ");";

	//std::cout << "DatabaseObjectsTest::testCreateInsertNode: " << "" << std::endl;
	//	std::cout << ss.str() << std::endl;
	//	std::cout << node->getDatabaseObject()->getInsert("nodesTable") << std::endl;
	std::string exp_str = ss.str();

	ASSERT_EQUAL( exp_str, node->getDatabaseObject()->getInsert("nodesTable"));

}
void DatabaseObjectsTest::testCreateInsertConnection() {
	boost::shared_ptr<components::Node> node1 = components::Node::getRandom();
	boost::shared_ptr<components::Node> node2 = components::Node::getRandom();
	boost::shared_ptr<components::Connection> con1(new components::Connection());
	boost::shared_ptr<components::Impulse> imp1 = components::Impulse::getRandom();
	boost::shared_ptr<components::Impulse> imp2 = components::Impulse::getRandom();
	con1->getMutableConnector().connectInput(node1);
	con1->getMutableConnector().connectOutput(node2);
	node1->getMutableConnector().connectOutput(con1);
	node2->getMutableConnector().connectInput(con1);
	con1->add(imp1);
	con1->add(imp2);
	common::TimeKeeper::getTimeKeeper().update();

	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();

	std::string id_str = con1->getUUIDString();
	std::string in_id_str = node1->getUUIDString();
	std::string out_id_str = node2->getUUIDString();
	std::string impulse_count_str = ConnectionDatabaseObject::toString<int>(con1->getImpulses().getSize());
	std::string cycle_str = ConnectionDatabaseObject::toString<unsigned long int>(cycle);

	std::stringstream ss;
	ss << "insert into " << "connectionsTable" << " (" << ConnectionDatabaseObject::ID_TAG << ", "
			<< ConnectionDatabaseObject::INPUT_ID_TAG << ", " << ConnectionDatabaseObject::OUTPUT_ID_TAG << ", "
			<< ConnectionDatabaseObject::CYCLE_TAG << ", " << ConnectionDatabaseObject::IMPULSE_COUNT_TAG << ") ";
	ss << " values (" << "\'" << id_str << "\'" << ", " << "\'" << in_id_str << "\'" << ", " << "\'" << out_id_str
			<< "\'" << ", " << cycle_str << ", " << impulse_count_str << ");";

	//std::cout << "DatabaseObjectsTest::testCreateInsertConnection: " << "" << std::endl;
	//	std::cout << ss.str() << std::endl;
	//	std::cout << con1->getDatabaseObject()->getInsert("connectionsTable") << std::endl;
	std::string exp_str = ss.str();

	ASSERT_EQUAL( exp_str, con1->getDatabaseObject()->getInsert("connectionsTable"));
}

void DatabaseObjectsTest::testCreateFromDatabaseEntry(){
	ASSERTM("TODO", false);
}
}//NAMESPACE

}//NAMESPACE
