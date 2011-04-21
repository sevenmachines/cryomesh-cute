/*
 * CycleTests
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CYCLETESTS_H_
#define CYCLETESTS_H_

#include "ICuteSuite.h"
namespace cryomesh {

namespace usecases {

class CycleTests {
public:
	CycleTests(){}
	virtual ~CycleTests(){}
	static void runSuite();
	static void testFullCycle();
};

}

}

#endif /* CYCLETESTS_H_ */
