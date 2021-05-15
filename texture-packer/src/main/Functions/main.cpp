// Copyright ii887522

#ifndef TEST

#define ALLOCATOR_IMPLEMENTATIONS
#include <Any/allocator.h>

#include <SDL.h>
#include <Any/App.h>
#include <Struct/Size.h>
#include <Struct/Color.h>
#include <Functions/control_flow.h>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "../ViewGroupFactory/TexturePackerViewGroupFactory.h"
#include "../Any/CommandLine.h"
#include <SDL_image.h>

using ii887522::viewify::App;
using ii887522::viewify::Size;
using ii887522::viewify::Color;
using ii887522::viewify::eventLoop;
using std::cerr;
using std::invalid_argument;
using std::filesystem::directory_iterator;

namespace ii887522::texturePacker {

static int main(int argc, char** argv) try {
  const CommandLine commandLine{ argc, argv };
  commandLine.validate();
  TexturePackerViewGroupFactory texturePackerViewGroupFactory{ commandLine.getInputDirPath(), commandLine.getOutputDirPath() };  // See also TexturePackerViewGroupFactory.h for more details
  eventLoop(App{ "Texture Packer", Size{ commandLine.getAtlasSize().w, commandLine.getAtlasSize().h }, Color{ 0u, 0u, 0u }, &texturePackerViewGroupFactory, SDL_WINDOW_MINIMIZED });
  return EXIT_SUCCESS;
} catch (const invalid_argument&) {
  cerr << "Command Line: texture-packer <input-directory-path> <atlas-width> <atlas-height>\n";
  cerr << "Param <input-directory-path>: it must exists and ends with either '/' or '\\'\n";
  cerr << "Param <output-directory-path>: it must ends with either '/' or '\\'\n";
  cerr << "Param <atlas-width>: it must be equal to 2^n where n is a non-negative integer\n";
  cerr << "Param <atlas-height>: it must be equal to 2^n where n is a non-negative integer\n";
  return EXIT_FAILURE;
}

}  // namespace ii887522::texturePacker

int main(int argc, char** argv) {
  return ii887522::texturePacker::main(argc, argv);
}

#endif
