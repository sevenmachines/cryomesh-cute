/*
 * ActivityTimerTest.h
 *
 *  Created on: 1 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVITYTIMERTEST_H_
#define ACTIVITYTIMERTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace components {

class ActivityTimerTest : public ICuteSuite{
public:
	ActivityTimerTest(){}
	virtual ~ActivityTimerTest(){}

	static void runSuite();

	static void testdistanceTimer();
};

}

}

#endif /* ACTIVITYTIMERTEST_H_ */
