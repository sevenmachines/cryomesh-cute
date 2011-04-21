/*
 * FullSystem.h
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef FULLSYSTEMTESTS_H_
#define FULLSYSTEMTESTS_H_

#include "ICuteSuite.h"
namespace cryomesh {

namespace usecases {

class FullSystemTests {
public:
	FullSystemTests(){}
	virtual ~FullSystemTests(){}
	static void runSuite();
	static void testBasicFullSystem();

};

}

}

#endif /* FULLSYSTEMTESTS_H_ */
