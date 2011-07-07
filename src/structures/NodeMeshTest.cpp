/*
 * NodeMeshTest.cpp
 *
 *  Created on: 26 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeMeshTest.h"
#include "structures/NodeMesh.h"
#include "ICuteSuite.h"
#include "structures/Cluster.h"

namespace cryomesh {

namespace structures {

void NodeMeshTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(NodeMeshTest::testRegenerateNeighbourhoods));
	s.push_back(CUTE(NodeMeshTest::testRegenerateActivities));
	s.push_back(CUTE(NodeMeshTest::testWarpNodes));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "NodeMeshTest");
}

void NodeMeshTest::testRegenerateNeighbourhoods() {
	const int NODECOUNT = 10;
	const double BOUNDINGBOXMAX = 10;
	const double BOUNDINGBOXMAXRADIUS = std::sqrt(3 * (BOUNDINGBOXMAX * BOUNDINGBOXMAX));
	// set up test neighbourhood
	boost::shared_ptr<Cluster> cluster(new Cluster(NODECOUNT, 1, spacial::Point(10, 10, 10)));
	// make radius large enough so we all have neighbours
	NodeMesh nodemesh(*cluster, 5);
	nodemesh.regenerateNeighbourhoods();

	NodeMesh::NeighbourhoodRanges minmaxes = nodemesh.getNeighbourRanges();
	ASSERT(minmaxes.minimumNeighbourCount>0);
	ASSERT(minmaxes.maximumNeighbourCount<NODECOUNT);
	ASSERT(minmaxes.minimumNeighbourDistance>0);
	ASSERT(minmaxes.maximumNeighbourDistance<BOUNDINGBOXMAXRADIUS);

}

void NodeMeshTest::testRegenerateActivities() {
	// set up cluster
	const int NODECOUNT = 10;
	const double BOUNDINGBOXMAX = 0.5;
	// set up test neighbourhood
	boost::shared_ptr<Cluster> cluster(
			new Cluster(NODECOUNT, 1, spacial::Point(BOUNDINGBOXMAX, BOUNDINGBOXMAX, BOUNDINGBOXMAX)));
	// make radius large enough so we all have neighbours
	NodeMesh nodemesh(*cluster, 5);
	nodemesh.regenerateNeighbourhoods();

	// give all nodes an activity in a range
	cluster->getMutableNodeMap().addRandomImpulses(1);
	common::TimeKeeper::getTimeKeeper().update();
	// regenerate activities from nieghbours
	nodemesh.regenerateActivities();
	// ensure neighbour interpolated activities are sane
	const std::map<boost::shared_ptr<components::Node>, double> & acts = nodemesh.getNeighbourhoodActivities();
	// forall in acts
	{
		std::map<boost::shared_ptr<components::Node>, double>::const_iterator it_acts = acts.begin();
		const std::map<boost::shared_ptr<components::Node>, double>::const_iterator it_acts_end = acts.end();
		while (it_acts != it_acts_end) {
			ASSERT(it_acts->second>0);
			++it_acts;
		}
	}
}

void NodeMeshTest::testWarpNodes() {
	// create cluster, ensure all nodes are close enough to have neighbours
	// set up cluster
	const int NODECOUNT = 10;
	const double BOUNDINGBOXMAX = 0.5;
	// set up test neighbourhood
	boost::shared_ptr<Cluster> cluster(
			new Cluster(NODECOUNT, 1, spacial::Point(BOUNDINGBOXMAX, BOUNDINGBOXMAX, BOUNDINGBOXMAX)));

	// give all nodes an activity in a range
	cluster->getMutableNodeMap().addRandomImpulses(0.5);
	// update cluster includes updating mesh activities
	cluster->update();
	// get nodes and map to current activity
	std::map<boost::uuids::uuid, double> original_node_activities;
	std::map<boost::uuids::uuid, int> original_node_impulse_count;

	common::Cycle this_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	common::Cycle next_cycle = common::TimeKeeper::getTimeKeeper().getCycle() + 1;

	// forall in all_nodes
	{
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
				cluster->getNodeMap().getCollection();
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes =
				all_nodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
				all_nodes.end();
		while (it_all_nodes != it_all_nodes_end) {
			original_node_activities[it_all_nodes->first] = it_all_nodes->second->getActivity(next_cycle);
			original_node_impulse_count[it_all_nodes->first] = it_all_nodes->second->getImpulses().getSize();
			++it_all_nodes;
		}
	}

	// update to next cycle
	common::TimeKeeper::getTimeKeeper().update();
	// check cluster and its copy have same current activities for all nodes
	{
		// forall in all_nodes
		{
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
					cluster->getNodeMap().getCollection();
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes =
					all_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
					all_nodes.end();
			const std::map<boost::uuids::uuid, double>::const_iterator it_found_original_activities_end =
					original_node_activities.end();
			const std::map<boost::uuids::uuid, int>::const_iterator it_original_impulse_count_end =
					original_node_impulse_count.end();

			while (it_all_nodes != it_all_nodes_end) {
				// find orginal activity
				std::map<boost::uuids::uuid, double>::const_iterator it_found_original_activities =
						original_node_activities.find(it_all_nodes->first);
				double original_activity;

				if (it_found_original_activities != it_found_original_activities_end) {
					original_activity = it_found_original_activities->second;
				} else {
					std::cout << "NodeMeshTest::testWarpNodes: " << "ERROR: Original uuid not found" << std::endl;
					assert(false);
				}

				// find orginal impulse count
				std::map<boost::uuids::uuid, int>::const_iterator it_found_original_impulse_count =
						original_node_impulse_count.find(it_all_nodes->first);
				int original_impulse_count;

				if (it_found_original_impulse_count != it_original_impulse_count_end) {
					original_impulse_count = it_found_original_impulse_count->second;
				} else {
					std::cout << "NodeMeshTest::testWarpNodes: " << "ERROR: Original uuid not found in impulse count"
							<< std::endl;
					assert(false);
				}

				double new_activity = it_all_nodes->second->getActivity();
				int dnoteq = common::Maths::compareDoubles(original_activity, new_activity, 0.00001);
				std::cout << "NodeMeshTest::testWarpNodes: pre-warp -> " << "original: " << original_activity
						<< " new_activity: " << new_activity << " delta: " << new_activity - original_activity
						<< std::endl;
				ASSERT_EQUAL(0, dnoteq);

				int new_impulse_count = it_all_nodes->second->getImpulses().getSize();
				//	ASSERT_EQUAL(1 , original_impulse_count);
				//	ASSERT_EQUAL(0 , new_impulse_count);
				++it_all_nodes;

			}
		}
	}

	// warp the clusters nodes using mesh
	cluster->update();
	cluster->warpNodes();

	// check cluster and its copy have same diferent activities for all nodes
	{
		// forall in all_nodes
		{
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
					cluster->getNodeMap().getCollection();
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes =
					all_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
					all_nodes.end();
			const std::map<boost::uuids::uuid, double>::const_iterator it_found_original_activities_end =
					original_node_activities.end();
			const std::map<boost::uuids::uuid, int>::const_iterator it_original_impulse_count_end =
					original_node_impulse_count.end();

			while (it_all_nodes != it_all_nodes_end) {
				// find orginal activity
				std::map<boost::uuids::uuid, double>::const_iterator it_found_original_activities =
						original_node_activities.find(it_all_nodes->first);
				double original_activity;

				if (it_found_original_activities != it_found_original_activities_end) {
					original_activity = it_found_original_activities->second;
				} else {
					std::cout << "NodeMeshTest::testWarpNodes: " << "ERROR: Original uuid not found" << std::endl;
					assert(false);
				}

				// find orginal impulse count
				std::map<boost::uuids::uuid, int>::const_iterator it_found_original_impulse_count =
						original_node_impulse_count.find(it_all_nodes->first);
				int original_impulse_count;

				if (it_found_original_impulse_count != it_original_impulse_count_end) {
					original_impulse_count = it_found_original_impulse_count->second;
				} else {
					std::cout << "NodeMeshTest::testWarpNodes: " << "ERROR: Original uuid not found in impulse count"
							<< std::endl;
					assert(false);
				}

				double new_activity = it_all_nodes->second->getActivity();
				int dnoteq = common::Maths::compareDoubles(original_activity, new_activity, 0.00001);
				std::cout << "NodeMeshTest::testWarpNodes: post-warp -> " << "original: " << original_activity
						<< " new_activity: " << new_activity << " delta: " << new_activity - original_activity
						<< std::endl;
				ASSERT(dnoteq != 0);

				int new_impulse_count = it_all_nodes->second->getImpulses().getSize();
				ASSERT_EQUAL(original_impulse_count +1 , new_impulse_count);
				++it_all_nodes;
			}
		}
	}

	std::map<boost::uuids::uuid, int> temp_node_impulse_count;

	// forall in all_nodes
	{
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
				cluster->getNodeMap().getCollection();
		std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes =
				all_nodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
				all_nodes.end();
		while (it_all_nodes != it_all_nodes_end) {
			temp_node_impulse_count[it_all_nodes->first] = it_all_nodes->second->getImpulses().getSize();
			++it_all_nodes;
		}
	}

	/*
	// warp the clusters nodes using mesh
	cluster->update();

	// check cluster and its copy have same diferent activities for all nodes
	{
		// forall in all_nodes
		{
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> > & all_nodes =
					cluster->getNodeMap().getCollection();
			std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes =
					all_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<components::Node> >::const_iterator it_all_nodes_end =
					all_nodes.end();
			const std::map<boost::uuids::uuid, double>::const_iterator it_found_original_activities_end =
					original_node_activities.end();
			const std::map<boost::uuids::uuid, int>::const_iterator it_temp_node_impulse_count_end =
					temp_node_impulse_count.end();

			while (it_all_nodes != it_all_nodes_end) {

				std::map<boost::uuids::uuid, int>::const_iterator it_found_temp_node_impulse_count =
						temp_node_impulse_count.find(it_all_nodes->first);

				int temp_impulse_count;
				if (it_found_temp_node_impulse_count != it_temp_node_impulse_count_end) {
					temp_impulse_count = it_found_temp_node_impulse_count->second;
				} else {
					std::cout << "NodeMeshTest::testWarpNodes: " << "ERROR: Original uuid not found in impulse count"
							<< std::endl;
					assert(false);
				}
				int new_impulse_count = it_all_nodes->second->getImpulses().getSize();
				ASSERT_EQUAL(temp_impulse_count-1 , new_impulse_count);
				++it_all_nodes;
			}
		}
	}
	*/

}
}//NAMESPACE
}//NAMESPACE
