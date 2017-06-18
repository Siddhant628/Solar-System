#pragma once

// Windows
#include <SDKDDKVer.h>
#include <stdio.h>
#include <wrl.h>

// DirectX
#include <DirectXMath.h>

// Standard
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Library
#include "Utility.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "..\Library.Shared\ModelMaterial.h"

// Library.Desktop
#include "UtilityWin32.h"

 // Local
#include "ModelProcessor.h"
#include "MeshProcessor.h"
#include "ModelMaterialProcessor.h"