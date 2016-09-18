# ACTParser
Parse .acts file generated with ACTS system.

## Usage

1. Add all source into your project.
2. 
```C++
  #include "ActsFileParser.h"
  :::
  DFF_VideoMat::CActsFileParser parser;
  parser.LoadProject(filename);
  parser.:::
```

## Convention of Extrinsic Parameters

`T = [R|t]`, where `R` is the rotation and `t` is the translation (world frame with respect to camera frame)
