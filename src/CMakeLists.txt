find_package(fmt REQUIRED)

set(EXES
  d01
)

foreach(EXE ${EXES})
  add_executable(${EXE} ${EXE}.cpp)
  target_include_directories(${EXE} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )
  target_compile_features(${EXE} PRIVATE cxx_std_20)
  target_link_libraries(${EXE} PRIVATE fmt::fmt)
endforeach()
