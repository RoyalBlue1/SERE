#pragma once
#include <string>
#include <filesystem>
#include <map>
#include <unordered_map>
#include "PakLoading/utils.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

enum class AssetType_t
{
	// model
	// pak
	MDL_ = MAKEFOURCC('m', 'd', 'l', '_'),
	ARIG = MAKEFOURCC('a', 'r', 'i', 'g'),
	ASEQ = MAKEFOURCC('a', 's', 'e', 'q'),
	ANIR = MAKEFOURCC('a', 'n', 'i', 'r'),
	// non pak
	MDL  = MAKEFOURCC('m', 'd', 'l', '\0'),	
	SEQ  = MAKEFOURCC('s', 'e', 'q', '\0'),

	// texture/material
	MATL = MAKEFOURCC('m', 'a', 't', 'l'),
	MSNP = MAKEFOURCC('m', 's', 'n', 'p'),
	MT4A = MAKEFOURCC('m', 't', '4', 'a'),
	TXTR = MAKEFOURCC('t', 'x', 't', 'r'),
	TXAN = MAKEFOURCC('t', 'x', 'a', 'n'),
	TXLS = MAKEFOURCC('t', 'x', 'l', 's'),
	TXTX = MAKEFOURCC('t', 'x', 't', 'x'),
	UIMG = MAKEFOURCC('u', 'i', 'm', 'g'),
	UIIA = MAKEFOURCC('u', 'i', 'i', 'a'),
	FONT = MAKEFOURCC('f', 'o', 'n', 't'),

	// particle (texture)
	EFCT = MAKEFOURCC('e', 'f', 'c', 't'),
	RPSK = MAKEFOURCC('r', 'p', 's', 'k'),

	// dx/shader
	SHDR = MAKEFOURCC('s', 'h', 'd', 'r'),
	SHDS = MAKEFOURCC('s', 'h', 'd', 's'),

	// ui
	UI   = MAKEFOURCC('u', 'i', '\0', '\0'),
	HSYS = MAKEFOURCC('h', 's', 'y', 's'),
	RLCD = MAKEFOURCC('r', 'l', 'c', 'd'),
	RTK  = MAKEFOURCC('r', 't', 'k', '\0'),

	// pak
	PTCH = MAKEFOURCC('P', 't', 'c', 'h'),
	VERS = MAKEFOURCC('v', 'e', 'r', 's'), // patch version

	// descriptors (stats, specs, etc)
	DTBL = MAKEFOURCC('d', 't', 'b', 'l'),
	STGS = MAKEFOURCC('s', 't', 'g', 's'),
	STLT = MAKEFOURCC('s', 't', 'l', 't'),
	RSON = MAKEFOURCC('r', 's', 'o', 'n'),
	SUBT = MAKEFOURCC('s', 'u', 'b', 't'),
	LOCL = MAKEFOURCC('l', 'o', 'c', 'l'),

	// vpk
	WRAP = MAKEFOURCC('w', 'r', 'a', 'p'),
	WEPN = MAKEFOURCC('w', 'e', 'p', 'n'),
	IMPA = MAKEFOURCC('i', 'm', 'p', 'a'),

	// map
	RMAP = MAKEFOURCC('r', 'm', 'a', 'p'),
	LLYR = MAKEFOURCC('l', 'l', 'y', 'r'),

	// odl
	ODLA = MAKEFOURCC('o', 'd', 'l', 'a'),
	ODLC = MAKEFOURCC('o', 'd', 'l', 'c'),
	ODLP = MAKEFOURCC('o', 'd', 'l', 'p'),

	// audio
	ASRC = MAKEFOURCC('a', 's', 'r', 'c'), // source
	AEVT = MAKEFOURCC('a', 'e', 'v', 't'), // event
};


struct AssetVersion_t
{
	AssetVersion_t() : majorVer(0), minorVer(0) {};
	AssetVersion_t(int majorVersion) : majorVer(majorVersion), minorVer(0) {};
	AssetVersion_t(int majorVersion, int minorVersion) : majorVer(majorVersion), minorVer(minorVersion) {};

	int majorVer;
	int minorVer;

	std::string ToString() const
	{
		return minorVer == 0 ? std::format("v{}", majorVer) : std::format("v{}.{}", majorVer, minorVer);
	}
};

class CAssetContainer;

class CAsset
{
public:

	enum ContainerType
	{
		PAK,
		MDL,
		AUDIO,


		_COUNT
	};

	virtual ~CAsset() {};

	// Get the display name for this asset.
	const std::string& GetAssetName() const { return m_assetName; }
	const AssetVersion_t& GetAssetVersion() const { return m_assetVersion; }
	virtual const uint64_t GetAssetGUID() const = 0;

	// Get the type of container file that this asset came from.
	virtual const ContainerType GetAssetContainerType() const = 0;

	// Get the type of this asset within the container file. (e.g., model, material, sound)
	virtual uint32_t GetAssetType() const = 0;

	const bool GetExportedStatus() const { return m_exported; }

	void* GetAssetData() const { return m_assetData; }

	// Get the name of the file that contains this asset.
	virtual std::string GetContainerFileName() const = 0;

	void* GetContainerFile() const { return m_containerFile; }

	template <typename T>
	T* GetContainerFile() const { return static_cast<T*>(m_containerFile); }

	// setters

	void SetAssetName(const std::string& name)
	{
		m_assetName = std::filesystem::path(name).make_preferred().string();;
	};

	void SetAssetVersion(const AssetVersion_t& version)
	{
		m_assetVersion = version;
	}

	//void SetAssetName(const std::filesystem::path& path)
	//{
	//	m_assetName = std::filesystem::path(path).make_preferred().string();
	//}

	// Set the pointer to the internal asset data that was used for creating this asset.
	void SetInternalAssetData(void* ptr)
	{
		m_assetData = ptr;
	}

	// Set the pointer to the class of the file that contains this asset.
	void SetContainerFile(CAssetContainer* ptr)
	{
		m_containerFile = ptr;
	}

	void SetExportedStatus(bool exported)
	{
		m_exported = exported;
	}

private:
	std::string m_assetName;
	AssetVersion_t m_assetVersion;

	bool m_exported;

protected:
	void* m_assetData;

	void* m_containerFile;
};

class CAssetContainer
{
public:
	typedef CAsset::ContainerType ContainerType;

	virtual ~CAssetContainer() {};

	virtual const ContainerType GetContainerType() const = 0;

};

// functions for asset loading.
typedef void(*AssetLoadFunc_t)(CAssetContainer* container, CAsset* asset);

// functions for previewing the asset
typedef void* (*AssetPreviewFunc_t)(CAsset* const asset, const bool firstFrameForAsset);

// functions around exporting the asset.
typedef bool(*AssetExportFunc_t)(CAsset* const asset, const int setting);

#define REGISTER_TYPE(type) g_assetData.m_assetTypeBindings[type.type] = type

struct AssetTypeBinding_t
{
	uint32_t type;
	uint32_t headerAlignment;
	AssetLoadFunc_t loadFunc;

};

class CGlobalAssetData
{
public:
	std::vector<CAssetContainer*> v_assetContainers;

	struct AssetLookup_t
	{
		uint64_t m_guid;
		CAsset* m_asset;
	};

	std::vector<AssetLookup_t> v_assets;
	std::map<uint32_t, AssetTypeBinding_t> m_assetTypeBindings;

	// map of pak crc to status of whether the pak has already been loaded
	std::unordered_map<uint64_t, bool> m_pakLoadStatusMap;

	std::unordered_map<std::string, uint8_t> m_patchMasterEntries;

	CAssetContainer* m_pakPatchMaster;

	CAsset* const FindAssetByGUID(const uint64_t guid)
	{
		const auto it = std::ranges::find(v_assets, guid, &AssetLookup_t::m_guid);
		return it != v_assets.end()
			? it->m_asset : nullptr;
	}

	template<typename T>
	T* const FindAssetByGUID(const uint64_t guid)
	{
		const auto it = std::ranges::find(v_assets, guid, &AssetLookup_t::m_guid);
		return it != v_assets.end()
			&& it->m_asset->GetAssetContainerType() == CAsset::ContainerType::PAK
			? static_cast<T*>(it->m_asset) : nullptr;
	}

	void ClearAssetData()
	{
		for (const auto& lookup : v_assets)
		{
			delete lookup.m_asset;
		}
		v_assets.clear();
		v_assets.shrink_to_fit();

		for (CAssetContainer* container : v_assetContainers)
		{
			delete container;
		}
		v_assetContainers.clear();
		v_assetContainers.shrink_to_fit();

		if (m_pakPatchMaster)
		{
			delete m_pakPatchMaster;
			m_pakPatchMaster = NULL;
		}

		m_patchMasterEntries.clear();
		m_pakLoadStatusMap.clear();
	}

	void ProcessAssetsPostLoad();
};

extern CGlobalAssetData g_assetData;
