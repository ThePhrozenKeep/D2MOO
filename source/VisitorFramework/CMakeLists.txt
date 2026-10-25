add_library(VisitorFramework STATIC 
  include/DifferenceDescriptor.h
  include/DifferenceType.h
  include/DiffContext.h
  include/DiffFieldPath.h
  include/DiffVisitor.h
  include/DynamicArrayCountRegistry.h
  include/FieldVisitor.h
  include/Macros.h
  include/TestComparator.h
  include/VisitorTraits.h

  src/DifferenceDescriptor.cpp
  src/DynamicArrayCountRegistry.cpp
)
target_include_directories(VisitorFramework PUBLIC include)
target_compile_features(VisitorFramework PRIVATE cxx_std_17)
