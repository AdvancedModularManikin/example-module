
# Tutorial 7
## Builing an AMM compliant module
This tutorial will show the minimum, recommended requirements necessary to construct an AMM module that is spec compliant.

In order to use the AMM Library, this header must be included.
```
#include <amm_std.h>
```

Every module has a required set of static fields it must maintain to be AMM compliant. These are: **Module ID**, **DDS Manager**, **Operational Description**, **Module Configuration**, a **Status** for each capability, and **Educational Encounter**.

After which, there are a few topics that modules are required to publish and subscribe to
be AMM compliant. Those topics are **Operational Description**, **Module Configuration**, **Simulation Control**, and **Status**. Another optional topic to subscribe to is **Tick** which raises an event whenever the simulation loop is advanced by one frame.

This example contains an optional data structure, **Module State**, that simply groups these fields together that are subject to change. The struct itself isn't required, but it's fields are. When a RESET is called on Simulation Control, this struct makes setting default values convenient.

###### Initializing A Module

The following is the required set of fields a module needs to be AMM compliant.
```
struct ModuleState {

   AMM::ModuleConfiguration mc;

   AMM::Status fooStatus;

   AMM::UUID educationalEncoutner;

   int tickCount = 0;
};

AMM::DDSManager<void>* mgr;

AMM::UUID moduleId;

AMM::OperationalDescription od;

ModuleState currentState;

ModuleState defaultState;

bool isSimRunning = false;
```

Data representing this module's state.\
This structure isn't a requirement, but helps manage other fields that are designed to be reset to a default value mid-simulation.\
`struct ModuleState`

Module Configuration.\
`AMM::ModuleConfiguration mc`

Status of each capability this module has.\
Subject to change during simulation.\
This module only has one capability.\
`AMM::Status fooStatus`

ID of the current educational encounter running on the current sim.\
This should always mirror what educational encounter is on Module Configuration.\
This module should NOT be setting this field.\
Module Manager sets this field when Module Configuration data is received.\
`AMM::UUID educationalEncoutner`

Number of ticks this module recieved from Sim Manager while the sim is running.\
Not a module requirement. Shows that this example module is ticking.\
`int tickCount = 0`

DDS Manager for this module.\
`AMM::DDSManager<void>* mgr`

This module's ID.\
Gets published at start with Operational Description.\
`AMM::UUID moduleId`

This module's operational description.\
Static data initialized and published at start.\
`AMM::OperationalDescription od`

The current state of this module.\
Data that is changed as the sim progresses.\
`ModuleState currentState`

The default state of this module.\
Data that is cached at this module's start, then loaded as the currentState when Sim Control RESET is called.\
`ModuleState defaultState`

Control flag for updating this module with the active sim.\
`bool isSimRunning = false`


###### MODULE ID
Every module has a Module ID that is generated at the module's inception and remains static for the duration of the module's life span. This module's ID is used as the Module ID field in topic types that require one.
```
moduleId.id(AMM::DDSManager<void>::GenerateUuidString());
```

###### DDS MANAGER
DDS Manager for sending and receiving data over the network.
```
mgr = new AMM::DDSManager<void>("Config/Config.xml");
```

Once a module has a live DDS Manager, it now needs to fill out some description data about itself. Two topic types that are required at the module's inception are **Operational Description** and **Module Configuration**.

###### OPERATIONAL DESCRIPTION
Describes what this module does and annouces it to other modules on the AMM network.\
Operational Description values are initialized at the module's inception and remain static during this module's life time.
```
mgr->InitializeOperationalDescription();
```

Modules only need to publish Operational Description once, and only once after a module has been fully initialized and ready to receive sim data.
```
mgr->CreateOperationalDescriptionPublisher();
```

Initializing Operational Description fields.
```
AMM::OperationalDescription od;

od.name("Example Module - T7");
od.description("An example module for demo purposes.");
od.manufacturer("VCOM3D");
od.serial_number("0000");
od.module_version("1.0.0");
od.capabilities_schema(AMM::Utility::read_file_to_string("Config/CapabilitiesSchema.xml"));
```


###### MODULE CONFIGURATION
How this module, and every other module is/are configured.\
Modules initialize their original config via config xml files that are loaded by the DDS Manager, and need to keep track of their configuration during the module's life span.
```
mgr->InitializeModuleConfiguration();
```

Note that the values in Module Configuration are subject to change during simulation.\
Not only is every module required to publish Module Configuration at their start, must also subscribe to this topic too and act accordingly when the received data contains a module id that matches this module's id.
```
mgr->CreateModuleConfigurationPublisher();
```

Initializing Module Configuration fields.\
Note that the Module Configuration representing this module is in the Module State struct.
```
struct ModuleState {
   AMM::ModuleConfiguration mc;
} currentState;

using namespace std::chrono;
auto timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

currentState.mc.name("Example Module - T7");
currentState.mc.module_id(moduleId);
currentState.mc.timestamp(timestamp);
currentState.mc.capabilities_configuration(
   AMM::Utility::read_file_to_string("Config/CapabilitiesConfiguration.xml")
);
```

Modules also need to subscribe to Module Configuration, because every module is subject to state change according to the Module Manager.
```
mgr->CreateModuleConfigurationSubscriber(&OnNewModuleConfiguration);

void OnNewModuleConfiguration (AMM::ModuleConfiguration& modConfig, eprosima::fastrtps::SampleInfo_t* info) {

   std::cout << "Module config received." << std::endl;

   if (moduleId.id() != modConfig.module_id().id()){
      std::cout << "Module config not for this module." << std::endl;
      return;
   }

   isSimRunning = false;

   currentState.mc = modConfig;
}
```
When receiving Module Configuration data, all modules will only commit to action when the incoming data has a module ID that matches this module's ID. If true, enter a halted state, and set the module's configuration to when the incoming data is.




###### SIMULATION CONTROL
This is another topic type modules are required to subscribe to.
No module should be publishing Simulation Control unless it is a designated point of control for the user.
```
mgr->InitializeSimulationControl();
```

EVERY module MUST subscribe to Simulation Control.\
This dictates when a module should be executing its AMM update loop.
```
mgr->CreateSimulationControlSubscriber(&OnNewSimulationControl);
```

AMM modules are required to act accordingly to the data subscribed to in this receiver.
```
void OnNewSimulationControl (AMM::SimulationControl& simControl, eprosima::fastrtps::SampleInfo_t* info) {

   switch (simControl.type()) {

   case AMM::ControlType::RUN :

      std::cout << "Sim RUN." << std::endl;

      // Set this flag true and allow regular updates to occur.

      isSimRunning = true;
      break;

   case AMM::ControlType::HALT :

      std::cout << "Sim HALT." << std::endl;

      // Set this flag false and not allow updates that involve patient
      // action or movement other than render status for current or initial sim state.

      isSimRunning = false;
      break;

   case AMM::ControlType::RESET :

      std::cout << "Sim RESET." << std::endl;

      // On RESET, modules also become HALTED and reset their state to their startup defaults.

      isSimRunning = false;
      currentState = defaultState;
      break;

   case AMM::ControlType::SAVE :

      std::cout << "Sim set to SAVE.n" << std::endl;

      // On SAVE, modules publish their current Module Configuration so the Module Monager
      // on the network may cache their data as a save state for future use.

      mgr->WriteModuleConfiguration(currentState.mc);
      break;
   }
}

```

###### STATUS
The final topic type requirement for a module to be AMM compliant is Status.
Each module maintains a Status for each capability it has.
This means a module will have multiple Status objects that are published when created or when the values in the fields are changed. The timestamp field MUST be updated to current time when a change to a Status is being published.
```
mgr->InitializeStatus();
mgr->CreateStatusPublisher();
```

Initializing Status fields.
```
struct ModuleState {
   AMM::ModuleConfiguration mc;
} currentState;

using namespace std::chrono;
auto timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

currentState.fooStatus.module_id(moduleId);
currentState.fooStatus.module_name("Example Module");
currentState.fooStatus.capability("Foo");
currentState.fooStatus.timestamp(timestamp);
currentState.fooStatus.value(AMM::StatusValue::OPERATIONAL);
currentState.fooStatus.message("Ready");
```

###### TICK
Optional topic type to subscribe to.\
Though this type is not part of the AMM spec, it is the main driver for how the simulation is able to advance forward in time.

Subscribe to this topic if your module is dependent on updating when the sim as a whole updates.
```
mgr->InitializeTick();
mgr->CreateTickSubscriber(&Update);
```

```
void Update (AMM::Tick& tick, eprosima::fastrtps::SampleInfo_t* info) {

   std::cout << "Tick received!" << std::endl;

   // Update this module according to what it should be doing.

   if (!isSimRunning) return;

   // Not a module requirement.
   // Shows that the module is updating something in this example when a tick is received.

   currentState.tickCount++;
   std::cout << "Tick Count: " << currentState.tickCount << std::endl;
}
```
Also refered to as the AMM Update Loop.

Do not allow updates if the sim is halted.
This is up to the user's discretion on what should update and what should not. AMM spec deictates that no manikin activity or movement should update while halted, but environmental effects may still be rendered to maintain state of the current scenario.

###### POST INITIALIZATION
Once the module is initialized with its defaults, cache the current state so when a RESET is called, all the properites on the module can be reverted.
```
struct ModuleState {

   AMM::ModuleConfiguration mc;

   AMM::Status fooStatus;

   AMM::UUID educationalEncoutner;

   int tickCount = 0;
};

ModuleState currentState;
ModuleState defaultState;

defaultState = currentState;
```

> **NOTE:**\
Operational Description is not being cached, because it should remain static and unchanged after initialization for the remainder of the module's lifespan.

Remember to provide a slight pause between activating publishers and writing data to allow publishers to finish initializing.
```
std::this_thread::sleep_for(std::chrono::milliseconds(250));
```

Finally write out **Operational Description**, **Module Configuration**, and the **Status** for each capability.
```
mgr->WriteOperationalDescription(od);
mgr->WriteModuleConfiguration(currentState.mc);
mgr->WriteStatus(currentState.fooStatus);
```

Now run the source code for yourself!\
https://github.com/AdvancedModularManikin/example-module/blob/master/Source/Tutorial_7.cpp

### END TUTORIAL 7
