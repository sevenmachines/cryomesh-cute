/*
 * DatabaseManagerTest.h
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DATABASEMANAGERTEST_H_
#define DATABASEMANAGERTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace manager {

class DatabaseManagerTest {
public:
	DatabaseManagerTest(){}
	virtual ~DatabaseManagerTest(){}

	static void runSuite();
	static void testCreation();
	static void testCommands();
};

}

}

#endif /* DATABASEMANAGERTEST_H_ */
