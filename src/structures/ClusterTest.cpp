/*
 * ClusterTest.cpp
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#include "ClusterTest.h"
#include "structures/Cluster.h"

namespace cryomesh {

namespace structures {

void ClusterTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterTest::testCreateConnections));
	s.push_back(CUTE(ClusterTest::testCreateNodes));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterTest");
}

void ClusterTest::testCreateNodes() {
	structures::Cluster cluster;
	int presz = cluster.getNodes().size();
	cluster.createNodes(10);
	int postsz = cluster.getNodes().size();
	ASSERT_EQUAL(presz+10, postsz);

}

void ClusterTest::testCreateConnections() {
	structures::Cluster cluster;
	const int CON_COUNT = 2;
	const int NODE_COUNT = 100;
	cluster.createNodes(NODE_COUNT);
	cluster.createConnectivity(CON_COUNT);

	const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & allnodes = cluster.getNodes();

	// forall in allnodes
	{
		int count = 0;
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_allnodes =
				allnodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_allnodes_end =
				allnodes.end();
		while (it_allnodes != it_allnodes_end) {
			int insz = it_allnodes->second->getConnector().getInputs().size();
			int outsz = it_allnodes->second->getConnector().getOutputs().size();
			//std::cout << "ClusterTest::testCreateConnections: " << "count:" << count << " in:" << insz << " out:"
			//		<< outsz << std::endl;

			// allow for random and self connection
			ASSERT(insz>=CON_COUNT-1);
			ASSERT(insz<=CON_COUNT+1);
			ASSERT(outsz>=CON_COUNT-1);
			ASSERT(outsz<=CON_COUNT+1);
			++count;
			++it_allnodes;
		}
	}
}
}//NAMESPACE

}//NAMESPACE
