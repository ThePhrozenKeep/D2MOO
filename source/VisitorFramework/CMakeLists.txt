add_library(VisitorFramework STATIC 
  include/DifferenceType.h
)
target_include_directories(VisitorFramework PUBLIC include)
target_compile_features(VisitorFramework PRIVATE cxx_std_17)
