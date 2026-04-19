.. date: 17/03/2026
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

Introduction
============

This tutorial provides a hands-on introduction to building applications with MARTe2.
It is designed to guide new users from setting up their environment and running their
first application to extending the framework with custom components.

The tutorial follows a progressive approach, where each section introduces new
concepts through practical examples. A simple control problem (a mass-spring-damper) is used
as a common thread to illustrate how different MARTe2 components are assembled into
working applications.

Goals
-----

By the end of this tutorial, you will be able to:

- Configure and deploy MARTe2 in different environments
- Run a basic MARTe2 application
- Understand the core architecture of a MARTe2 system
- Diagnose common configuration and startup errors
- Modify application behaviour through configuration
- Use existing components from MARTe2-components
- Write and integrate a simple GAM
- Understand the role of messages in application control
- Implement a minimal custom DataSource
- Recognise patterns for structuring more complex applications

What You Will Learn
-------------------

This tutorial is organised as a step-by-step path:

- Configure and deploy MARTe2 on:

  - a general-purpose Linux system
  - a CODAC Core System environment
- Start from a minimal working application
- Explore how MARTe2 components interact (GAMs, DataSources, signals)
- Introduce a simple control example (water tank)
- Extend the application using standard components
- Modify behaviour through configuration changes
- Implement custom logic with a GAM
- Use messages to trigger runtime behaviour
- Develop a simple DataSource
- Explore basic decoupling and architectural patterns

Prerequisites
-------------

To follow this tutorial, you should have:

- Basic knowledge of C++
- Familiarity with Linux command line tools
- A general understanding of software development concepts

No prior experience with MARTe2 is required.

How to Use This Tutorial
------------------------

The tutorial is designed to be followed sequentially. It starts with environment
setup and deployment, ensuring that you can build and run MARTe2 applications
before moving on to application design and extension.

You are encouraged to:

- Run all the provided examples
- Modify configurations and observe the effects
- Experiment with small changes to better understand the system

Later sections introduce more advanced topics and can be explored independently
once the basics are understood.