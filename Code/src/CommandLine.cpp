#include "CommandLine.h"

using namespace Greet;

bool GetVoxAlg(std::string sAlg, Voxelizer::VoxAlg* retAlg)
{
  std::for_each(sAlg.begin(), sAlg.end(), [](char& c) {return std::tolower(c);});
  if(sAlg == "rlv")
    *retAlg = Voxelizer::VoxAlg::RLV;
  else if(sAlg == "ilv")
    *retAlg = Voxelizer::VoxAlg::ILV;
  else if(sAlg == "bresenham" || sAlg == "bre")
    *retAlg = Voxelizer::VoxAlg::Bresenham;
  else
  {
    Log::Error("Invalid voxelization algorithm specified: ", sAlg);
    Log::Error("Valid input: rlv, ilv, bresenham");
    return false;
  }
  return true;
}

std::string GetArg(int argc, char** argv, int index, const std::string& def)
{
  if(index >= argc)
    return def;
  return argv[index];
}

CommandLineData::CommandLineData(int argc, char **argv)
{
  using namespace Greet;

  for(int i = 0;i<argc;i++)
  {
    if(argv[i][0] == '-')
    {
      std::string arg = &argv[i][1];
      if(arg == "h" || arg == "-help")
      {
        std::cout << (1+(char*)R"(
Usage: Voxelizer [options]

  Options:
    -h, --help        Displays this information
    -r, --res         Resolution of voxelization
                        [1-2048]
    -a, --alg         Voxelization algorithm
                        [rlv, ilv, bresenham]
    -e, --eval        Type of voxelization evaluation
                        [perf, comp]
    -n, --runs        Amount of performance runs
    -o, --logfile     Performance logfile
    -c, --compalg     Voxelization algorithm to compare with
                        [rlv, ilv, bresenham]
    -m, --model       Model to voxelize
                        [bunny, dragon, monkey, sphere, tetra,
                        cube, quad, %filename%]
    -s, --screenshot  Take a screenshot and close application instantly
        )") << std::endl;
        valid = false;
        return;
      }
      else if(arg == "r" || arg == "-res")
      {
        voxelGridSize = stoi(GetArg(argc, argv, ++i, "100"));
        if(voxelGridSize < 1 || voxelGridSize > 2048)
        {
          Log::Error("Invalid voxel grid size: ", GetArg(argc, argv, i, "100"));
          Log::Error("Valid input: 1 - 2048");
          valid = false;
          return;
        }
      }
      else if(arg == "a" || arg == "-alg")
      {
        if(!GetVoxAlg(GetArg(argc, argv, ++i, "rlv"), &primaryAlg))
        {
          valid = false;
          return;
        }
      }
      else if(arg == "e" || arg == "-eval")
      {
        runPerformance = false;
        runCompare = false;
        std::string command = GetArg(argc, argv, ++i, "");
        if(command == "perf") // performance measurement
          runPerformance = true;
        else if(command == "comp") // Error analysis
          runCompare = true;
      }
      else if(arg == "n" || arg == "-runs")
      {
        runs = stoi(GetArg(argc, argv, ++i, "100"));
      }
      else if(arg == "o" || arg == "-output" || arg == "-logfile")
      {
        logFile = GetArg(argc, argv, ++i, "");
      }
      else if(arg == "c" || arg == "-compalg")
      {
        if(!GetVoxAlg(GetArg(argc, argv, ++i, "ilv"), &secondaryAlg))
        {
          valid = false;
          return;
        }
      }
      else if(arg == "m" || arg == "-model")
      {
        model = GetArg(argc, argv, ++i, "bunny");
      }
      else if(arg == "s" || arg == "-screenshot")
      {
        screenshot = true;
        screenshotName = GetArg(argc, argv, ++i, "screenshot.png");
      }
    }
  }
}

bool CommandLineData::Valid()
{
  return valid;
}
