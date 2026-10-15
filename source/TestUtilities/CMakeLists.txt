add_library(TestUtilities STATIC 
  include/FunctionPair.h
)
target_include_directories(TestUtilities PUBLIC include)
target_compile_features(TestUtilities PRIVATE cxx_std_17)
