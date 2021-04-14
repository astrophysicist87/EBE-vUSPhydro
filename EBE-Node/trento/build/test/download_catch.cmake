message(STATUS "Downloading Catch test header")

file(DOWNLOAD
  "https://github.com/catchorg/Catch2/releases/download/v2.4.0/catch.hpp"
  "/projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento/test/catch.hpp"
  TIMEOUT 20
  STATUS status
  TLS_VERIFY ON)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR
    "download failed: code ${status_code} ${status_string}")
endif()
