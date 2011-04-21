/**
 * UseCaseTests.h
 *
 *  Created on: 4 Feb 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef USECASESTEST_H_
#define USECASESTEST_H_

#include "ICuteSuite.h"


namespace cryomesh {

/**
 *  Use case test of propagation from a Node, through a
 *  Connections and ending in a Node again
 */
class UseCasesTest : public ICuteSuite{
public:
	virtual ~UseCasesTest(){}

	/**
	 * Run all tests
	 */
	static void runSuite();

	/**
	 * Test propagation through nodes and connections
	 */
	static void testPropagation();

	/**
	 * Test a full cycle run
	 */
	static void testFullCycle();

	static void testBasicFullSystem();
protected:
	UseCasesTest(){}

};

}

#endif /* USECASESTEST_H_ */
