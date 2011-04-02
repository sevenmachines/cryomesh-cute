/*
 * NodeTest.cpp
 *
 *  Created on: 19 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "NodeTest.h"
#include "components/Connection.h"
#include "common/TimeKeeper.h"

namespace cryomesh {
namespace components {

NodeTest::NodeTest() {

}

NodeTest::~NodeTest() {
}

void NodeTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(NodeTest::testContruction));
	s.push_back(CUTE(NodeTest::testUpdateImpulses));
	s.push_back(CUTE(NodeTest::testEmitmpulse));
	s.push_back(CUTE(NodeTest::testCheckActivation));
	s.push_back(CUTE(NodeTest::testGetActivities));
	s.push_back(CUTE(NodeTest::testGetSetCurrentActivity));
	s.push_back(CUTE(NodeTest::testAddActivity));
	s.push_back(CUTE(NodeTest::testDataObject));
	s.push_back(CUTE(NodeTest::testSpacialSettings));
	s.push_back(CUTE(NodeTest::testAddImpulse));
	s.push_back(CUTE(NodeTest::testForceFire));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "NodeTest");
}

void NodeTest::testContruction() {
	NodeTest nt;
	const Impulse & imp = *(nt.getEmittedImpulse());
	ASSERT(imp.getSum() !=0);
}

void NodeTest::testUpdateImpulses() {

	//check no fire but experation only
	{
		common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
		const long int BASE_CYCLE = tk.getCycle().toLInt();
		boost::shared_ptr<Node> node1 = getDefaultNode();

		const int LENGTH1 = 10;
		boost::shared_ptr<Impulse> imp1(new Impulse(0.0001, LENGTH1));
		boost::shared_ptr<Impulse> imp2(new Impulse(0.0001, LENGTH1));

		const int DELAY2 = 2;
		node1->addImpulse(imp1);

		//	std::cout << "NodeTest::testUpdateImpulses: " << "cycle: " << common::TimeKeeper::getTimeKeeper().getCycle()
		//			<< std::endl;
		//	std::cout << *node1 << std::endl;
		//std::cout << "NodeTest::testUpdateImpulses: " << "imp1: " << "(" << imp1->getFirstActiveCycle().toLInt() << ","
		//			<< imp1->getLastActiveCycle().toLInt() << ")" << std::endl;

		for (int i = 0; i < LENGTH1 + DELAY2 + 5; i++) {
			tk.update();
			if (i == 1) {
				node1->addImpulse(imp2);
				//		std::cout << "NodeTest::testUpdateImpulses: " << "imp2: " << "("
				//			<< imp2->getFirstActiveCycle().toLInt() << "," << imp2->getLastActiveCycle().toLInt() << ")"
				//				<< std::endl;
			}
			//	std::cout << "NodeTest::testUpdateImpulses: " << i << ": " << "cycle: " << tk.getCycle().toLInt() << std::endl;
			node1->update();
			int sz = node1->getImpulses().getSize();
			if (i < 10 && i >= 1) {
				ASSERT_EQUAL(2, sz);
			} else if (i < 12) {
				// imp1 is now dropped
				ASSERT_EQUAL(1, sz);
			} else {
				//imp2 is dropped
				ASSERT_EQUAL(0, sz);
			}

		}
	}

	//check  fire
	//	std::cout << "NodeTest::testUpdateImpulses: " << "########### FIRE ###########" << std::endl;
	{
		{
			common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
			const long int BASE_CYCLE = tk.getCycle().toLInt();
			boost::shared_ptr<Node> node1 = getDefaultNode();

			const int LENGTH1 = 10;
			boost::shared_ptr<Impulse> imp1(new Impulse(2, LENGTH1));
			boost::shared_ptr<Impulse> imp2(new Impulse(4, LENGTH1));

			const int DELAY2 = 2;
			node1->addImpulse(imp1);

			//	std::cout << "NodeTest::testUpdateImpulses: " << "cycle: " << common::TimeKeeper::getTimeKeeper().getCycle()
			//			<< std::endl;
			//	std::cout << *node1 << std::endl;
			//std::cout << "NodeTest::testUpdateImpulses: " << "imp1: " << "(" << imp1->getFirstActiveCycle().toLInt()
			//		<< "," << imp1->getLastActiveCycle().toLInt() << ")" << std::endl;

			for (int i = 0; i < LENGTH1 + DELAY2 + 5; i++) {
				tk.update();
				if (i == 1) {
					node1->addImpulse(imp2);
					//	std::cout << "NodeTest::testUpdateImpulses: " << "imp2: " << "("
					//			<< imp2->getFirstActiveCycle().toLInt() << "," << imp2->getLastActiveCycle().toLInt()
					//				<< ")" << std::endl;
				}
				//	std::cout << "NodeTest::testUpdateImpulses: " << i << ": " << "cycle: " << tk.getCycle().toLInt() << std::endl;
				node1->update();
				int sz = node1->getImpulses().getSize();
				if (i < 1) {
					ASSERT_EQUAL(1, sz);
				} else if (i < 2 && i >= 1) {
					ASSERT_EQUAL(2, sz);
				} else {
					//imp2 is dropped
					ASSERT_EQUAL(0, sz);
				}

			}
		}
	}
}
void NodeTest::testEmitmpulse() {
	// test positive
	{
		boost::shared_ptr<Node> node1 = getDefaultNode();
		bool original_positive = (node1->getEmittedImpulse()->getSum() > 0);
		node1->emitImpulsePositive();

		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & objs_in =
				node1->getConnector().getInputs();
		ASSERT_EQUAL(2, objs_in.size());
		// forall in objs_in
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_in = objs_in.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_in_end =
					objs_in.end();
			while (it_objs_in != it_objs_in_end) {
				int impssz = it_objs_in->second->getImpulses().getSize();
				ASSERT_EQUAL(0, impssz);
				++it_objs_in;
			}
		}

		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & objs_out =
				node1->getConnector().getOutputs();
		ASSERT_EQUAL(3, objs_out.size());
		// forall in objs_out
		{
			int count = 0;
			std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_out = objs_out.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_out_end =
					objs_out.end();
			while (it_objs_out != it_objs_out_end) {
				int impssz = it_objs_out->second->getImpulses().getSize();
				//	std::cout << "NodeTest::testEmitmpulse: " << "count: " << count << std::endl;
				ASSERT_EQUAL(1, impssz);
				const boost::shared_ptr<Impulse> temppulse =
						(it_objs_out->second->getImpulses().getCollection().begin()->second);
				double sum = temppulse->getSum();
				//	std::cout << "NodeTest::testEmitmpulse: " << "sum:" << sum << std::endl;
				if (original_positive == true) {
					ASSERT(sum>0);
				} else {
					ASSERT(sum<0);
				}
				++count;
				++it_objs_out;
			}
		}
	}

	// test negative
	{
		boost::shared_ptr<Node> node1 = getDefaultNode();
		bool original_positive = (node1->getEmittedImpulse()->getSum() > 0);
		node1->emitImpulseNegative();

		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & objs_in =
				node1->getConnector().getInputs();
		ASSERT_EQUAL(2, objs_in.size());
		// forall in objs_in
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_in = objs_in.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_in_end =
					objs_in.end();
			while (it_objs_in != it_objs_in_end) {
				int impssz = it_objs_in->second->getImpulses().getSize();
				ASSERT_EQUAL(0, impssz);
				++it_objs_in;
			}
		}

		const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & objs_out =
				node1->getConnector().getOutputs();
		ASSERT_EQUAL(3, objs_out.size());
		// forall in objs_out
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_out = objs_out.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_objs_out_end =
					objs_out.end();
			while (it_objs_out != it_objs_out_end) {
				int impssz = it_objs_out->second->getImpulses().getSize();
				ASSERT_EQUAL(1, impssz);
				const boost::shared_ptr<Impulse> temppulse =
						(it_objs_out->second->getImpulses().getCollection().begin()->second);
				double sum = temppulse->getSum();
				//	std::cout << "NodeTest::testEmitmpulse: " << "sum:" << sum << std::endl;
				if (original_positive == true) {
					ASSERT(sum<0);
				} else {
					ASSERT(sum>0);
				}
				++it_objs_out;
			}
		}
	}
}
void NodeTest::testCheckActivation() {
	// No activation -> Positive activation
	{
		boost::shared_ptr<Node> node1 = getDefaultNode();
		boost::shared_ptr<Impulse> imp1(new Impulse(2, 10));
		boost::shared_ptr<Impulse> imp2(new Impulse(2, 10));

		node1->addImpulse(imp1);
		node1->addImpulse(imp2);

		//	std::cout << "NodeTest::testCheckActivation: " << "" << std::endl;
		//	std::cout << node1->getImpulses() << std::endl;
		Node::ActivationState act_state;

		act_state = node1->checkActivationState();
		bool statenone = (Node::None == act_state);
		ASSERT(statenone);

		common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
		for (int i = 0; i < 1; ++i) {
			tk.update();
			node1->updateActivity();
			//	std::cout << "NodeTest::testCheckActivation: " << "time:" << tk.getCycle().toLInt() << "act: "
			//			<< node1->updateActivity() << std::endl;
		}
		act_state = node1->checkActivationState();
		statenone = (Node::None == act_state);
		ASSERT(statenone);

		// Positive activation
		for (int i = 0; i < 3; ++i) {
			tk.update();
			node1->updateActivity();
			//	std::cout << "NodeTest::testCheckActivation: " << "time:" << tk.getCycle().toLInt() << "act: "
			//			<< node1->updateActivity() << std::endl;
		}
		act_state = node1->checkActivationState();
		bool statepos = (Node::Positive == act_state);
		ASSERT(statepos);
	}

	// No activation -> Negative activation
	{
		boost::shared_ptr<Node> node1 = getDefaultNode();
		boost::shared_ptr<Impulse> imp1(new Impulse(-2, 10));
		boost::shared_ptr<Impulse> imp2(new Impulse(-2, 10));

		node1->addImpulse(imp1);
		node1->addImpulse(imp2);

		//	std::cout << "NodeTest::testCheckActivation: " << "impulses:" << std::endl;
		//	std::cout << node1->getImpulses() << std::endl;
		Node::ActivationState act_state;
		act_state = node1->checkActivationState();

		bool statenone = (Node::None == act_state);
		ASSERT(statenone);
		common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
		for (int i = 0; i < 1; ++i) {
			tk.update();
			node1->updateActivity();
			//	std::cout << "NodeTest::testCheckActivation: " << "time:" << tk.getCycle().toLInt() << "act: "
			//			<< node1->updateActivity() << std::endl;
		}
		act_state = node1->checkActivationState();

		statenone = (Node::None == act_state);
		ASSERT(statenone);
		// Negative activation
		for (int i = 0; i < 3; ++i) {
			tk.update();
			node1->updateActivity();
			//		std::cout << "NodeTest::testCheckActivation: " << "time:" << tk.getCycle().toLInt() << " act: "
			//			<< node1->updateActivity() << std::endl;
		}
		act_state = node1->checkActivationState();
		bool stateneg = (Node::Negative == act_state);
		ASSERT(stateneg);
	}
}

void NodeTest::testGetActivities() {
	NodeTest nt;
	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	nt.addActivity(tk.getCycle(), 1);
	nt.addActivity(tk.getCycle() + 1, 2);
	nt.addActivity(tk.getCycle() + 2, 3);
	ASSERT_EQUAL(3, nt.getActivities().size());
}

void NodeTest::testGetSetCurrentActivity() {
	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	NodeTest nt;
	// Can no longer set activity directly

	nt.addActivity(tk.getCycle(), 1);
	nt.addActivity(tk.getCycle() + 1, 2);
	nt.addActivity(tk.getCycle() + 2, 3);
	int act_sz = nt.getActivities().size();
	ASSERT_EQUAL(3, act_sz);
	ASSERT_EQUAL_DELTA(1, nt.getActivity() , 0.00001);
	ASSERT_EQUAL_DELTA(3, nt.getActivity(tk.getCycle() + 2) , 0.00001);

	nt.setActivity(4);
	ASSERT_EQUAL(3, nt.getActivities().size());
	ASSERT_EQUAL_DELTA(4, nt.getActivity(), 0.00001);
}

void NodeTest::testAddActivity() {
	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	NodeTest nt;
	// over egg
	for (int i = 0; i < Node::MAX_ACTIVITIES_LENGTH + 10; i++) {
		nt.addActivity(tk.getCycle() + i, 1);
	}
	ASSERT_EQUAL(Node::MAX_ACTIVITIES_LENGTH, nt.getActivities().size());

}

void NodeTest::testDataObject() {
	boost::shared_ptr<Impulse> imp1 = Impulse::getRandom();
	boost::shared_ptr<Impulse> imp2 = Impulse::getRandom();
	boost::shared_ptr<Impulse> imp3 = Impulse::getRandom();
	boost::shared_ptr<Impulse> imp4 = Impulse::getRandom();
	boost::shared_ptr<Impulse> imp5 = Impulse::getRandom();
	boost::shared_ptr<Node> node(new Node);
	ASSERT(imp1!=0);
	ASSERT(imp2!=0);
	ASSERT(imp4!=0);
	ASSERT(imp5!=0);
	ASSERT(imp3!=0);
	ASSERT(node !=0);
	node->addImpulse(imp1);
	node->addImpulse(imp2);
	node->addImpulse(imp3);
	node->addImpulse(imp4);
	node->addImpulse(imp5);

	// active logging
	node->enableLogging(true);
	const unsigned int MAX_COUNT = 105;
	for (unsigned int i = 0; i < MAX_COUNT; i++) {
		common::TimeKeeper::getTimeKeeper().update();
		node->update();
	}

	const dataobjects::DataObject<unsigned long int, double> & datamap = node->getDataObject();
	//std::cout << "NodeTest::testDataObject: " << "" << std::endl;
	//std::cout << datamap << std::endl;
	ASSERT_EQUAL(100, datamap.getMap().size());
}

void NodeTest::testSpacialSettings() {
	boost::shared_ptr<Connection> con(new Connection);
	boost::shared_ptr<Node> node1 = components::Node::getRandom();
	boost::shared_ptr<Node> node2 = components::Node::getRandom();
	node1->getMutableConnector().connectOutput(con);
	node2->getMutableConnector().connectInput(con);
	con->getMutableConnector().connectInput(node1);
	con->getMutableConnector().connectOutput(node2);

	int innode_count = con->getConnector().getInputs().size();
	int outnode_count = con->getConnector().getOutputs().size();
	ASSERT_EQUAL(1, innode_count);
	ASSERT_EQUAL(1, outnode_count);
	node1->setPosition(spacial::Point(10, 10, 10));

	spacial::Point point1_pre = node1->getPosition();
	spacial::Point point2_pre = node2->getPosition();
	double distance_pre = con->getActivityTimer()->getDelay();
	double dist_pre_exp = point1_pre.getDistance(point2_pre);
	ASSERT_EQUAL_DELTA(dist_pre_exp, distance_pre, 0.00001);
	// move
	node1->setPosition(spacial::Point(50, 50, 50));
	spacial::Point point1_post = node1->getPosition();
	spacial::Point point2_post = node2->getPosition();
	double distance_post = con->getActivityTimer()->getDelay();
	double dist_post_exp = point1_post.getDistance(point2_post);
	ASSERT_EQUAL(point2_pre, point2_post);
	ASSERT( !(point1_pre == point1_post));
	ASSERT_EQUAL(spacial::Point(50,50,50), point1_post);
	ASSERT_EQUAL_DELTA(dist_post_exp, distance_post, 0.00001);

}

void NodeTest::testAddImpulse() {
	const common::Cycle BASE_CYCLE = common::TimeKeeper::getTimeKeeper().getCycle();
	boost::shared_ptr<Node> node1 = getDefaultNode();
	boost::shared_ptr<Impulse> imp1(new Impulse(2, 10));
	boost::shared_ptr<Impulse> imp2(new Impulse(4, 3));

	const int DELAY1 = 2;
	const int DELAY2 = 4;
	const int DELAY3 = 7;

	{
		for (int i = 0; i < DELAY1; i++) {
			common::TimeKeeper::getTimeKeeper().update();
		}
		node1->addImpulse(imp1);
		const common::Cycle start = imp1->getFirstActiveCycle();
		const common::Cycle now = common::TimeKeeper::getTimeKeeper().getCycle();
		ASSERT_EQUAL(now, start);
	}

	{
		for (int i = 0; i < DELAY2; i++) {
			common::TimeKeeper::getTimeKeeper().update();
		}
		node1->addImpulse(imp2);
		const common::Cycle start = imp2->getFirstActiveCycle();
		const common::Cycle now = common::TimeKeeper::getTimeKeeper().getCycle();
		ASSERT_EQUAL(now, start);
	}

	// with delay
	const common::Cycle BASE_DELAY3(3);
	boost::shared_ptr<Impulse> imp3(new Impulse(5, 2, BASE_DELAY3));
	{
		for (int i = 0; i < DELAY3; i++) {
			common::TimeKeeper::getTimeKeeper().update();
		}
		node1->addImpulse(imp3);
		const common::Cycle start = imp3->getFirstActiveCycle();
		const common::Cycle now = common::TimeKeeper::getTimeKeeper().getCycle();
		ASSERT_EQUAL(now+BASE_DELAY3, start);
	}
}

void NodeTest::testForceFire() {
	common::Cycle start_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	std::cout << "NodeTest::testForceFire: " << "start time: " << start_cycle << std::endl;
	boost::shared_ptr<Node> node1 = Node::getRandom();
	node1->setDebug(true);
	std::cout << "NodeTest::testForceFire: " << *node1 << std::endl;
	ASSERT(node1->getLastActivationState() == None);
	node1->forceFire();
	double act1 = node1->getActivity();

	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	std::cout << "NodeTest::testForceFire: " << *node1 << std::endl;
	common::Cycle cycle1_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	std::cout << "NodeTest::testForceFire: " << "cycle1 time: " << cycle1_cycle << std::endl;
	double act2 = node1->getActivity();
	std::cout << "NodeTest::testForceFire: " << "act1:" << act1 << " act2:" << act2 << std::endl;
	bool to_fire = (node1->getLastActivationState() == Positive) || (node1->getLastActivationState() == Negative);
	ASSERT(to_fire);

	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	ASSERT_EQUAL(None, node1->getLastActivationState());

}

boost::shared_ptr<Node> NodeTest::getDefaultNode() {
	boost::shared_ptr<Connection> con_in1(new Connection);
	boost::shared_ptr<Connection> con_in2(new Connection);
	boost::shared_ptr<Connection> con_out1(new Connection);
	boost::shared_ptr<Connection> con_out2(new Connection);
	boost::shared_ptr<Connection> con_out3(new Connection);
	boost::shared_ptr<Node> node1 = Node::getRandom();
	node1->getMutableConnector().connectInput(con_in1);
	node1->getMutableConnector().connectInput(con_in2);
	node1->getMutableConnector().connectOutput(con_out1);
	node1->getMutableConnector().connectOutput(con_out2);
	node1->getMutableConnector().connectOutput(con_out3);
	int insz = node1->getConnector().getInputs().size();
	int outsz = node1->getConnector().getOutputs().size();
	ASSERT_EQUAL(2, insz);
	ASSERT_EQUAL(3, outsz);
	return node1;

}
} // namespace
} // namespace
