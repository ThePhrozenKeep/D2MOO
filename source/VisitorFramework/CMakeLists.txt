add_library(VisitorFramework STATIC 
  include/DifferenceDescriptor.h
  include/DifferenceType.h
  include/DiffContext.h
  include/VisitorTraits.h

  src/DifferenceDescriptor.cpp
)
target_include_directories(VisitorFramework PUBLIC include)
target_compile_features(VisitorFramework PRIVATE cxx_std_17)
