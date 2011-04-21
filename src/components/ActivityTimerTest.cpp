/*
 * ActivityTimerTest.cpp
 *
 *  Created on: 1 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ActivityTimerTest.h"
#include "components/ActivityTimerDistance.h"

namespace cryomesh {

namespace components {

void ActivityTimerTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE( ActivityTimerTest::testdistanceTimer));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ActivityTimerTest");
}

void ActivityTimerTest::testdistanceTimer() {
	//test random
	boost::shared_ptr<ActivityTimerDistance> act_timer = ActivityTimerDistance::getRandom();
	ASSERT (act_timer->getDelay()>ActivityTimerDistance::MIN_DISTANCE);
	ASSERT (act_timer->getDelay() <ActivityTimerDistance::MAX_DISTANCE);
	ASSERT (act_timer->getDecrement()>ActivityTimerDistance::MIN_DECREMENT_FRACTION *ActivityTimerDistance::MAX_DISTANCE);
	ASSERT (act_timer->getDecrement() <ActivityTimerDistance::MAX_DECREMENT_FRACTION * ActivityTimerDistance::MAX_DISTANCE);

	double pre_delay = act_timer->getDelay();
	//test decrement
	--(*act_timer);
	double post_delay = act_timer->getDelay();

	ASSERT_EQUAL_DELTA(pre_delay-act_timer->getDecrement(), post_delay, 0.00001);
}

}//NAMESPACE

}//NAMESPACE
