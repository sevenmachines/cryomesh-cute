/**
 * ImpulseCollectionTest.h
 *
 *  Created on: 25 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef IMPULSECOLLECTIONTEST_H_
#define IMPULSECOLLECTIONTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace components {

/**
 *  Collection of tests for ImpulseCollection
 */
class ImpulseCollectionTest {
public:

/**
 * Run all tests
 */
	static void runSuite();

	/**
	 * Test adding and removing impulses
	 */
	static void testAddRemoveImpulses();

	/**
	 * Test clearing functions
	 */
	static void testClear();

	/**
	 * Test activity calculation
	 */
	static void testGetActivity();

	/**
	 * Test operators
	 */
	static void testOperators() ;

	/**
	 * test clear active cycles
	 */
	static void testClearActiveCycles();

	/**
	 * Test data logging with data objects
	 */
	static void testDataObjects();

};

}

}

#endif /* IMPULSECOLLECTIONTEST_H_ */
