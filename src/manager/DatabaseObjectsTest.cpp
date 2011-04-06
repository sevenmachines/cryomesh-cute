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
#include "manager/DatabaseManager.h"
#include "state/Pattern.h"
#include "manager/PatternDatabaseObject.h"

using namespace cryomesh::components;

namespace cryomesh {

namespace manager {

void DatabaseObjectsTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertNode));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertConnection));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateInsertPattern));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateConnectionFromDatabaseEntry));
	s.push_back(CUTE(DatabaseObjectsTest::testCreateNodeFromDatabaseEntry));
	s.push_back(CUTE(DatabaseObjectsTest::testCreatePatternFromDatabaseEntry));
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

void DatabaseObjectsTest::testCreateInsertPattern() {
	std::string pattern_string = "1010101010";
	state::Pattern pattern(pattern_string);
	std::string id_str = pattern.getUUIDString();
	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();

	std::string cycle_str = ConnectionDatabaseObject::toString<unsigned long int>(cycle);

	std::stringstream ss;
	ss << "insert into " << "outputsTable" << " (" << PatternDatabaseObject::ID_TAG << ", "
			<< PatternDatabaseObject::CYCLE_TAG << ", " << PatternDatabaseObject::PATTERN_TAG << ") ";
	ss << " values (" << "\'" << id_str << "\'" << ", " << cycle_str << ", " << pattern_string << ");";

	//std::cout << "DatabaseObjectsTest::testCreateInsertConnection: " << "" << std::endl;
	//	std::cout << ss.str() << std::endl;
	//	std::cout << con1->getDatabaseObject()->getInsert("connectionsTable") << std::endl;
	std::string exp_str = ss.str();

	ASSERT_EQUAL( exp_str, pattern.getDatabaseObject()->getInsert("outputsTable"));
}

void DatabaseObjectsTest::testCreateConnectionFromDatabaseEntry() {
	DatabaseManager dbm;
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	boost::shared_ptr<components::Node> node1 = components::Node::getRandom();
	boost::shared_ptr<components::Node> node2 = components::Node::getRandom();
	node->setActivity(-1);
	node1->setActivity(1);
	node2->setActivity(2);
	node->update();
	boost::shared_ptr<components::Connection> con1(new components::Connection);
	boost::shared_ptr<components::Connection> con2(new components::Connection);
	con2->getMutableConnector().connectInput(node1);
	con2->getMutableConnector().connectOutput(node2);
	node1->getMutableConnector().connectOutput(con2);
	node2->getMutableConnector().connectInput(con2);
	node->setActivity(0.5);
	node->update();

	dbm.insertNode(*(node->getDatabaseObject()));
	dbm.insertConnection(*(con2->getDatabaseObject()));

	//connection
	{
		std::stringstream ss;
		ss << "id=" << "'";
		std::string connection_entry = dbm.selectConnection(con2->getUUIDString(),
				common::TimeKeeper::getTimeKeeper().getCycle());
		ConnectionDatabaseObject cdo(connection_entry);

		//get value
		{
			std::string id = cdo.getUUID();
			std::string inputid = cdo.getInputNodeUUID();
			std::string outputid = cdo.getOutputNodeUUID();
			int impulse_count = cdo.getImpulseCount();
			common::Cycle cycle = cdo.getCycle();

			std::string exp_id = con2->getUUIDString();
			std::string exp_inputid = con2->getConnector().getInputs().begin()->second->getUUIDString();
			std::string exp_outputid = con2->getConnector().getOutputs().begin()->second->getUUIDString();
			int exp_impulse_count = con2->getImpulses().getSize();
			common::Cycle exp_cycle = common::TimeKeeper::getTimeKeeper().getCycle();

			ASSERT_EQUAL(exp_id, id);
			ASSERT_EQUAL(exp_inputid, inputid);
			ASSERT_EQUAL(exp_outputid, outputid);
			ASSERT_EQUAL(exp_impulse_count, impulse_count);
			ASSERT_EQUAL(exp_cycle, cycle);
		}
	}
}
void DatabaseObjectsTest::testCreateNodeFromDatabaseEntry() {
	DatabaseManager dbm;
	boost::shared_ptr<components::Node> node = components::Node::getRandom();
	boost::shared_ptr<components::Node> node1 = components::Node::getRandom();
	boost::shared_ptr<components::Node> node2 = components::Node::getRandom();
	node->setActivity(-1);
	node1->setActivity(1);
	node2->setActivity(2);
	node->update();
	boost::shared_ptr<components::Connection> con1(new components::Connection);
	boost::shared_ptr<components::Connection> con2(new components::Connection);
	con2->getMutableConnector().connectInput(node1);
	con2->getMutableConnector().connectOutput(node2);
	node1->getMutableConnector().connectOutput(con2);
	node2->getMutableConnector().connectInput(con2);
	node->setActivity(0.5);
	node->update();

	dbm.insertNode(*(node->getDatabaseObject()));
	dbm.insertConnection(*(con2->getDatabaseObject()));

	//node
	{
		std::stringstream ss;
		ss << "id=" << "'";
		std::string node_entry = dbm.selectNode(node->getUUIDString(), common::TimeKeeper::getTimeKeeper().getCycle());
		NodeDatabaseObject ndo(node_entry);

		//get value
		{
			spacial::Point point = ndo.getPoint();
			double point_x = point.getX();
			double point_y = point.getY();
			double point_z = point.getZ();
			std::string id = ndo.getUUID();
			double activity = ndo.getActivity();
			common::Cycle cycle = ndo.getCycle();

			std::string exp_id = node->getUUIDString();
			spacial::Point exp_point = node->getPosition();
			double exp_point_x = exp_point.getX();
			double exp_point_y = exp_point.getY();
			double exp_point_z = exp_point.getZ();
			common::Cycle exp_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
			double exp_activity = node->getActivity();

			ASSERT_EQUAL(exp_id, id);
			ASSERT_EQUAL_DELTA(exp_point_x, point_x, 0.0001);
			ASSERT_EQUAL_DELTA(exp_point_y, point_y, 0.0001);
			ASSERT_EQUAL_DELTA(exp_point_z, point_z, 0.0001);
			ASSERT_EQUAL_DELTA(exp_activity, activity, 0.0001);
			ASSERT_EQUAL(exp_cycle, cycle);
		}
	}

}

void DatabaseObjectsTest::testCreatePatternFromDatabaseEntry() {
	// generate a pattern
	DatabaseManager dbm;
	std::string pattern_string = "1010101010";
	state::Pattern pattern(pattern_string);
	std::string id_str = pattern.getUUIDString();
	unsigned int cycle = common::TimeKeeper::getTimeKeeper().getCycle().toULInt();

	std::string cycle_str = ConnectionDatabaseObject::toString<unsigned long int>(cycle);

	dbm.insertOutputPattern(*(pattern.getDatabaseObject()));

	//retrieve it
	{
		std::string entry = dbm.selectOutputPattern(pattern.getUUIDString(),
				common::TimeKeeper::getTimeKeeper().getCycle());
		// check equality
		PatternDatabaseObject pdo(entry);
		std::string act_pattern_string = pdo.getPattern().getString();
		std::string act_id_str = pattern.getUUIDString();
		unsigned long int act_cycle = pdo.getCycle().toULInt();

		ASSERT_EQUAL(pattern_string, act_pattern_string);
		ASSERT_EQUAL(id_str, act_id_str);
		ASSERT_EQUAL(cycle, act_cycle);
	}
}
}//NAMESPACE

}//NAMESPACE
