// Copyright ii887522

#ifdef TEST

#include <cstring>
#include <stdexcept>
#include <filesystem>
#include <catch.hpp>
#include "../../main/Any/CommandLine.h"
#include "../../main/Any/constants.h"

using std::invalid_argument;
using std::filesystem::create_directory;
using std::filesystem::remove_all;

namespace ii887522::texturePacker {

TEST_CASE("test CommandLine.h") {
  create_directory("res/test/a/");
  SECTION("test validate() function") {
    {
      char* args[]{ new char[15u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 2u, "a");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "a");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "a");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "3");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "a");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "3");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "3");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize());
    }
    {
      char* args[]{ new char[15u], new char[3u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 3u, "a/");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "3");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "3");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[3u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "3");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "3");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\b\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "3");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "3");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\b\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "3");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\b\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 2u, "a");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\b\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a/");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\b\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\a\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\c\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\d\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\e\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\f\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_THROWS_AS((CommandLine{ sizeof args / sizeof(char*), args }.validate()), invalid_argument);
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\g\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\h\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
    {
      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_I], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_I], 12u, "res\\test\\i\\");
      strcpy_s(args[OUTPUT_DIRECTORY_PATH_I], 3u, "a\\");
      strcpy_s(args[ATLAS_WIDTH_I], 2u, "4");
      strcpy_s(args[ATLAS_HEIGHT_I], 2u, "4");
      REQUIRE_NOTHROW(CommandLine{ sizeof args / sizeof(char*), args }.validate());
    }
  }
  remove_all("res/test/a/");
}

}  // namespace ii887522::texturePacker

#endif
