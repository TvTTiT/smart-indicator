# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/thanh/espv5.1.1/esp-idf/components/bootloader/subproject"
  "D:/3s-test/smart-indicator/build/bootloader"
  "D:/3s-test/smart-indicator/build/bootloader-prefix"
  "D:/3s-test/smart-indicator/build/bootloader-prefix/tmp"
  "D:/3s-test/smart-indicator/build/bootloader-prefix/src/bootloader-stamp"
  "D:/3s-test/smart-indicator/build/bootloader-prefix/src"
  "D:/3s-test/smart-indicator/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/3s-test/smart-indicator/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/3s-test/smart-indicator/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
