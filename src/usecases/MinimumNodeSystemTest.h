/*
 * OneNodeSystemTest.h
 *
 *  Created on: 21 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef MINIMUMNODESYSTEMTEST_H_
#define MINIMUMNODESYSTEMTEST_H_

#include "ICuteSuite.h"
#include "components/Node.h"
#include "components/Connection.h"

using namespace cryomesh::components;

namespace cryomesh {

namespace usecases {

class MinimumNodeSystemTest {
public:
	MinimumNodeSystemTest(){}
	virtual ~MinimumNodeSystemTest(){}
	static void runSuite();
	static void testMinimumNodeSystem();

	static bool checkNodeHasImpulses(const boost::shared_ptr< Node > & node);
	static bool checkNodePassesImpulses(const boost::shared_ptr< Node > & node);
	static bool checkConnectionHasImpulses(const boost::shared_ptr< Connection > & connection);
	static bool checkConnectionPassesImpulses(const boost::shared_ptr< Connection > & connection);

	//helpers
	static void setAllConnectionsDebug(std::map<boost::uuids::uuid, boost::shared_ptr< Connection > > & cons, bool debug);
	static void setAllInputConnectionsDebug(boost::shared_ptr< Node > node, bool debug) ;
	static void setAllOutputConnectionsDebug(boost::shared_ptr< Node > node, bool debug) ;

	static void printAllNodes(std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & nodes);
};

}

}

#endif /* MINIMUMNODESYSTEMTEST_H_ */
