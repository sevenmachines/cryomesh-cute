/*
 * CycleTest.h
 *
 *  Created on: 16 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CYCLETEST_H_
#define CYCLETEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace common {

class CycleTest :public ICuteSuite {
public:
	CycleTest(){}
	virtual ~CycleTest(){}
	static void runSuite();
	static void testOperators();

};

}

}

#endif /* CYCLETEST_H_ */
