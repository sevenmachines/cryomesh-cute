/*
 * ConnectionTest.cpp
 *
 *  Created on: 14 Feb 2011
 *      Author: niall
 */

#include "ConnectionTest.h"
#include "components/Connection.h"

namespace cryomesh {

namespace components {

void ConnectionTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ConnectionTest::testCreationConnection));
	s.push_back(CUTE(ConnectionTest::testUpdate));
	s.push_back(CUTE(ConnectionTest::testImpulsePropagation));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ConnectionTest");
}

void ConnectionTest::testImpulsePropagation() {
	boost::shared_ptr<Node> node1 = Node::getRandom();
	boost::shared_ptr<Node> node2 = Node::getRandom();
	boost::shared_ptr<Connection> connection1(new Connection);

	//node1->setDebug(true);
	//node2->setDebug(true);
	//connection1->setDebug(true);

	connection1->getMutableConnector().connectInput(node1);
	node1->getMutableConnector().connectOutput(connection1);
	connection1->getMutableConnector().connectOutput(node2);
	node2->getMutableConnector().connectInput(connection1);

	node1->addImpulse(Impulse::getTriggerImpulse());

	// check start structure
	{
		int node1_inputs = node1->getConnector().getInputs().size();
		int node1_outputs = node1->getConnector().getOutputs().size();
		int node2_inputs = node2->getConnector().getInputs().size();
		int node2_outputs = node2->getConnector().getOutputs().size();

		int connection_inputs = connection1->getConnector().getInputs().size();
		int connection_outputs = connection1->getConnector().getOutputs().size();

		ASSERT_EQUAL(0, node1_inputs);
		ASSERT_EQUAL(1, node1_outputs);
		ASSERT_EQUAL(1, node2_inputs);
		ASSERT_EQUAL(0, node2_outputs);
		ASSERT_EQUAL(1, connection_inputs);
		ASSERT_EQUAL(1, connection_outputs);
	}
	// check start state
	{
		ASSERT_EQUAL(false, node1->isTriggered());
		ASSERT_EQUAL(true,node1->isActive());
		ASSERT_EQUAL(true,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(0, connection1->getImpulses().getSize());

	}
	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	//std::cout << "ConnectionTest::testImpulsePropagation: 2" << *node1 << std::endl;

	// check  state
	{
		ASSERT_EQUAL(true, node1->isTriggered());
		ASSERT_EQUAL(false,node1->isActive());
		ASSERT_EQUAL(false,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(1, connection1->getImpulses().getSize());

	}
	common::TimeKeeper::getTimeKeeper().update();
	node1->update();
	//std::cout << "ConnectionTest::testImpulsePropagation: 3" << *node1 << std::endl;

	{
		ASSERT_EQUAL(false, node1->isTriggered());
		ASSERT_EQUAL(false,node1->isActive());
		ASSERT_EQUAL(false,node1->isLive());
		ASSERT_EQUAL(false, node2->isTriggered());
		ASSERT_EQUAL(false,node2->isActive());
		ASSERT_EQUAL(false,node2->isLive());
		ASSERT_EQUAL(1, connection1->getImpulses().getSize());

	}

	// update until we pass on the connections impulse
	bool impulse_propagated = false;
	int count_limit = 1000;
	while (impulse_propagated == false && count_limit > 0) {
		common::TimeKeeper::getTimeKeeper().update();
		connection1->update();
		node1->update();
		node2->update();
		if (connection1->getImpulses().getSize() < 1) {
			impulse_propagated = true;
			ASSERT_EQUAL(1, node2->getImpulses().getSize());

			// work for delay
			boost::shared_ptr< Impulse > node2_imp = node2->getImpulses().begin()->second;
			int delay = node2_imp->getActivityDelay();
			common::Cycle start_cyc = node2_imp->getFirstActiveCycle();
			int count =0;
			while (common::TimeKeeper::getTimeKeeper().getCycle()<start_cyc){
				common::TimeKeeper::getTimeKeeper().update();
				node2->update();
				++count;
			}
			ASSERT_EQUAL(count, delay);

			ASSERT_EQUAL(false, node2->isTriggered());
			ASSERT_EQUAL(true,node2->isActive());
			ASSERT_EQUAL(true,node2->isLive());
			//std::cout << "ConnectionTest::testImpulsePropagation: " << *node2 << std::endl;
		} else {
			ASSERT_EQUAL(false, node1->isTriggered());
			ASSERT_EQUAL(false,node1->isActive());
			ASSERT_EQUAL(false,node1->isLive());
			ASSERT_EQUAL(false, node2->isTriggered());
			ASSERT_EQUAL(false,node2->isActive());
			ASSERT_EQUAL(false,node2->isLive());
		}
		--count_limit;
	}
}
void ConnectionTest::testCreationConnection() {
	Connection con;

	const double DELTA = 0.00001;
	// test we have some sort of random activity timer
	{
		boost::shared_ptr<components::ActivityTimerDistance> act_timer = con.getActivityTimer();
		ASSERT(act_timer->getDelay()>0);
		ASSERT(act_timer->getDecrement()>0);
	}
	boost::shared_ptr<Impulse> imp1(new Impulse);
	boost::shared_ptr<Impulse> imp2(new Impulse);
	boost::shared_ptr<Impulse> imp3(new Impulse);
	imp1->randomise();
	imp2->randomise();
	imp3->randomise();

	con.add(imp1);
	con.add(imp2);
	// test to see added impulses have there timers set correctly
	{
		boost::shared_ptr<components::ActivityTimerDistance> con_act_timer = con.getActivityTimer();
		boost::shared_ptr<components::ActivityTimerDistance> imp1_act_timer = imp1->getActivityTimer();
		boost::shared_ptr<components::ActivityTimerDistance> imp2_act_timer = imp2->getActivityTimer();
		ASSERT_EQUAL_DELTA(con_act_timer->getDelay(), imp1_act_timer->getDelay(), DELTA);
		ASSERT_EQUAL_DELTA(con_act_timer->getDelay(), imp2_act_timer->getDelay(), DELTA);
		ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp1_act_timer->getDecrement(), DELTA);
		ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp2_act_timer->getDecrement(), DELTA);
	}
	{
		int sz = con.getImpulses().getSize();
		ASSERT_EQUAL(2, sz);
	}
	con.add(imp1);
	con.add(imp2);
	{
		int sz = con.getImpulses().getSize();
		ASSERT_EQUAL(2, sz);
	}
	con.remove(imp1);
	{
		int sz = con.getImpulses().getSize();
		ASSERT_EQUAL(1, sz);
	}
	con.remove(imp2);
	{
		int sz = con.getImpulses().getSize();
		ASSERT_EQUAL(0, sz);
	}

	ASSERT(false);
}

void ConnectionTest::testUpdate() {
	const double DELTA = 0.00001;

	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	tk.reset();

	// set up con->node 1:1
	boost::shared_ptr<Connection> con1(new Connection);
	boost::shared_ptr<Node> node1(new Node);
	con1->getMutableConnector().connectOutput(node1);
	node1->getMutableConnector().connectInput(con1);

	const common::Cycle start_cycle = tk.getCycle();
	// Set up knows impulses
	boost::shared_ptr<Impulse> imp1(new Impulse(1, 9, 0));
	boost::shared_ptr<Impulse> imp2(new Impulse(1, 6, 2));
	boost::shared_ptr<Impulse> imp3(new Impulse(1, 3, 4));
	imp1->setFirstActiveCycle(start_cycle);
	imp2->setFirstActiveCycle(start_cycle);
	imp3->setFirstActiveCycle(start_cycle);

	con1->add(imp1);

	// test to see added impulses have there timers set correctly
	{
		boost::shared_ptr<components::ActivityTimerDistance> con_act_timer = con1->getActivityTimer();
		boost::shared_ptr<components::ActivityTimerDistance> imp1_act_timer = imp1->getActivityTimer();
		ASSERT_EQUAL_DELTA(con_act_timer->getDelay(), imp1_act_timer->getDelay(), DELTA);
		ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp1_act_timer->getDecrement(), DELTA);
	}
	con1->update();

	con1->add(imp2);
	// test to see added impulses have there timers set correctly
	{
		boost::shared_ptr<components::ActivityTimerDistance> con_act_timer = con1->getActivityTimer();
		boost::shared_ptr<components::ActivityTimerDistance> imp2_act_timer = imp2->getActivityTimer();
		ASSERT_EQUAL_DELTA(con_act_timer->getDelay(), imp2_act_timer->getDelay(), DELTA);
		ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp2_act_timer->getDecrement(), DELTA);
	}
	con1->update();
	con1->update();
	con1->add(imp3);
	// Update
	{
		con1->update();
		// test to see added impulses have there timers set correctly
		{
			boost::shared_ptr<components::ActivityTimerDistance> con_act_timer = con1->getActivityTimer();
			boost::shared_ptr<components::ActivityTimerDistance> imp1_act_timer = imp1->getActivityTimer();
			boost::shared_ptr<components::ActivityTimerDistance> imp2_act_timer = imp2->getActivityTimer();
			const double con_delay = con_act_timer->getDelay();
			const double imp1_delay = imp1_act_timer->getDelay();
			ASSERT(con_delay>imp1_delay);
			ASSERT(con_act_timer->getDelay()> imp2_act_timer->getDelay());
			ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp1_act_timer->getDecrement(), DELTA);
			ASSERT_EQUAL_DELTA(con_act_timer->getDecrement(), imp2_act_timer->getDecrement(), DELTA);
		}
		const double imp1count = imp1->getActivityTimer()->getDelay() / imp1->getActivityTimer()->getDecrement();
		const double imp2count = imp2->getActivityTimer()->getDelay() / imp2->getActivityTimer()->getDecrement();
		const double imp3count = imp3->getActivityTimer()->getDelay() / imp3->getActivityTimer()->getDecrement();
		//	std::cout << "ConnectionTest::testUpdate: " << "imp1count:" << imp1count << " imp2count:" << imp1count
		//			<< "imp3count:" << imp1count << std::endl;
		double max_count = std::max(imp1count, imp2count);
		max_count = std::max(max_count, imp3count);
		for (int i = 0; i < max_count; i++) {
			con1->update();
			int exp_count = 3;
			if (i > imp1count - 1) {
				--exp_count;
			}
			if (i > imp2count - 1) {
				--exp_count;
			}
			if (i > imp3count - 1) {
				--exp_count;
			}
			//	std::cout << "ConnectionTest::testUpdate: " << "i:" << i << " exp_count:" << exp_count << std::endl;
			ASSERT_EQUAL(exp_count, con1->getImpulses().getSize());

			ASSERT_EQUAL(3-exp_count, node1->getImpulses().getSize());
		}

		// count our impulses
		{
			unsigned int con_count = con1->getImpulses().getSize();
			unsigned int node_count = node1->getImpulses().getSize();

			ASSERT_EQUAL(0 , con_count);
			ASSERT_EQUAL(3, node_count);
		}
	}
	ASSERT(false);
}
}//NAMESPACE

}//NAMESPACE
