/*
 * ConnectorTest.h
 *
 *  Created on: 19 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef CONNECTORTEST_H_
#define CONNECTORTEST_H_

#include "ICuteSuite.h"
#include "common/Connector.h"
#include "components/Connection.h"
#include "components/Node.h"

namespace cryomesh {

namespace common {

class ConnectorTest: public ICuteSuite {
public:
	ConnectorTest();
	virtual ~ConnectorTest();
	static void runSuite();

	/**
	 * Test connecting and disconnecting
	 */
	static void testConnectDisconnect();

	/**
	 * Test connecting and disconnecting by lists
	 */
	static void testListConnectDisconnect();

	/**
	 * Test propagtion of impulses along connection
	 */
	static void testPropagation();

	/**
	 * Test maximium connections
	 */
	static void testMaximums();
};

}

}

#endif /* CONNECTORTEST_H_ */
