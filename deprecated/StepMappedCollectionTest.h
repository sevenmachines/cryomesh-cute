/**
 * StepMappedCollectionTest.h
 *
 *  Created on: 9 Feb 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef STEPMAPPEDCOLLECTIONTEST_H_
#define STEPMAPPEDCOLLECTIONTEST_H_

#include "common/StepMappedCollection.h"
#include "common/Cycle.h"
#include "components/Impulse.h"

#include "ICuteSuite.h"

#include <string>

/**
 *  Test cases for StepMappedCollection
 */
namespace cryomesh {

namespace common {

class StepMappedCollectionTest {
public:
	StepMappedCollectionTest() {
	}
	virtual ~StepMappedCollectionTest() {
	}
	static void runSuite();

	/**
	 * Test general collection functionality
	 */
	static void testGeneral();

	/**
	 * Test clearing
	 */
	static void testClear();

	/**
	 * Test ranges
	 */
	static void testGetRange();

	/**
	 * Test Summing
	 */
	static void testGetValue();
};

}

}

#endif /* STEPMAPPEDCOLLECTIONTEST_H_ */
