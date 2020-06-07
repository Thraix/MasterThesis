#pragma once

#include "Voxelizer.h"
#include <logging/Log.h>

struct CommandLineData
{
  private:
    bool valid = true;
  public:
    int voxelGridSize = 128;
    Voxelizer::VoxAlg primaryAlg = Voxelizer::VoxAlg::RLV;
    std::string model = "bunny";
    std::string logFile = "";

    // Performance variables
    bool runPerformance = false;
    int runs = 100;

    // Compare varialbes
    bool runCompare = false;
    Voxelizer::VoxAlg secondaryAlg = Voxelizer::VoxAlg::ILV;

    // Screenshot then close
    bool screenshot;
    std::string screenshotName;

  public:
    CommandLineData(int argc, char **argv);

    bool Valid();
};
