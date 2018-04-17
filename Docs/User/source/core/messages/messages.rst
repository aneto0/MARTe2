.. date: 17/04/2018
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

Messages
========

The MARTe messaging mechanism allows MARTe components to exchange information based on the Objects addresses (source and destination) in the :vcisdoxygencl:`ObjectRegistryDatabase`.

Messages provide a generic interface between MARTe components and any components and protocols that live outside a MARTe application. This allows, for instance, to replace the external component without changing the internal message protocol.

This interface is tipically used for non real-time activities, such as configuration and state management. In particular, messages are the main technology behind the :doc:`StateMachine </core/statemachine/statemachine>` 

.. image:: Messages-0.png

The main actors are the :vcisdoxygencl:`Message` and the :vcisdoxygencl:`MessageI` components. The former defines the payload, the sender and the destination of the message. The latter is the interface that allows to receive messages. 

Message
-------

The :vcisdoxygencl:`Message` requires the definition of the following parameters:

- Sender: the Object that is sending the message;
- Destination: the Object that will the message;
- Function: a string which identifies the scope of the action to be performed in the Destination object;
- IsReply: true if this message is a reply to a previous message;
- ExpectsReply: true if a reply is expected from the Destination component.

.. image:: Messages-1.png

Given that the Message is also a :vcisdoxygencl:`ReferenceContainer` the payload is defined by any References that are inserted into the container.
 
MessageI
--------

In order to be able to receive messages a component shall inherit from :vcisdoxygencl:`MessageI` and shall register one or more filters that will be responsible for deciding if a given message is to be accepted.

Filters shall inherit from :vcisdoxygencl:`MessageFilter` and can be either permanent or temporary. A temporary filter will be removed from the :vcisdoxygencl:`MessageI` after accepting the first message. 

.. code-block:: c++

   class MessageFilterEx1: public MARTe::Object, public MARTe::MessageFilter {
      ...
      //Permanent filter MessageFilter(true) 
      MessageFilterEx1 () : MARTe::Object(), MARTe::MessageFilter(true) {
      }
      
      virtual MARTe::ErrorManagement::ErrorType ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
         //Check if this filter is appropriate to handle the message
      }

.. code-block:: c++

   class MessageEx1: public MARTe::Object, public MARTe::MessageI {
      ... 
      MessageEx1 () : MARTe::Object(), MARTe::MessageI() {
        filter = ReferenceT<MessageFilterEx1>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetOwner(this);
        //Install the filter
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    }


.. image:: Messages-2.png

.. image:: Messages-3.png

A Message is sent by calling the function ``MessageI::SendMessage``.