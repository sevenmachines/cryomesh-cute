/*
 * ConnectionTest.h
 *
 *  Created on: 14 Feb 2011
 *      Author: niall
 */

#ifndef CONNECTIONTEST_H_
#define CONNECTIONTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace components {

class ConnectionTest {
public:
	ConnectionTest(){}
	virtual ~ConnectionTest(){}

	static void runSuite();
	static void testCreationConnection();
	static void testUpdate();
	static void testImpulsePropagation();
};

}

}

#endif /* CONNECTIONTEST_H_ */
