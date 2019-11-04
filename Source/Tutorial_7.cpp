
#include <chrono>
#include <iostream>

/// In order to use the AMM Library, this header must be included.
#include <amm_std.h>

namespace T7 {

/// Tutorial 7 -- Builing an AMM compliant module
/// This tutorial will show the minimum, recommended requirements necessary to construct
/// an AMM module that is spec compliant.
///
/// Please scroll down to Tutorial_7 to begin.
///
/// NOTE:
/// Everything associated in this namespace is part of this compliancy example.





/// Data representing this module's state.
struct ModuleState {

   /// Module Configuration.
   AMM::ModuleConfiguration mc;

   /// Status of each capability this module has.
   /// Subject to change during simulation.
   /// This module only has one capability.
   AMM::Status fooStatus;

   /// ID of the current educational encounter running on the current sim.
   /// This should always mirror what educational encounter is on Module Configuration.
   /// This module should NOT be setting this field.
   /// Module Manager sets this field when Module Configuration data is received.
   AMM::UUID educationalEncoutner;

   /// Number of ticks this module recieved from Sim Manager
   /// while the sim is running.
   int tickCount = 0;
};

/// DDSManager for this module.
AMM::DDSManager<void>* mgr;

/// This module's ID.
/// Gets published at start with Operational Description.
AMM::UUID moduleId;

/// This module's operational description.
/// Static data initialized and published at start.
AMM::OperationalDescription od;

/// The current state of this module.
/// Data that is changed as the sim progresses.
ModuleState currentState;

/// The default state of this module.
/// Data that is cached at this module's start, then loaded
/// as the currentState when Sim Control RESET is called.
ModuleState defaultState;

/// Control flag for running this module with the active sim.
bool isSimRunning = false;

void OnNewSimulationControl (AMM::SimulationControl& simControl, eprosima::fastrtps::SampleInfo_t* info) {

   /// AMM modules are required to act accordingly to the data subscribed to in this receiver.

   switch (simControl.type()) {

   case AMM::ControlType::RUN :

      std::cout << "Sim RUN." << std::endl;

      /// Set this flag true and allow regular updates to occur.
      isSimRunning = true;
      break;

   case AMM::ControlType::HALT :

      std::cout << "Sim HALT." << std::endl;

      /// Set this flag false and not allow updates that involve patient
      /// action or movement other than render status for current or initial sim state.
      isSimRunning = false;
      break;

   case AMM::ControlType::RESET :

      std::cout << "Sim RESET." << std::endl;

      /// On RESET, modules also become HALTED and reset their state to their startup defaults.
      isSimRunning = false;
      currentState = defaultState;
      break;

   case AMM::ControlType::SAVE :

      std::cout << "Sim set to SAVE.n" << std::endl;

      /// On SAVE, modules publish their current Module Configuration so the Module Monager
      /// on the network may cache their data as a save state for future use.
      mgr->WriteModuleConfiguration(currentState.mc);
      break;
   }
}

void OnNewModuleConfiguration (AMM::ModuleConfiguration& modConfig, eprosima::fastrtps::SampleInfo_t* info) {

   std::cout << "Module config received." << std::endl;

   /// Only acknowledge this event if the incoming ID matches this module.
   if (moduleId.id() != modConfig.module_id().id()){
      std::cout << "Module config not for this module." << std::endl;
      return;
   }

   std::cout << "Changing this module's config." << std::endl;

   /// Enter a halted state.
   /// According to module behaviour requirements, all modules must enter a halted state
   /// when new Module Configuration data is published.
   isSimRunning = false;

   currentState.mc = modConfig;
}

/// Receiver for Tick data that advances the simulation one frame forward in time.
/// Also refered to as the AMM Update Loop.
void Update (AMM::Tick& tick, eprosima::fastrtps::SampleInfo_t* info) {

   std::cout << "Tick received!" << std::endl;

   /// Update this module according to what it should be doing.

   /// Do not allow updates if the sim is halted.
   /// This is up to the user's discretion on what should update and what should not.
   /// AMM spec deictates that no manikin activity or movement should update while halted,
   /// but environmental effects may still be rendered to maintain state of the current scenario.
   if (!isSimRunning) return;

   /// Not a module requirement.
   /// Shows that the module is updating something in this example when a tick is received.
   currentState.tickCount++;
   std::cout << "Tick Count: " << currentState.tickCount << std::endl;
}


/// START TUTORIAL HERE.
void Tutorial_7 () {

   /// Timestamp generation.
   using namespace std::chrono;
   auto timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


   /// Every module has a required set of static fields it must maintain to be AMM compliant.
   /// These are: Module ID, DDS Manager, Operational Description, Module Configuration, a Status
   /// for each capability, and Educational Encounter.
   ///
   /// After which, there are a few topics that modules are required to publish and subscribe to
   /// be AMM compliant.
   /// Those topics are Module Configuration, Operational Description, Simulation Control, and Status.

   /// This example contains an optional data structure, Module State, that simply groups these fields
   /// together that are subject to change. The struct itself isn't required, but it's fields are.
   /// When a RESET is called on Simulation Control, this struct makes setting default values convenient.


   /// MODULE ID
   /// Every module has a Module ID that is generated at the module's inception and remains static for
   /// the duration of the module's life span. This module's ID is used as the Module ID field in topic
   /// types that require one.
   moduleId.id(AMM::DDSManager<void>::GenerateUuidString());


   /// DDS MANAGER
   /// This is basically what makes something an AMM module.
   mgr = new AMM::DDSManager<void>("Config/Config.xml");

   /// Once a module has a live DDS Manager, it now needs to fill out some description data
   /// about itself. Two topic types that are required at the module's inception are
   /// Operational Description and Module Configuration .


   /// OPERATIONAL DESCRIPTION
   /// Describes what this module does and annouces it to other modules on the AMM network.
   /// Operational Description values are initialized at the module's inception and remain static
   /// during this module's life time.
   mgr->InitializeOperationalDescription();

   /// Modules only need to publish Operational Description ocne, and only once after
   /// a module has been fully initialized and ready to receive sim data.
   mgr->CreateOperationalDescriptionPublisher();

   /// Initializing Operational Description fields.
   od.name("Example Module - T7");
   od.description("An example module for demo purposes.");
   od.manufacturer("VCOM3D");
   od.serial_number("0000");
   od.module_version("1.0.0");
   od.capabilities_schema(AMM::Utility::read_file_to_string("Config/CapabilitiesSchema.xml"));


   /// MODULE CONFIGURATION
   /// How this module, and every other module is/are configured.
   /// Modules initialize their original config via config xml files that are loaded by
   /// the DDS Manager, and need to keep track of their configuration during the module's
   /// life span.
   mgr->InitializeModuleConfiguration();

   /// Note that the values in Module Configuration are subject to change during simulation.
   /// Not only is every module required to publish Module Configuration at their start, must
   /// also subscribe to this topic too and act accordingly when the received data contains a
   /// module id that matches this module's id.
   mgr->CreateModuleConfigurationPublisher();

   /// Modules also need to subscribe to Module Configuration, because every module is subject
   /// to state change according to the Module Manager.
   /// GOTO: On New Module Configuration.
   mgr->CreateModuleConfigurationSubscriber(&OnNewModuleConfiguration);

   /// Initializing Module Configuration fields.
   /// Note that the Module Configuration representing this module is in the Module State struct.
   currentState.mc.name("Example Module - T7");
   currentState.mc.module_id(moduleId);
   currentState.mc.timestamp(timestamp);
   currentState.mc.capabilities_configuration(
      AMM::Utility::read_file_to_string("Config/CapabilitiesConfiguration.xml")
   );


   /// SIMULATION CONTROL
   /// This is another topic type modules are required to subscribe to.
   /// No module should be publishing Simulation Control unless it is a designated point of control
   /// for the user.
   mgr->InitializeSimulationControl();

   /// EVERY module MUST subscribe to Simulation Control.
   /// This dictates when a module should be executing its AMM update loop.
   /// GOTO: On New Simulation Control.
   mgr->CreateSimulationControlSubscriber(&OnNewSimulationControl);


   /// The final topic type requirement for a module to be AMM compliant is Status.
   /// Each module maintains a Status for each capability it has.
   /// This means a module will have multiple Status objects that are published when created
   /// or when the values in the fields are changed.
   /// The timestamp field MUST be updated to current time when a change to a Status is being published.
   mgr->InitializeStatus();
   mgr->CreateStatusPublisher();

   /// Initializing Status fields.
   currentState.fooStatus.module_id(moduleId);
   currentState.fooStatus.module_name("Example Module");
   currentState.fooStatus.capability("Foo");
   currentState.fooStatus.timestamp(timestamp);
   currentState.fooStatus.value(AMM::StatusValue::OPERATIONAL);
   currentState.fooStatus.message("Ready");


   /// Another recommened topic type to subscribe to is Tick.
   /// Though this type is not part of the AMM spec, it is the main driver
   /// for how the simulation is able to advance forward in time.
   ///
   /// Subscribe to this topic if your module is dependent on updating
   /// when the sim as a whole updates.
   mgr->InitializeTick();
   mgr->CreateTickSubscriber(&Update);


   /// Once the module is initialized with its defaults, cache the current
   /// state so when a RESET is called, all the properites on the module can
   /// be reverted.
   defaultState = currentState;

   /// NOTE:
   /// Operational Description is not being cached, because it should remain static and unchanged
   /// after initialization for the remainder of the module's lifespan.

   /// Need a pause to allow publishers to finish initializing.
   std::this_thread::sleep_for(std::chrono::milliseconds(250));

   /// Write out Operational Description, Module Configuration, and the Status for each capability.
   mgr->WriteOperationalDescription(od);
   mgr->WriteModuleConfiguration(currentState.mc);
   mgr->WriteStatus(currentState.fooStatus);



   bool isRunning = true;
   auto loop = [&]() {
      /// Module's main execution loop.
      /// Non-AMM specific updating logic is done here according to user specification.
      /// Loop ends when module sets the flag. Then DDS Manager shuts down and the program exits.
      /// Logic that alters AMM data should NOT go here. Use a Tick subscriber as the update loop.
      for (;;) {
         if (!isRunning) break;
      }
   };
   std::thread t(loop);
   std::cout << "Listening for data... Press return to exit." << std::endl;
   std::cin.get();
   isRunning = false;
   t.join();


   mgr->Shutdown();

   /// Not completely necessary.
   /// This ensures that any lingering internal FastRTPS async operations are done
   /// before deleting DDS Manager.
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   delete mgr;

   /// Before executing this turorial, be sure to run 2 instances of Command Line Module and run
   /// one of them in listening mode to see the data published from this module, and to write data
   /// to this module.

   /// END TUTORIAL 7

} // Tutorial_7

} // namespace T7
