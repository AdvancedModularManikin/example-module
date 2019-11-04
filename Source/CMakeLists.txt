#############################
# CMake - Test Module - root/Source
#############################

set(SourceFiles
   ExampleModule.cpp
   Tutorial_1.cpp
   Tutorial_2.cpp
   Tutorial_3.cpp
   Tutorial_4.cpp
   Tutorial_5.cpp
   Tutorial_6.cpp
   Tutorial_7.cpp
)

add_executable(AMMExampleModule ${SourceFiles})

target_link_libraries(
   AMMExampleModule
   PUBLIC amm_std
   PUBLIC fastcdr
   PUBLIC fastrtps
)
