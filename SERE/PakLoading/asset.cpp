//from github.com/r-ex/rsx

#include <execution>

#include "PakLoading/asset.h"
#include "PakLoading/utils.h"

static std::vector<uint32_t> postLoadOrder =
{
    'rtxt', // Texture first.
    'gmiu', // UI Atlas after.

    'rdhs', // Shader hdr first.
    'sdhs', // Shader set after.
    'ltam', // Material after.

    // [rika]: aseq after arig/model that way the skeleton is set before parsing
    'gira', // Arig first
    '_ldm', // Model after
    'qesa', // Aseq last

};

void CGlobalAssetData::ProcessAssetsPostLoad()
{



    
}

CGlobalAssetData g_assetData;