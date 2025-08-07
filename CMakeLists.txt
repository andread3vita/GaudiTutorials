cmake_minimum_required(VERSION 3.12)
project(GaudiTutorial LANGUAGES CXX)

find_package(Gaudi REQUIRED)
find_package(k4FWCore REQUIRED)
find_package(EDM4HEP REQUIRED)

add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable)

# Delegate build to subdirectory
add_subdirectory(HandsOn1)
add_subdirectory(MoliereRadiusFunctional)