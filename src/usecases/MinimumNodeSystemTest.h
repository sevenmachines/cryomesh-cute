/*
 * OneNodeSystemTest.h
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef MINIMUMNODESYSTEMTEST_H_
#define MINIMUMNODESYSTEMTEST_H_

#include "ICuteSuite.h"
namespace cryomesh {

namespace usecases {

class MinimumNodeSystemTest {
public:
	MinimumNodeSystemTest(){}
	virtual ~MinimumNodeSystemTest(){}
	static void runSuite();
	static void testMinimumNodeSystem();
};

}

}

#endif /* MINIMUMNODESYSTEMTEST_H_ */
