
/// For logging purposes.
#include <string>
#include <iostream>

/// For looping purposes.
#include <thread>


/// In order to use the AMM Library, this header must be included.
#include <amm_std.h>

namespace T1 { void Tutorial_1 (); }
namespace T2 { void Tutorial_2 (); }
namespace T3 { void Tutorial_3 (); }
namespace T4 { void Tutorial_4 (); }
namespace T5 { void Tutorial_5 (); }
namespace T6 { void Tutorial_6 (); }
namespace T7 { void Tutorial_7 (); }

int main () {

   /// BUG:
   /// There is currently a known issue where publishers and subscribers fail
   /// to send or received data between tutorials.
   ///
   /// This is because every tutorial ends with DDS Manager :: Shutdown being called
   /// which is causing all future publishers and subscribers from working if another
   /// DDS Manager is created after one is shutdown.
   ///
   /// RECOMMENDATION: Restart the program between each tutorial and only use Shutdown
   /// in custom modules if the program is planning to exit.

   for (;;) {

      std::cout
      << " Please read through each tutorial before executing this code.\n"
      << "\n"
      << " [1] Run Tutorial 1\n"
      << " [2] Run Tutorial 2\n"
      << " [3] Run Tutorial 3\n"
      << " [4] Run Tutorial 4\n"
      << " [5] Run Tutorial 5\n"
      << " [6] Run Tutorial 6\n"
      << " [7] Run Tutorial 7\n"
      << " [8] Exit\n"
      << " >> ";

      std::string action;
      std::getline(std::cin, action);

      if      (action == "1") T1::Tutorial_1();
      else if (action == "2") T2::Tutorial_2();
      else if (action == "3") T3::Tutorial_3();
      else if (action == "4") T4::Tutorial_4();
      else if (action == "5") T5::Tutorial_5();
      else if (action == "6") T6::Tutorial_6();
      else if (action == "7") T7::Tutorial_7();
      else if (action == "8") break;
      else    std::cout << "Invalid option!\n";

      std::cout << std::endl;
   }

   return 0;
}
