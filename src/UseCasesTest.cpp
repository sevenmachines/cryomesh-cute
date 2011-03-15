#include "UseCasesTest.h"

#include "structures/Bundle.h"
#include "components/Node.h"
#include "components/Connection.h"

using namespace cryomesh::components;
using namespace cryomesh::structures;

namespace cryomesh {
void UseCasesTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(UseCasesTest::testPropagation));
	s.push_back(CUTE(UseCasesTest::testFullCycle));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "UseCasesTest");
}

void UseCasesTest::testPropagation() {

	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	tk.reset();
	// set up node and connections
	//Connection is
	// NodeA->2ConsA->NodeB->3ConsB->NodeC
	boost::shared_ptr<Node> nodeA(new Node);
	boost::shared_ptr<Node> nodeB(new Node);
	boost::shared_ptr<Node> nodeC(new Node);

	boost::shared_ptr<Connection> con_a1(new Connection);
	boost::shared_ptr<Connection> con_a2(new Connection);
	boost::shared_ptr<Connection> con_b1(new Connection);
	boost::shared_ptr<Connection> con_b2(new Connection);
	boost::shared_ptr<Connection> con_b3(new Connection);

	// Get 3 impulses which will sum greater than activity trigger
	boost::shared_ptr<Impulse> imp1(new Impulse(0.4 * Node::ACTIVITY_THRESHOLD, 8));
	boost::shared_ptr<Impulse> imp2(new Impulse(0.5 * Node::ACTIVITY_THRESHOLD, 4));
	boost::shared_ptr<Impulse> imp3(new Impulse(0.6 * Node::ACTIVITY_THRESHOLD, 2));

	// Reset start times
	{
		const common::Cycle & now = common::TimeKeeper::getTimeKeeper().getCycle();
		imp1->setFirstActiveCycle(now);
		imp2->setFirstActiveCycle(now + 2);
		imp3->setFirstActiveCycle(now + 4);
	}

	// Connect Node A to Node B
	{
		nodeA->getMutableConnector().connectOutput(con_a1);
		nodeA->getMutableConnector().connectOutput(con_a2);
		nodeB->getMutableConnector().connectInputs(std::vector<boost::shared_ptr<Connection> > { con_a1, con_a2 });
		//nodeB->getMutableConnector().connectInput(con_a2);
	}

	// Connect NodeB to NodeC
	{
		nodeB->getMutableConnector().connectOutput(con_b1);
		nodeB->getMutableConnector().connectOutput(con_b2);
		nodeB->getMutableConnector().connectOutput(con_b3);

		nodeC->getMutableConnector().connectInput(con_b1);
		nodeC->getMutableConnector().connectInput(con_b2);
		nodeC->getMutableConnector().connectInput(con_b3);
	}

	// Quick sanity check
	{
		// NodeA
		{
			int insz = nodeA->getConnector().getInputs().size();
			int outsz = nodeA->getConnector().getOutputs().size();
			ASSERT_EQUAL(0, insz);
			ASSERT_EQUAL(2, outsz);
		}

		// NodeB
		{
			int insz = nodeB->getConnector().getInputs().size();
			int outsz = nodeB->getConnector().getOutputs().size();
			ASSERT_EQUAL(2, insz);
			ASSERT_EQUAL(3, outsz);
		}

		// NodeC
		{
			int insz = nodeC->getConnector().getInputs().size();
			int outsz = nodeC->getConnector().getOutputs().size();
			ASSERT_EQUAL(3, insz);
			ASSERT_EQUAL(0, outsz);
		}

	}

	// Apply impulses
	{
		nodeA->addImpulse(imp1);
		nodeA->addImpulse(imp2);
		nodeA->addImpulse(imp3);

	}
	nodeA->update();
	// Sanity check
	{
		ASSERT_EQUAL(3, nodeA->getImpulses().getSize());
		ASSERT(nodeA->getImpulses().getActivity()>0);
	}

	// From [0-2) -> imp1
	// [2, 4) -> imp1 + imp2
	// [4, 6) -> imp1 + imp2 + imp3
	// [6, 8) -> imp1 + imp3
	// [8, 8) -> imp1

	const double DELTA = 0.00001;
	// Propogate with no firing
	{
		for (int i = 0; i < 10; i++) {
		//	std::cout << "UseCasesTest::testPropagation: " << "i: " << i << " imp1:" << imp1->getActivity() << " imp2:"
		//		<< imp2->getActivity() << " imp3:" << imp3->getActivity() << " total:" << imp1->getActivity()
		//			+ imp2->getActivity() + imp3->getActivity() << std::endl;
			if (i < 2) {
				// From [0-2) -> imp1
				double expected = imp1->getActivity();
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);
			} else if (i < 4) {
				// [2, 4) -> imp1 + imp2
				double expected = imp1->getActivity() + imp2->getActivity();
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);

			} else if (i < 7) {
				// [4, 6) -> imp1 + imp2 + imp3
				double expected = imp1->getActivity() + imp2->getActivity() + imp3->getActivity();
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);
			} else if (i < 8) {
				// [6, 8) -> imp1 + imp3
				double expected = imp1->getActivity() + imp3->getActivity();
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);
			} else if (i < 9) {
				// [8, 8] -> imp1
				double expected = imp1->getActivity();
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);
			} else if (i < 10) {
				double expected = 0;
				double actual = nodeA->updateActivity();
				ASSERT_EQUAL_DELTA(expected, actual, DELTA);
			}

			// check for activation
			if (i == 4 || i == 5) {
				double expected = Node::Positive;
				Node::ActivationState actual = nodeA->checkActivationState();
				bool state_equality = (expected == actual);
				ASSERT(state_equality);
			} else {
				double expected = Node::None;
				Node::ActivationState actual = nodeA->checkActivationState();
				bool state_equality = (expected == actual);
				ASSERT(state_equality);
			}
			tk.update();
		}
	}

	// Reset start times
	{
		const common::Cycle & now = common::TimeKeeper::getTimeKeeper().getCycle();
		imp1->setFirstActiveCycle(now);
		imp2->setFirstActiveCycle(now + 2);
		imp3->setFirstActiveCycle(now + 4);
	}

	// Propogate with firing
	{
		for (int i = 0; i < 10; i++) {
			//std::cout << "UseCasesTest::testPropagation: " << "i: " << i << " imp1:"
			//		<< imp1->getActivity() << " imp2:" << imp2->getActivity() << " imp3:" << imp3->getActivity()
			//		<< " total:" << imp1->getActivity() + imp2->getActivity() + imp3->getActivity() << std::endl;
			if (i == 4) {
				double expected = Node::Positive;
				Node::ActivationState actual = nodeA->checkActivationState();
				bool state_equality = (expected == actual);
				ASSERT(state_equality);

				// Do update which will clear impulses since they're all active
			} else {
				double expected = Node::None;
				Node::ActivationState actual = nodeA->checkActivationState();
				bool state_equality = (expected == actual);
				ASSERT(state_equality);
			}
			nodeA->update();
			tk.update();
		}
	}

	// Impulse has been fired into output Connections but we have not propagated themyet
	{
		// test that impulses actually arrived at nodeA's outputs con_a1 and a2
		{
			int sz_a1 = con_a1->getImpulses().getSize();
			int sz_a2 = con_a2->getImpulses().getSize();
			ASSERT_EQUAL(1, sz_a1);
			ASSERT_EQUAL(1, sz_a2);
		}

	}
	ASSERTM("TODO: Actually propagate an impulse", false);
}

void UseCasesTest::testFullCycle() {
	const int FIBRE_WIDTH=10;
	Bundle bundle;
	boost::shared_ptr<Cluster> cluster1 = bundle.createCluster(10,10);
	boost::shared_ptr<Cluster> cluster2 = bundle.createCluster(10, 10);
	boost::shared_ptr<Cluster> cluster3 = bundle.createCluster(10, 10);
	ASSERT_EQUAL(3, bundle.getClusters().getSize());

	boost::shared_ptr<Fibre> fibre1in = bundle.connectPrimaryInputCluster(cluster1->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre12 = bundle.connectCluster(cluster1->getUUID(), cluster2->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre23 = bundle.connectCluster(cluster2->getUUID(), cluster3->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre13 = bundle.connectCluster(cluster1->getUUID(), cluster3->getUUID(), FIBRE_WIDTH);
	boost::shared_ptr<Fibre> fibre3out = bundle.connectPrimaryOutputCluster(cluster3->getUUID(), FIBRE_WIDTH);
	ASSERT_EQUAL(5, bundle.getFibres().getSize());

	ASSERT_EQUAL(Fibre::OutputCluster, fibre1in->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre12->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre12->isConnected(cluster2) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre23->isConnected(cluster2) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre23->isConnected(cluster3) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre13->isConnected(cluster1) );
	ASSERT_EQUAL(Fibre::OutputCluster, fibre13->isConnected(cluster3) );
	ASSERT_EQUAL(Fibre::InputCluster, fibre3out->isConnected(cluster3) );

	// Structure summary
	// pin->cluster1-> cluster2->cluster3->pout
	//						 --------------->
	const unsigned int TOTAL_UPDATES=100;
	for (int i = 0; i< TOTAL_UPDATES; i++) {
	//	std::cout<<"UseCasesTest::testFullCycle: "<<i<<std::endl;
	//	std::cout<<"###################################################"<<std::endl;
		//std::cout<<bundle<<std::endl;
	//	std::cout<<"###################################################"<<std::endl;
		fibre1in->trigger(0.8);
		bundle.update();
	}

	ASSERTM("TODO", false);
}
}//NAMESPACE
