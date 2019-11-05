




# Tutorial 2
## Initializaing <void> DDS Manager and subscribing to Assessment data.

---

In order to use the AMM Library, this header must be included.
```
#include <amm_std.h>
```

Create a void template DDS Manager object.
```
AMM::DDSManager<void>* mgr = new AMM::DDSManager<void>("Config/Config.xml");
```

The reason why `<void>` is being used here is due to template specialization for DDS Manager.\
DDS Manager is designed to accept either a <void> template (meaning no type is required), or a template of a single user defined type.
\
More on the user defined type in Tutorial 3.

Initialize Assessment to start using it.
```
mgr->InitializeAssessment();
```

> **NOTE:**\
See Tutorial 1 for all the usable AMM types, or the AMM Specification Docs for a complete list.\
https://github.com/AdvancedModularManikin/specification


Once Assessment is initialize, create the subscriber for it.

For creating subscribers, a reference to the function the user wants to use for callbacks is required to be passed in. This allows a module to receive Assessment data when it is published on the AMM DDS network.
```
// Defined in global scope.
void OnAssessmentEvent (AMM::Assessment& assessment, eprosima::fastrtps::SampleInfo_t* info) {
   std::cout << "Assessment received!" << std::endl;
}
```

Now, call Create Assessment Subscriber passing a ref to the function.
```
mgr->CreateAssessmentSubscriber(&OnAssessmentEvent);
```

This is where template specialization matters. Since `<void>` was used for DDS Manager, it's expecting references to void functions in global scope. These user-defined functions used for subscription event callbacks must retain a specific signature to be accepted as a valid funtion reference to be passed in.

The first argument can be any of the 17 AMM data types depending on the type that has been initialized.
Passing a function ref where is first argument is defined as `AMM::EventFragment`, when the method is
aimed for Assessment, will result is compilation error.

> **NOTE:**\
Required function signature for Assessment.\
`(AMM::Assessment assessment, eprosima::fastrtps::SampleInfo_t* info)`

The second argument is always `SampleInfo_t*`.

Now run the source code for yourself!\
https://github.com/AdvancedModularManikin/example-module/blob/master/Source/Tutorial_2.cpp

### END TUTORIAL 2

Move on to Tutorial 3.\
https://github.com/AdvancedModularManikin/example-module/blob/master/Documents/Tutorial_3.md
