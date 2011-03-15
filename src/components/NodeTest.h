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

	/**
	 * Test data objects usage
	 */
	static void testDataObject();

	/**
	 * test spacial settings and distance connection calcs
	 */
	static void testSpacialSettings();

	static boost::shared_ptr< Node > getDefaultNode();
};

}
}
#endif /* NODETEST_H_ */
