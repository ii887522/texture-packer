// Copyright ii887522

#ifdef TEST

#include "CommandLine.test.h"
#include <cstring>
#include <cassert>
#include <stdexcept>
#include "../../main/Any/CommandLine.h"
#include "../../main/Any/constants.h"

using std::invalid_argument;

namespace ii887522::texturePacker {

static void testValidate() {
  try {
    char* args[]{ new char[15u] };
    strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
    CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize();
  } catch (const invalid_argument&) {
    try {
      char* args[]{ new char[15u], new char[2u] };
      strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
      strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 2u, "a");
      CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize();
    } catch (const invalid_argument&) {
      try {
        char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
        strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
        strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 2u, "a");
        strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
        strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "a");
        strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "a");
        CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize();
      } catch (const invalid_argument&) {
        try {
          char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
          strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
          strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 2u, "a");
          strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
          strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "3");
          strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "a");
          CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize();
        } catch (const invalid_argument&) {
          try {
            char* args[]{ new char[15u], new char[2u], new char[2u], new char[2u], new char[2u] };
            strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
            strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 2u, "a");
            strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
            strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "3");
            strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "3");
            CommandLine{ sizeof args / sizeof(char*), args }.getAtlasSize();
          } catch (const invalid_argument&) {
            assert(false);
          }
          try {
            char* args[]{ new char[15u], new char[3u], new char[2u], new char[2u], new char[2u] };
            strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
            strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 3u, "a/");
            strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
            strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "3");
            strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "3");
            CommandLine{ sizeof args / sizeof(char*), args }.validate();
          } catch (const invalid_argument&) {
            try {
              char* args[]{ new char[15u], new char[3u], new char[2u], new char[2u], new char[2u] };
              strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
              strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 3u, "a\\");
              strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
              strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "3");
              strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "3");
              CommandLine{ sizeof args / sizeof(char*), args }.validate();
            } catch (const invalid_argument&) {
              try {
                char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
                strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
                strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 12u, "res\\test\\b\\");
                strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
                strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "3");
                strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "3");
                CommandLine{ sizeof args / sizeof(char*), args }.validate();
              } catch (const invalid_argument&) {
                try {
                  char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
                  strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
                  strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 12u, "res\\test\\b\\");
                  strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
                  strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "4");
                  strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "3");
                  CommandLine{ sizeof args / sizeof(char*), args }.validate();
                } catch (const invalid_argument&) {
                  try {
                    char* args[]{ new char[15u], new char[12u], new char[2u], new char[2u], new char[2u] };
                    strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
                    strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 12u, "res\\test\\b\\");
                    strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 2u, "a");
                    strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "4");
                    strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "4");
                    CommandLine{ sizeof args / sizeof(char*), args }.validate();
                  } catch (const invalid_argument&) {
                    try {
                      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
                      strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
                      strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 12u, "res\\test\\b\\");
                      strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 3u, "a/");
                      strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "4");
                      strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "4");
                      CommandLine{ sizeof args / sizeof(char*), args }.validate();
                    } catch (const invalid_argument&) {
                      assert(false);
                    }
                    try {
                      char* args[]{ new char[15u], new char[12u], new char[3u], new char[2u], new char[2u] };
                      strcpy_s(args[PROGRAM_NAME_INDEX], 15u, "texture-packer");
                      strcpy_s(args[INPUT_DIRECTORY_PATH_INDEX], 12u, "res\\test\\b\\");
                      strcpy_s(args[OUTPUT_DIRECTORY_PATH_INDEX], 3u, "a\\");
                      strcpy_s(args[ATLAS_WIDTH_INDEX], 2u, "4");
                      strcpy_s(args[ATLAS_HEIGHT_INDEX], 2u, "4");
                      CommandLine{ sizeof args / sizeof(char*), args }.validate();
                    } catch (const invalid_argument&) {
                      assert(false);
                    }
                    return;
                  }
                }
              }
            }
          }
          assert(false);
        }
      }
    }
  }
  assert(false);
}

void testCommandLine() {
  testValidate();
}

}  // namespace ii887522::texturePacker

#endif
