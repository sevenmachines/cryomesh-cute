/**
 * ImpulseTest.h
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef IMPULSETEST_H_
#define IMPULSETEST_H_

/**
 *  Description
 */
#include "ICuteSuite.h"

namespace cryomesh {

namespace components {

class ImpulseTest: public ICuteSuite {
public:
	ImpulseTest();
	virtual ~ImpulseTest();

	/**
	 * Run all tests
	 */
	static void runSuite();
	/**
	 * Test the creation of activity graphs
	 */
	static void testActivityCreation();

	/**
	 * Test getting activity max and mins
	 */
	static void testActivityMaxMin();

	/**
	 * Test working with active cycles
	 */
	static void testActiveCycles();

	/**
	 * Test all operators
	 */
	static void testOperators() ;

	/**
	 * test isActive
	 */
	static void testIsActive();

	static void testRandom();
};

}

}

#endif /* IMPULSETEST_H_ */
