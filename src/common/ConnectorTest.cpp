/*
 * ConnectorTest.cpp
 *
 *  Created on: 19 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "ConnectorTest.h"

using namespace cryomesh::components;

namespace cryomesh {

namespace common {

ConnectorTest::ConnectorTest() {

}

ConnectorTest::~ConnectorTest() {
}

void ConnectorTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ConnectorTest::testConnectDisconnect));
	s.push_back(CUTE(ConnectorTest::testListConnectDisconnect));
	s.push_back(CUTE(ConnectorTest::testPropagation));
	s.push_back(CUTE(ConnectorTest::testMaximums));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ConnectorTest");
}

void ConnectorTest::testConnectDisconnect() {
	boost::shared_ptr<Node> pnode1(new Node());
	boost::shared_ptr<Node> pnode2(new Node());
	boost::shared_ptr<Node> pnode3(new Node());

	boost::shared_ptr<Connection> pcon1(new Connection());
	boost::shared_ptr<Connection> pcon2(new Connection());
	boost::shared_ptr<Connection> pcon3(new Connection());

	// connect inputs
	{
		pnode1->getMutableConnector().connectInput(pcon1);
		pnode1->getMutableConnector().connectInput(pcon2);
		pnode1->getMutableConnector().connectInput(pcon3);

		pcon1->getMutableConnector().connectOutput(pnode1);
		pcon2->getMutableConnector().connectOutput(pnode1);
		pcon3->getMutableConnector().connectOutput(pnode1);

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();
		unsigned int out_count2 = pcon2->getConnector().getOutputs().size();
		unsigned int out_count3 = pcon3->getConnector().getOutputs().size();

		ASSERT_EQUAL(3, in_count);
		ASSERT_EQUAL(1, out_count1);
		ASSERT_EQUAL(1, out_count2);
		ASSERT_EQUAL(1, out_count3);
	}
	// connect outputs
	{
		pnode2->getMutableConnector().connectOutput(pcon1);
		pnode2->getMutableConnector().connectOutput(pcon2);
		pnode2->getMutableConnector().connectOutput(pcon3);

		pcon1->getMutableConnector().connectInput(pnode2);
		pcon2->getMutableConnector().connectInput(pnode2);
		pcon3->getMutableConnector().connectInput(pnode2);

		unsigned int out_count = pnode2->getConnector().getOutputs().size();
		unsigned int in_count1 = pcon1->getConnector().getInputs().size();
		unsigned int in_count2 = pcon2->getConnector().getInputs().size();
		unsigned int in_count3 = pcon3->getConnector().getInputs().size();

		ASSERT_EQUAL(3, out_count);
		ASSERT_EQUAL(1, in_count1);
		ASSERT_EQUAL(1, in_count2);
		ASSERT_EQUAL(1, in_count3);
	}
	//disconnect inputs
	{
		pnode2->getMutableConnector().disconnectOutput(pcon1);
		{
			unsigned int temp_out_count = pnode2->getConnector().getOutputs().size();
			ASSERT_EQUAL(2, temp_out_count);

		}
		pnode2->getMutableConnector().disconnectOutput(pcon2);
		{
			unsigned int temp_out_count = pnode2->getConnector().getOutputs().size();
			ASSERT_EQUAL(1, temp_out_count);

		}
		pnode2->getMutableConnector().disconnectOutput(pcon3);
		{
			unsigned int temp_out_count = pnode2->getConnector().getOutputs().size();
			ASSERT_EQUAL(0, temp_out_count);

		}
		pnode2->getMutableConnector().disconnectOutput(pcon3);
		{
			unsigned int temp_out_count = pnode2->getConnector().getOutputs().size();
			ASSERT_EQUAL(0, temp_out_count);

		}
		pcon1->getMutableConnector().disconnectInput(pnode2);
		{
			unsigned int temp_out_count = pcon1->getConnector().getInputs().size();
			ASSERT_EQUAL(0, temp_out_count);

		}
		pcon2->getMutableConnector().disconnectInput(pnode2);
		{
			unsigned int temp_out_count = pcon1->getConnector().getInputs().size();
			ASSERT_EQUAL(0, temp_out_count);

		}
		pcon3->getMutableConnector().disconnectInput(pnode2);
		{
			unsigned int temp_out_count = pcon3->getConnector().getInputs().size();
			ASSERT_EQUAL(0, temp_out_count);

		}
	}
}

void ConnectorTest::testListConnectDisconnect() {
	boost::shared_ptr<Node> pnode1(new Node());
	boost::shared_ptr<Node> pnode2(new Node());
	boost::shared_ptr<Node> pnode3(new Node());
	boost::shared_ptr<Node> pnode4(new Node());
	boost::uuids::uuid pnode1_uuid = pnode1->getUUID();
	boost::uuids::uuid pnode2_uuid = pnode2->getUUID();

	boost::shared_ptr<Connection> pcon1(new Connection());
	boost::shared_ptr<Connection> pcon2(new Connection());
	boost::shared_ptr<Connection> pcon3(new Connection());
	boost::shared_ptr<Connection> pcon4(new Connection());
	boost::uuids::uuid pcon1_uuid = pcon1->getUUID();
	boost::uuids::uuid pcon2_uuid = pcon2->getUUID();

	// connect inputs by vector pointer
	{
		pnode1->getMutableConnector().connectInputs(std::vector<boost::shared_ptr<Connection> >( { pcon1, pcon2 }));

		pcon1->getMutableConnector().connectOutputs(std::vector<boost::shared_ptr<Node> >( { pnode1, pnode2 }));

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(2, in_count);
		ASSERT_EQUAL(1, out_count1); //connections now has max 1 ins/outs
	}
	// disconnect inputs by vector pointer
	{
		pnode1->getMutableConnector().disconnectInputs(std::vector<boost::shared_ptr<Connection> >( { pcon1, pcon2 }));
		pcon1->getMutableConnector().disconnectOutputs(std::vector<boost::shared_ptr<Node> >( { pnode1, pnode2 }));

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(0, in_count);
		ASSERT_EQUAL(0, out_count1);
	}

	//connect inputs by initialiser list
	{
		pnode1->getMutableConnector().connectInputs( { pcon1, pcon2 });
		pcon1->getMutableConnector().connectOutputs( { pnode1, pnode2 });

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(2, in_count);
		ASSERT_EQUAL(1, out_count1);//connections now has max 1 ins/outs
	}

	//disconnect inputs by initialiser list
	{
		pnode1->getMutableConnector().disconnectInputs( { pcon1, pcon2 });
		pcon1->getMutableConnector().disconnectOutputs( { pnode1, pnode2 });

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(0, in_count);
		ASSERT_EQUAL(0, out_count1);
	}

	//disconnect inputs by vector list of uuids
	{
		pnode1->getMutableConnector().connectInputs( { pcon1, pcon2 });
		pcon1->getMutableConnector().connectOutputs( { pnode1, pnode2 });

		pnode1->getMutableConnector().disconnectInputs( { pcon1_uuid, pcon2_uuid });
		pcon1->getMutableConnector().disconnectOutputs( { pnode1_uuid, pnode2_uuid });

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(0, in_count);
		ASSERT_EQUAL(0, out_count1);
	}

	//disconnect inputs by initialiser list of uuids
	{
		pnode1->getMutableConnector().connectInputs( { pcon1, pcon2 });
		pcon1->getMutableConnector().connectOutputs( { pnode1, pnode2 });

		pnode1->getMutableConnector().disconnectInputs( { pcon1_uuid, pcon2_uuid });
		pcon1->getMutableConnector().disconnectOutputs( { pnode1_uuid, pnode2_uuid });

		unsigned int in_count = pnode1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(0, in_count);
		ASSERT_EQUAL(0, out_count1);
	}

}

void ConnectorTest::testPropagation() {
	ASSERTM("TODO", false);
}

void ConnectorTest::testMaximums() {
	boost::shared_ptr<Node> pnode1(new Node());
	boost::shared_ptr<Node> pnode2(new Node());
	boost::shared_ptr<Node> pnode3(new Node());
	boost::shared_ptr<Node> pnode4(new Node());
	boost::shared_ptr<Connection> pcon1(new Connection());
	boost::shared_ptr<Connection> pcon2(new Connection());
	boost::shared_ptr<Connection> pcon3(new Connection());
	boost::shared_ptr<Connection> pcon4(new Connection());

	// connect inputs by vector pointer
	{
		pcon1->getMutableConnector().connectInputs(std::vector<boost::shared_ptr<Node> >( { pnode1, pnode2 }));

		pcon1->getMutableConnector().connectOutputs(std::vector<boost::shared_ptr<Node> >( { pnode3, pnode4 }));

		unsigned int in_count = pcon1->getConnector().getInputs().size();
		unsigned int out_count1 = pcon1->getConnector().getOutputs().size();

		ASSERT_EQUAL(1, in_count);
		ASSERT_EQUAL(1, out_count1);
	}
}
} //namespace

} //namespace
