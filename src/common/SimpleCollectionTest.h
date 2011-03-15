/**
 * SimpleCollectionTest.h
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef SIMPLECOLLECTIONTEST_H_
#define SIMPLECOLLECTIONTEST_H_

#include "ICuteSuite.h"

/**
 *  Tests for SimpleCollection
 */
namespace cryomesh {

namespace common {

class SimpleCollectionTest :public ICuteSuite {
public:

	/**
	 * Destructor
	 */
	virtual ~SimpleCollectionTest();

	/**
	 * Run all tests
	 */
	static void runSuite();

	/**
	 * Test adding and removing elements
	 */
	static void testAddRemove();

	/**
	 * Test getting sum of all elements
	 */
	static void testSum();
	/**
	 * Test getting maximum and minimum values
	 */
	static void testMaxMin();
	/**
	 * Test getting value by index
	 */
	static void testGetValueAt();

	/**
	 * test inverting and reversing
	 */
	static void testInverseReverse();

	/**
		 * Test all operators
		 */
	static void testOperators() ;

	/**
	 * Test padding collections
	 */
	static void testPad();

	/**
	 * Test adding removing objects by initialiser lists and collection
	 */
	static void testAddRemoveList();

protected:

	/**
	 * Protected contructor to prevent initialisation
	 */
	SimpleCollectionTest();
};

}

}

#endif /* SIMPLECOLLECTIONTEST_H_ */
