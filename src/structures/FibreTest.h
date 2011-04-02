/*
 * FibreTest.h
 *
 *  Created on: 16 Feb 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef FIBRETEST_H_
#define FIBRETEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace structures {

class FibreTest {
public:
	FibreTest(){}
	virtual ~FibreTest(){}

	static void runSuite();
	static void testCreation();
	static void testTrigger() ;
	static void testGetNodesPattern();
};

}

}

#endif /* FIBRETEST_H_ */
