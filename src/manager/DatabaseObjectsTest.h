/*
 * DatabaseObjectsTest.h
 *
 *  Created on: 15 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DATABASEOBJECTSTEST_H_
#define DATABASEOBJECTSTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace manager {

class DatabaseObjectsTest: public ICuteSuite {
public:
	DatabaseObjectsTest() {
	}
	virtual ~DatabaseObjectsTest() {
	}

	static void runSuite();
	static void testCreateInsertNode();
	static void testCreateInsertConnection();
	static void testCreateInsertPattern();
	static void testCreateConnectionFromDatabaseEntry();
	static void testCreateNodeFromDatabaseEntry();
	static void testCreatePatternFromDatabaseEntry();

};

}

}

#endif /* DATABASEOBJECTSTEST_H_ */
