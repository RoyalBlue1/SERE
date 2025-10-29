
#include "PakLoading/texture.h"




inline std::string FormatTextureAssetName(const char* const str)
{
    std::string name(str);
    if (!name.starts_with("texture/"))
        name = "texture/" + name;

    if (!name.ends_with(".rpak"))
        name += ".rpak";

    return name;
}


#undef max
void LoadTextureAsset(CPakAsset& pakAsset,ID3D11Device* device,ID3D11Texture2D** texture, ID3D11ShaderResourceView** view)
{


    std::unique_ptr<TextureAsset> txtrAsset = nullptr;

    switch (pakAsset.version())
    {
    case 8:
    {
        TextureAssetHeader_v8_t* v8 = reinterpret_cast<TextureAssetHeader_v8_t*>(pakAsset.header());
        txtrAsset = std::make_unique<TextureAsset>(v8);
        break;
    }
    case 9:
    {
        TextureAssetHeader_v9_t* v9 = reinterpret_cast<TextureAssetHeader_v9_t*>(pakAsset.header());
        txtrAsset = std::make_unique<TextureAsset>(v9, pakAsset.data()->guid);
        break;
    }
    case 10:
    {
        TextureAssetHeader_v10_t* v10 = reinterpret_cast<TextureAssetHeader_v10_t*>(pakAsset.header());
        txtrAsset = std::make_unique<TextureAsset>(v10, pakAsset.data()->guid);
        break;
    }
    default:
    {
        //assertm(false, "unaccounted asset version, will cause major issues!");
        return;
    }
    }

    txtrAsset->totalMipLevels = (txtrAsset->optStreamedMipLevels + txtrAsset->streamedMipLevels + txtrAsset->permanentMipLevels);
    txtrAsset->arraySize = txtrAsset->arraySize == 0 ? 1 : txtrAsset->arraySize;

    const AssetPtr_t optStreamEntry = pakAsset.getStarPakStreamEntry(true);
    const AssetPtr_t streamEntry = pakAsset.getStarPakStreamEntry(false);

    struct
    {
        size_t offsetRPak = 0;
        size_t offsetStarPak = 0;
        size_t offsetOptStarPak = 0;
    } mipOffsets;

    //assertm(txtrAsset->unkMipLevels == 0, "unhandled mip type");

    // start high, this will let us go from bottom to top
    for (int i = txtrAsset->totalMipLevels - 1, compIdx = txtrAsset->streamedMipLevels + txtrAsset->optStreamedMipLevels - 1; i >= 0; --i)
    {
        TextureMip_t mip {};

        if (i < txtrAsset->optStreamedMipLevels)
            mip.type = eTextureMipType::OptStarPak;
        else if (i < (txtrAsset->optStreamedMipLevels + txtrAsset->streamedMipLevels))
            mip.type = eTextureMipType::StarPak;
        else
            mip.type = eTextureMipType::RPak;

        const uint8_t x = s_pBytesPerPixel[txtrAsset->imgFormat].first;
        const uint8_t y = s_pBytesPerPixel[txtrAsset->imgFormat].second;

        const uint16_t mipWidth = static_cast<uint16_t>(std::max(1, (txtrAsset->width >> i)));
        const uint16_t mipHeight = static_cast<uint16_t>(std::max(1, (txtrAsset->height >> i)));

        const uint32_t bppWidth = (y + (mipWidth - 1)) >> (y >> 1);
        const uint32_t bppHeight = (y + (mipHeight - 1)) >> (y >> 1);
        const uint32_t sliceWidth = x * (y >> (y >> 1));

        const uint32_t pitch = sliceWidth * bppWidth;
        const uint32_t slicePitch = x * bppWidth * bppHeight;

        // for swizzling the stored image is aligned.
        uint32_t slicePitchData = slicePitch;


        size_t sizeSingle = IALIGN(slicePitchData, s_MipAligment[txtrAsset->swizzle]); // hard coded for now, however this will vary in the future (swizzling)
        size_t sizeAbsolute = sizeSingle * txtrAsset->arraySize;

        mip.isLoaded = true;
        switch (mip.type)
        {
            case eTextureMipType::RPak:
            {
                mip.compType = eCompressionType::NONE; // default

                if (!pakAsset.cpu())
                {
                    mip.isLoaded = false;
                    break;
                }

                mip.assetPtr.ptr = pakAsset.cpu() + mipOffsets.offsetRPak;
                mipOffsets.offsetRPak += sizeAbsolute;
                break;
            }
            case eTextureMipType::StarPak:
            {
                //assertm(txtrAsset->arraySize == 1, "texture arrays should not be streamed"); // if they are, the packed compressed size should cover this

                mip.compType = txtrAsset->StreamMipCompression(static_cast<uint8_t>(compIdx));

                // Need to re-grab the size on compressed textures.
                if (mip.compType != eCompressionType::NONE)
                {
                    sizeSingle = static_cast<uint64_t>((static_cast<uint32_t>(txtrAsset->compressedBytes[compIdx]) + 1) << 12u); // (1 << 12) = 4096, seems to be aligned to starpak pages.
                    sizeAbsolute = sizeSingle;
                }

                compIdx--;

                // version 6 has some weird starpak stuff going on
                if (IS_ASSET_PTR_INVALID(streamEntry))
                    mip.isLoaded = false;

                mip.assetPtr.offset = streamEntry.offset + mipOffsets.offsetStarPak;
                mipOffsets.offsetStarPak += sizeAbsolute;
                break;
            }
            case eTextureMipType::OptStarPak:
            {
                //assertm(txtrAsset->arraySize == 1, "texture arrays should not be streamed");

                mip.compType = txtrAsset->StreamMipCompression(static_cast<uint8_t>(compIdx));

                // Need to re-grab the size on compressed textures.
                if (mip.compType != eCompressionType::NONE)
                {
                    sizeSingle = static_cast<uint64_t>((static_cast<uint32_t>(txtrAsset->compressedBytes[compIdx]) + 1) << 12u); // (1 << 12) = 4096, seems to be aligned to starpak pages.
                    sizeAbsolute = sizeSingle;
                }

                compIdx--;

                if (IS_ASSET_PTR_INVALID(optStreamEntry))
                    mip.isLoaded = false;

                mip.assetPtr.offset = optStreamEntry.offset + mipOffsets.offsetOptStarPak;
                mipOffsets.offsetOptStarPak += sizeAbsolute;
                break;
            }
            default:
            {
                //assertm(false, "Unknown mip type.");
                break;
            }
        }

        mip.level = txtrAsset->totalMipLevels - static_cast<uint8_t>(i);
        mip.width = mipWidth;
        mip.height = mipHeight;
        mip.pitch = pitch;
        mip.slicePitch = slicePitch;
        mip.sizeSingle = sizeSingle;
        mip.sizeAbsolute = sizeAbsolute;
        mip.swizzle = txtrAsset->swizzle;

        txtrAsset->dataSizeUnaligned += slicePitch; // used for making a block of mips (without pak alignment)
        txtrAsset->mipArray.push_back(mip);
    }

    txtrAsset->mipSorting = txtrAsset->swizzle != 0 ? eTextureMipSorting::SORT_TOP_BOTTOM : txtrAsset->mipSorting;

    // resort mip offset

    for (auto& mip : txtrAsset->mipArray)
    {
        // no shuffling, skip
        if (mip.type == eTextureMipType::RPak && txtrAsset->mipSorting != eTextureMipSorting::SORT_TOP_BOTTOM)
            continue;

        switch (mip.type)
        {
        // should only get hit on ps4 + switch paks
        case eTextureMipType::RPak:
        {
            mipOffsets.offsetRPak -= mip.sizeAbsolute;
            mip.assetPtr.ptr = pakAsset.cpu() + mipOffsets.offsetRPak;
            break;
        }
        case eTextureMipType::StarPak:
        {
            mipOffsets.offsetStarPak -= mip.sizeAbsolute;
            mip.assetPtr.offset = streamEntry.offset + mipOffsets.offsetStarPak;
            break;
        }
        case eTextureMipType::OptStarPak:
        {
            mipOffsets.offsetOptStarPak -= mip.sizeAbsolute;
            mip.assetPtr.offset = optStreamEntry.offset + mipOffsets.offsetOptStarPak;
            break;
        }
        default:
        {
            //assertm(false, "Unknown mip type.");
            break;
        }
        }
    }


    std::sort(txtrAsset->mipArray.begin(), txtrAsset->mipArray.end(), [](const TextureMip_t& a, const TextureMip_t& b) { return a.level < b.level; });

    CreateTextureFromMip(pakAsset,&txtrAsset->mipArray[0],s_PakToDxgiFormat[txtrAsset->imgFormat],0,device,texture,view);
}


void CreateTextureFromMip(CPakAsset& asset, const TextureMip_t* const mip, const DXGI_FORMAT format, const size_t arrayIdx,ID3D11Device* device,ID3D11Texture2D** texture,ID3D11ShaderResourceView** view)
{
    // Texture isn't multiple of 4, most textures are BC which requires the width n height to be multiple of 4 causing a crash.
    if (mip->width < 3 || mip->height < 3)
        return;

    if (!mip->isLoaded)
        return;

    std::unique_ptr<char[]> txtrData = GetTextureDataForMip(asset, mip, format, arrayIdx);
    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Width = mip->width;
    texDesc.Height = mip->height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA resourceDesc{};
    resourceDesc.pSysMem = txtrData.get();
    resourceDesc.SysMemPitch = mip->pitch;
    resourceDesc.SysMemSlicePitch = mip->slicePitch;

    if (HRESULT res = device->CreateTexture2D(&texDesc, &resourceDesc, texture); res != S_OK) {
        printf("CreateTexture2D failed with code 0x%X\n",res);
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Format = format;
    viewDesc.Texture2D.MipLevels = -1;
    viewDesc.Texture2D.MostDetailedMip = 0;
    
    if (HRESULT res = device->CreateShaderResourceView(*texture,&viewDesc,view); res != S_OK) {
        printf("CreateShaderResourceView failed with code 0x%X\n",res);
    }
    return;//TODO std::move(g_dxHandler->CreateRenderTexture(txtrData.get(), mip->slicePitch, mip->width, mip->height, format, 1u, 1u));
};


std::unique_ptr<char[]> GetTextureDataForMip(CPakAsset& asset, const TextureMip_t* const mip, const DXGI_FORMAT format, const size_t arrayIndex)
{
    // [rika]: I swapped back to size (from slicePitch) because it's the size of the mip on disk, and we just create a new buffer anyways if it's compressed. saves some allocation of bytes.
    std::unique_ptr<char[]> txtrData;
    switch (mip->type)
    {
    case eTextureMipType::RPak:
    {
        txtrData = std::make_unique<char[]>(mip->sizeSingle);
        memcpy_s(txtrData.get(), mip->sizeSingle, mip->assetPtr.ptr + (mip->sizeSingle * arrayIndex), mip->sizeSingle);
        break;
    }
    case eTextureMipType::StarPak:
    {
        txtrData = asset.getStarPakData(mip->assetPtr.offset + (mip->sizeSingle * arrayIndex), mip->sizeSingle, false);
        break;
    }
    case eTextureMipType::OptStarPak:
    {
        txtrData = asset.getStarPakData(mip->assetPtr.offset + (mip->sizeSingle * arrayIndex), mip->sizeSingle, true);
        break;
    }
    default:
        //assertm(false, "Unknown mip type.");
        break;
    }

    if (mip->compType != eCompressionType::NONE)
    {
        uint64_t slicePitch = mip->slicePitch;
        txtrData = RTech::DecompressStreamedBuffer(std::move(txtrData), slicePitch, mip->compType);
    }



    return std::move(txtrData);
}