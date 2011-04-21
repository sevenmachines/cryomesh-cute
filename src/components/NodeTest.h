/*
 * NodeTest.h
 *
 *  Created on: 19 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef NODETEST_H_
#define NODETEST_H_

#include "ICuteSuite.h"
#include "components/Node.h"

namespace cryomesh {

namespace components {

class NodeTest: public ICuteSuite, public Node {
public:
	NodeTest();
	virtual ~NodeTest();
	static void runSuite();
	static void testContruction();

	static void testUpdateImpulses();
	static void testEmitmpulse();
	static void testCheckActivation();
	static void testGetActivities();
	static void testGetSetCurrentActivity();
	static void testAddActivity();
	static void testAddImpulse();
	static void testIsPrimary();
	/**
	 * Test data objects usage
	 */
	static void testDataObject();

	/**
	 * test spacial settings and distance connection calcs
	 */
	static void testSpacialSettings();

	static void testForceFire();

	static boost::shared_ptr<Node> getDefaultNode();
	template<class T>
	static bool checkConnectionCount(const boost::shared_ptr<T> obj, const int expected_in, const int expected_out);
};

}
}
#endif /* NODETEST_H_ */
