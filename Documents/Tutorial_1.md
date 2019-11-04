


# TUTORIAL 1
## Initializing DDS Manager and publishing Assessment data.

---

In order to use the AMM Library, this header must be included.  
```
#include <amm_std.h>
```

There are two template verisons of DDS Manager the user can instantiate.
This is the default template of DDS Manager to use.
```
AMM::DDSManager<void>* mgr = new AMM::DDSManager<void>("Config/Config.xml");
```
The argument that DDS Manager takes is the path to the config file that every module must load on start. There should be a config directory in the same location as the executable.

Note that <void> must be included in the template arguments to instantiate DDS Manager.\

> **NOTE:**\
Why does DDS Manager use this kind of template specialization?\
See Tutorial 3.

When a DDS Manager object is created, it creates a Participant for itself on the DDS network under a single DOMAIN ID reserved for AMM.

> **NOTE:**\
Multiple instances of DDS Manager can be created in one program.\
Only one Participant wil be create for a DDS Manager, and each Participant is unique to that DDS Manager object.

Once an instance of DDS Manager is created, the user now has access to all 17 AMM data types:
* Assessment
* Event Fragment
* Event Record
* Fragment Amendment Request
* Log
* Module Configuration
* Omitted Event
* Operational Description
* Physiology Modification
* Physiology Value
* Physiology Waveform
* Render Modification
* Simulation Control
* Status
* Tick
* Instrument Data
* Command

In order to start using a type, the user must initialize which one they're going to use.\
This initializes the AMM Assessment type.
```
mgr->InitializeAssessment();
```

When a type is initialized, DDS Manager takes initiative to register that type on the DDS network so this module can start receiving related data, and/or publishing it.

Once a type has been initialized, the user may now tell DDS Manager to create a data publisher to write data to the DDS network.
```
mgr->CreateAssessmentPublisher();
```

Now that a publisher is created for the Assessment type, the user can now start writing Assessment data.\
Data for Assessment must be generated first.
```
AMM::Assessment assessment;
```

Assessment has these following properties.
Example getting values.
```
auto id      = assessment.id();
auto eventId = assessment.event_id();
auto value   = assessment.value();
auto comment = assessment.comment();
```
These properties return the value when no params are used, and they set the value when a param is used.

> **NOTE:**\
Please see AMM Specification Docs to get a complete break down of AMM types, and other sub data types that represent these properties.\
https://github.com/AdvancedModularManikin/specification

Example setting values.
```
assessment.value(AMM::AssessmentValue::SUCCESS);
assessment.comment("Default comment.");
```

> **NOTE:**\
Please see AMM Specification Docs to get a complete break down of AMM Assessment Values.\
https://github.com/AdvancedModularManikin/specification

These are UUID subtypes, which is a string, that must be generated first.
```
AMM::UUID uuid;
uuid.id("00000000-0000-0000-000000000000");

assessment.id(uuid);
assessment.event_id(uuid);
```

> **ATTENTION:**\
There needs to be a brief delay in execution before calling Write after creating a Publisher.
This gives time for FastRTPS to properly initialize all the data writers and listeners it uses in the background.
\
\
Calling write immediately after creating a Publisher will result in a silent error where no data is written, if the FastRTPS framework isn't ready to write data.
\
\
200 ms is the lowest allowable delay.

```
std::this_thread::sleep_for(std::chrono::milliseconds(200));
```


Once publishable data is generated, it can about be written out to the DDS network.
```
mgr->WriteAssessment(assessment);
```

Now run the source code for yourself!\
_insert github link_

### END TUTORIAL 1

Move on to Tutorial 2.\
_insert github link_
