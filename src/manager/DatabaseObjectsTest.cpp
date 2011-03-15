/*
 * DatabaseObjectsTest.cpp
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DatabaseObjectsTest.h"
#include "manager/NodeDatabaseObject.h"
#include "components/Node.h"

using namespace cryomesh::components;

namespace cryomesh {

namespace manager {

void DatabaseObjectsTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertNode));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertConnection));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "DatabaseObjectsTest");
}

void DatabaseObjectsTest::testCreateInsertNode() {
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	node->setActivity(0.5);
	common::TimeKeeper::getTimeKeeper().update();
	node->update();
	unsigned long int id = node->getID();
	double x = node->getPosition().getX();
	double y = node->getPosition().getY();
	double z = node->getPosition().getZ();
	double activity = node->getActivity();
	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();

	std::string id_str = NodeDatabaseObject::toString<unsigned long int>(id);
	std::string x_str = NodeDatabaseObject::toString<double>(x);
	std::string y_str = NodeDatabaseObject::toString<double>(y);
	std::string z_str = NodeDatabaseObject::toString<double>(z);
	std::string activity_str = NodeDatabaseObject::toString<double>(activity);
	std::string cycle_str = NodeDatabaseObject::toString<unsigned long int>(cycle);
	std::stringstream ss;
	ss << "insert into " << "nodesTable" << " (" << NodeDatabaseObject::ID_TAG << ", " << NodeDatabaseObject::X_TAG
			<< ", " << NodeDatabaseObject::Y_TAG << ", " << NodeDatabaseObject::Z_TAG << ", "
			<< NodeDatabaseObject::CYCLE_TAG << ", " << NodeDatabaseObject::ACTIVITY_TAG << ") ";
	ss << " values (" << id_str << ", " << x_str << ", " << y_str << ", " << z_str << ", " << cycle_str << ", "
			<< activity_str << ");";

	std::cout << "DatabaseObjectsTest::testCreateInsertNode: " << "" << std::endl;
	std::cout << ss.str() << std::endl;
	std::cout << node->getDatabaseObject()->getInsert("nodesTable") << std::endl;
	std::string exp_str = ss.str();

	ASSERT_EQUAL( exp_str, node->getDatabaseObject()->getInsert("nodesTable"));

}
void DatabaseObjectsTest::testCreateInsertConnection() {
	ASSERT(false);
}
}//NAMESPACE

}//NAMESPACE
