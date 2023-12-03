find_package(absl REQUIRED)
find_package(fmt REQUIRED)

set(SRCS
  d01
  d02
)

foreach(EXE ${SRCS})
  add_executable(${EXE} ${EXE}.cpp)
  target_include_directories(${EXE} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )
  target_compile_features(${EXE} PRIVATE cxx_std_20)
  target_compile_options(${EXE} PRIVATE -Wall -Wextra)
  target_link_libraries(${EXE} PRIVATE fmt::fmt absl::strings)
endforeach()
