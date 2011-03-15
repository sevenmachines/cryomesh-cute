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
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ConnectionTest");
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
	boost::shared_ptr<Impulse> imp1(new Impulse(1, 9, start_cycle + common::Cycle(0)));
	boost::shared_ptr<Impulse> imp2(new Impulse(1, 6, start_cycle + common::Cycle(2)));
	boost::shared_ptr<Impulse> imp3(new Impulse(1, 3, start_cycle + common::Cycle(4)));

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
