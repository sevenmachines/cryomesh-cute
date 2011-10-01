/*
 * ClusterTest.cpp
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#include "ClusterTest.h"
#include "structures/Cluster.h"
#include "manipulators/ClusterArchitect.h"
#include "components/Node.h"
#include <map>
#include <boost/uuid/uuid.hpp>

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
	cluster.getClusterArchitect()->createRandomNodes(10);
	int postsz = cluster.getNodes().size();
	ASSERT_EQUAL(presz+10, postsz);

}

void ClusterTest::testCreateConnections() {

	// test small addition to a large bulk
	{
		structures::Cluster cluster;

		// set up base cluster
		{
			const int CON_COUNT = 2;
			const int NODE_COUNT = 100;
			cluster.getClusterArchitect()->createRandomNodes(NODE_COUNT, CON_COUNT);

		}

		// make an addition to bulk
		{
			const int CON_COUNT = 2;
			const int NODE_COUNT = 2;
			std::list<boost::shared_ptr<components::Node> > new_nodes =
					cluster.getClusterArchitect()->createRandomNodes(NODE_COUNT, CON_COUNT);

			// forall in new_nodes
			{
				int count = 0;
				std::list<boost::shared_ptr<components::Node> >::const_iterator it_new_nodes = new_nodes.begin();
				const std::list<boost::shared_ptr<components::Node> >::const_iterator it_new_nodes_end =
						new_nodes.end();
				while (it_new_nodes != it_new_nodes_end) {
					int insz = (*it_new_nodes)->getConnector().getInputs().size();
					int outsz = (*it_new_nodes)->getConnector().getOutputs().size();

					// should be exact since we are connecting before addition of new nodes to the bulk
					ASSERT(insz==CON_COUNT);
					ASSERT(outsz==CON_COUNT);
					++count;
					++it_new_nodes;
				}
			}
		}

	}
	// test creation from zero bulk
	{
		structures::Cluster cluster;

		//ASSERT PRE-STATE
		{
			const components::NodeMap & nmap = cluster.getNodeMap();
			int cluster_nodemap_sz = nmap.getSize();
			int all_input_connections_sz = nmap.getAllInputConnections().size();
			int all_output_connections_sz = nmap.getAllOutputConnections().size();
			ASSERT_EQUAL(cluster_nodemap_sz, cluster.getNodeMap().getSize());
			ASSERT_EQUAL(all_input_connections_sz, cluster.getNodeMap().getAllInputConnections().size());
			ASSERT_EQUAL(all_output_connections_sz, cluster.getNodeMap().getAllOutputConnections().size());
		}
		const int CON_COUNT = 2;
		const int NODE_COUNT = 10;
		cluster.getClusterArchitect()->createRandomNodes(NODE_COUNT, CON_COUNT);

		int total_in_count = 0;
		int total_out_count = 0;

		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & allnodes = cluster.getNodes();

		// forall in allnodes
		{
			int count = 0;
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_allnodes =
					allnodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_allnodes_end =
					allnodes.end();
			while (it_allnodes != it_allnodes_end) {
				total_in_count += it_allnodes->second->getConnector().getInputs().size();
				total_out_count += it_allnodes->second->getConnector().getOutputs().size();
				++count;
				++it_allnodes;
			}

		}

		//ASSERT POST-STATE
		{
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
					cluster.getNodeMap().getCollection();
			// forall in all_nodes
			{
				std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes = all_nodes.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
						all_nodes.end();
				while (it_all_nodes != it_all_nodes_end) {
					std::cout << "ClusterArchitect::createRandomNodes: " << "Node: " << it_all_nodes->second->getUUIDString()
							<<" connections: "<< it_all_nodes->second->getConnector().getInputs().size() <<"/"<<  it_all_nodes->second->getConnector().getOutputs().size() << std::endl;
					++it_all_nodes;
				}
			}
			int cluster_nodemap_sz = cluster.getNodeMap().getSize();
			int all_connections_sz = cluster.getNodeMap().getAllConnections().size();
			int all_input_connections_sz = cluster.getNodeMap().getAllInputConnections().size();
			int all_output_connections_sz = cluster.getNodeMap().getAllOutputConnections().size();
			ASSERT_EQUAL(NODE_COUNT, cluster_nodemap_sz);

			// min inputs are if all were connected evenly, we would have NODE_COUNT*CON_COUNT
			// but more likely some with a full in or out count might have to accept a new in or out

			ASSERT_EQUAL( (2*NODE_COUNT * CON_COUNT) , all_connections_sz);
			ASSERT_EQUAL(NODE_COUNT * CON_COUNT, all_input_connections_sz);
			ASSERT_EQUAL( NODE_COUNT * CON_COUNT, all_output_connections_sz);
		}
	}

}
} //NAMESPACE

} //NAMESPACE
