
#include "PakLoading/cpakfile.h"
#include "PakLoading/patchapi.h"

#include "PakLoading/utils.h"
#include "ImageAtlas.h"
#include "FontAtlas.h"
#include <fstream>



CPakFile::CPakFile() : m_pPatchDataHeader(nullptr), m_pPatchFileHeaders(nullptr), patchDataBuffer(nullptr), patchStreamCursor(nullptr), m_pAssetsRaw(nullptr), m_pAssetsInternal(nullptr), m_pDependentAssets(nullptr),
m_pGuidRefHeaders(nullptr), m_pHeader(nullptr), m_pPageHeaders(nullptr), m_pPointerHeaders(nullptr), m_pSegmentHeaders(nullptr), m_pExternalAssetRefOffsets(nullptr), m_pExternalAssetRefs(nullptr),
p{}, segmentCollections{}, numAssetsWithProcessedPages(0), numProcessedAssets(0), numProcessedPages(0), numResolvedPointers(0)
{
}


CPakFile::~CPakFile()
{

    for (int i = 0; i < PAK_MAX_SEGMENT_COLLECTIONS; ++i)
    {
        SegmentCollection_t* const collection = &this->segmentCollections[i];
        if (collection->buffer)
        {
            _aligned_free(collection->buffer);
            collection->buffer = nullptr;
        }
    }


    if (nullptr != m_pHeader) delete m_pHeader;
    if (nullptr != m_pAssetsInternal) delete[] m_pAssetsInternal;
}

struct PakFileLoadState_t
{
    PakFileLoadState_t() : fileBuffer(NULL), pageStart(0), pageEnd(0) {};
    std::shared_ptr<char[]> fileBuffer;

    int pageStart;
    int pageEnd;
};

const bool CPakFile::ParseFileBuffer(const std::filesystem::path& path)
{
    if (!ParseFromFile(path, this->m_Buf))
        return false;

    m_FilePath = path.string();

    // parse our initial header (subject to change)
    ParsePakFileHeader(m_Buf.get());



    return this->LoadAndPatchPakFileData<PakHdr_v7_t, PakAsset_v6_t>();

}

const bool CPakFile::ParsePakFileHeader(const char* buf)
{
    // pak should be definitely valid by now
    const short version = reinterpret_cast<const short*>(buf)[2];

    return ParsePakFileHeader(buf, version);
}

const bool CPakFile::ParsePakFileHeader(const char* buf, const short version)
{
    // cleanup the old header (if there is one)
    if (nullptr != m_pHeader)
        delete m_pHeader;


    m_pHeader = new PakHdr_t(reinterpret_cast<const PakHdr_v7_t*>(buf));




    return true;
}


template<class PakHdr, class PakAsset>
const bool CPakFile::LoadAndPatchPakFileData()
{
    if (g_assetData.m_pakLoadStatusMap.count(header()->crc) != 0)
    {
        printf("Pakfile '%s' failed to load because its CRC was already recorded as being loaded.\n", m_FilePath.c_str());

        return false;
    }

    // if this is not consistent across patches.. uhm?
    const short pakVersion = header()->version;

    ResetHeaders();
    CalculateLoadedAssetTypeInfo();

    std::vector<PakFileLoadState_t> pakChain(this->patchCount() + 1);

    // Get the index of the first page contained by this pak.
    this->firstPageIdx = 0;
    if (this->patchCount())
    {
        const PakPatchDataHdr_t* patchDataHeader = header()->GetPatchDataHeader();
        //assertm(patchDataHeader->patchDataStreamSize > 0, "pak patch header has no stream size?");

        this->firstPageIdx = patchDataHeader->patchPageCount;
        this->patchDataBuffer = std::shared_ptr<char[]>(new char[patchDataHeader->patchDataStreamSize]);

        memcpy(patchDataBuffer.get(), this->header()->GetPatchStreamData(), patchDataHeader->patchDataStreamSize);
        ParsePatchEditStream(); // the uhhhhhhhhhhhhhhhhhh
    }

    PakFileLoadState_t* const topPatchFile = &pakChain.front();
    topPatchFile->fileBuffer = this->m_Buf;
    topPatchFile->pageStart  = this->firstPageIdx;
    topPatchFile->pageEnd    = this->pageCount();

    // iterate over each patch file if we have a patch.
    size_t combinedPakBufferSize = this->header()->dcmpSize;

    for (int i = 0; i < this->patchCount(); ++i)
    {
        const uint16_t pakPatchFileIndex = header()->GetPatchFileIndices()[i];

        const std::string patchSuffix = pakPatchFileIndex == 0 ? "" : std::format("({:02})", pakPatchFileIndex);
        const std::filesystem::path patchFilePath = std::filesystem::path(this->m_FilePath).replace_filename(std::format("{}{}.rpak", this->getPakStem(), patchSuffix));

        PakFileLoadState_t loadState = {};
        if (!ParseFromFile(patchFilePath.string(), loadState.fileBuffer))
            return false; // [rexx]: i will deal with this later

        // get PakHdr from the newly loaded and decompressed pak
        const PakHdr* const patchPakHdr = reinterpret_cast<const PakHdr*>(loadState.fileBuffer.get());
        combinedPakBufferSize += patchPakHdr->dcmpSize - sizeof(PakHdr);

        // Save the initialised pak load state into the chain's loaded pakfiles vector.
        pakChain.at(static_cast<size_t>(i + 1)) = loadState;

        if(patchPakHdr->crc != 0 && g_assetData.m_pakLoadStatusMap.count(patchPakHdr->crc) == 0)
            g_assetData.m_pakLoadStatusMap.emplace(patchPakHdr->crc, true);
    }

    std::shared_ptr<char[]> combinedPakDataBuffer = std::make_shared<char[]>(combinedPakBufferSize);

    // copy top patch header into the buffer initially so all other file copies can behave the same way
    *reinterpret_cast<PakHdr*>(combinedPakDataBuffer.get()) = *reinterpret_cast<const PakHdr*>(this->header()->pakPtr);

    size_t nextPakDataOffset = sizeof(PakHdr);
    for (const PakFileLoadState_t& file : pakChain)
    {
        const PakHdr* const fileHeader = reinterpret_cast<const PakHdr*>(file.fileBuffer.get());
        const size_t pakDataSize = fileHeader->dcmpSize - sizeof(PakHdr);

        memcpy(combinedPakDataBuffer.get() + nextPakDataOffset, file.fileBuffer.get() + sizeof(PakHdr), pakDataSize);
        nextPakDataOffset += pakDataSize;
    }
    this->m_Buf = combinedPakDataBuffer;

    // new header of a patched pak buffer
    ParsePakFileHeader(m_Buf.get(), pakVersion);
    ResetHeaders();

    SortAssetsByHeaderPointer<PakAsset>();

    // get number of bytes in the page data part of the combined pak buffer
    this->p.offsetInFileBuffer = header()->GetNonPagedDataSize() + header()->GetPatchDataSize();
    this->p.numRemainingFileBufferBytes = combinedPakBufferSize - (p.offsetInFileBuffer);
    this->p.numBytesToPatch = header()->GetContainedPageDataSize();

    CreateHeaderSegmentCollection();
    AllocateSegments();

    // loop until all pages have been patched correctly
    // this might want a check to prevent infinitely looping
    int numIterations = 0;
    while (!LoadAndPatchAssetData<PakAsset>())
    {
        if (numIterations > 100)
        {
            //assert(0); // If this gets hit, patching has almost definitely failed.
            return false;
        }

        numIterations++;
    };

    this->patchDataBuffer.reset();

    m_pAssetsInternal = new PakAsset_t[assetCount()];

    for (int i = 0; i < assetCount(); ++i)
    {
        PakAsset* const pAsset = reinterpret_cast<PakAsset*>(this->sortedAssetPointers[i]);

        // check if headPagePtr is potentially valid (this should always be true)
        if (pAsset->headPagePtr.index != UINT32_MAX && pAsset->headPagePtr.offset != UINT32_MAX)
            pAsset->headPagePtr.ptr = pageBuffers[pAsset->headPagePtr.index] + pAsset->headPagePtr.offset;
        else
            pAsset->headPagePtr.ptr = nullptr;

        // check if dataPagePtr is potentially valid
        if (pAsset->dataPagePtr.index != UINT32_MAX && pAsset->dataPagePtr.offset != UINT32_MAX)
            pAsset->dataPagePtr.ptr = pageBuffers[pAsset->dataPagePtr.index] + pAsset->dataPagePtr.offset;
        else
            pAsset->dataPagePtr.ptr = nullptr;

        m_pAssetsInternal[i] = PakAsset_t(pAsset);
    }

    // Copy over the non-paged data from the combined buffer and then discard it, since all paged data will have been patched
    // into segment collection buffers by this point.
    std::shared_ptr<char[]> finalHeaderDataBuffer = std::make_unique<char[]>(header()->GetNonPagedDataSize());
    memcpy(finalHeaderDataBuffer.get(), m_Buf.get(), header()->GetNonPagedDataSize());
    m_Buf = finalHeaderDataBuffer;

    // fix header for final time
    ParsePakFileHeader(m_Buf.get(), pakVersion);
    ResetHeaders();

    if (header()->streamingFilesBufSize)
    {
        // var contains the size of the buffer containing streaming file paths
        int remainingLength = header()->streamingFilesBufSize;
        size_t offset = 0;
        while (remainingLength > 0)
        {
            const std::string streamingFilePath = header()->GetStreamingFilePaths() + offset;
            std::filesystem::path fileInfo(streamingFilePath);

            ParseStreamedFile(fileInfo.filename().string(), false);
            const int length = static_cast<int>(streamingFilePath.length());

            // add to offset so we can get the next path on the next iteration
            offset += static_cast<size_t>(length + 1);
            // subtract length + 1 from remaining length so we can terminate when the entire buffer has been found
            remainingLength -= length + 1;
        }
    }

    // if there are any optional streaming files
    if (header()->optStreamingFilesBufSize)
    {
        // var contains the size of the buffer containing "opt" streaming file paths
        int remainingLength = header()->optStreamingFilesBufSize;
        size_t offset = 0;
        while (remainingLength > 0)
        {
            std::string streamingFilePath = header()->GetOptStreamingFilePaths() + offset;
            std::filesystem::path fileInfo(streamingFilePath);

            ParseStreamedFile(fileInfo.filename().string(), true);
            const int length = static_cast<int>(streamingFilePath.length());

            // add to offset so we can get the next path on the next iteration
            offset += static_cast<size_t>(length + 1);
            // subtract length + 1 from remaining length so we can terminate when the entire buffer has been found
            remainingLength -= length + 1;
        }
    }

    ProcessAssets();
    return true;
}


const bool CPakFile::ParseFromFile(const std::filesystem::path& filePath, std::shared_ptr<char[]>& buf)
{

    std::ifstream file{filePath,std::ios::binary};
    if(!file.good())
        return false;
    file.seekg(0,std::ios::end);
    size_t fileSize = file.tellg();
    buf = std::shared_ptr<char[]>(new char[fileSize]);
    file.seekg(0,std::ios::beg);
    file.read(buf.get(),fileSize);
    if (!DecompressFileBuffer(buf.get(), &buf))
        return false;

    return true;
}

const bool CPakFile::ParseStreamedFile(const std::string& fileName, bool opt)
{
    // The end of the starpak path buffers is padded with null bytes to get back to (8 byte?) alignment
    // This check isn't entirely necessary, as we will fail out of this function with a null path anyway, however
    // this avoids needlessly performing a disk operation to attempt to open the file.
    if (fileName.length() == 0)
        return false;


    struct StarPakStreamEntry_t
    {
        uint64_t offset;
        uint64_t size;
    };
    std::unique_ptr<StarPak_t> pakEntry = std::make_unique<StarPak_t>();

    std::string path = std::filesystem::path(m_FilePath).parent_path().string().append("\\" + fileName);
    pakEntry.get()->filePath = path;

    std::ifstream file{path,std::ios::binary};
    file.seekg(0,std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(fileSize - sizeof(uint64_t));

    uint64_t entryCount = 0;
    file.read(reinterpret_cast<char*>(&entryCount), sizeof(uint64_t));
    file.seekg(fileSize - sizeof(uint64_t) - (sizeof(StarPakStreamEntry_t) * entryCount));

    for (uint64_t i = 0; i < entryCount; i++)
    {
        StarPakStreamEntry_t entry;
        file.read(reinterpret_cast<char*>(&entry), sizeof(StarPakStreamEntry_t));

        // [rika]: we should not being adding invalid starpak entries, these only really appear in pak V6 (possibly a bakery issue?)
        if (entry.size == 0)
            continue;

        pakEntry.get()->parsedOffsets.emplace(entry.offset, entry.size);
    }

    opt ? m_vOptStarPaks.emplace_back(std::move(pakEntry)) : m_vStarPaks.emplace_back(std::move(pakEntry));
    return true;
}

const bool CPakFile::DecompressFileBuffer(const char* fileBuffer, std::shared_ptr<char[]>* outBuffer)
{
    const short version = reinterpret_cast<const short*>(fileBuffer)[2];

    const PakHdr_t* header = nullptr;

    switch (version)
    {
    case 6: // no compression on 6
        return true;
    case 7:
        header = new PakHdr_t(reinterpret_cast<const PakHdr_v7_t*>(fileBuffer));
        break;
    case 8:
        header = new PakHdr_t(reinterpret_cast<const PakHdr_v8_t*>(fileBuffer));
        break;
    default:
        return false;
    }

    if (header->magic != pakFileMagic)
    {
        delete header;
        return false;
    }

    if ((header->flags & 0x300) == 0)
    {
        delete header;
        return true;
    }

    if (header->flags & 0x100) // standard pakfile compression
    {
        std::shared_ptr<char[]> dcmpBuf = std::shared_ptr<char[]>(new char[header->dcmpSize] {});

        RTech::PakDecompressContext_t context = {};
        uint64_t decodeSize = RTech::InitPakDecoder(&context, reinterpret_cast<const uint8_t*>(fileBuffer), PAK_DECODE_MASK, header->cmpSize, 0, header->pakHdrSize);

        context.m_outputMask = PAK_DECODE_MASK;
        context.m_outputBuf = uint64_t(dcmpBuf.get());

        if (RTech::DecompressPakFile(&context, header->cmpSize, decodeSize))
        {

            // get pakhdr back from compressed buffer
            memcpy_s(dcmpBuf.get(), header->pakHdrSize, fileBuffer, header->pakHdrSize);

            if (outBuffer->get() != nullptr)
                outBuffer->reset();

            // overwrite the provided buffer with the newly allocated and populated buffer
            *outBuffer = dcmpBuf;
        }
        else
        {
            delete header;
            return false;
        }
    }
    else if (header->flags & 0x200)
    {
        std::shared_ptr<char[]> dcmpBuf = std::shared_ptr<char[]>(new char[header->dcmpSize + header->pakHdrSize] {});

        const size_t compressedDataSize = header->cmpSize;

        // allocate a buffer for just the compressed file data
        // since the oodle decomp util func needs just the compressed data
        std::unique_ptr<char[]> cmpBuf = std::make_unique<char[]>(compressedDataSize);
        memcpy_s(cmpBuf.get(), compressedDataSize, fileBuffer + header->pakHdrSize, compressedDataSize);

        uint64_t decodeSize = header->dcmpSize;

        std::unique_ptr<char[]> data = RTech::DecompressStreamedBuffer(std::move(cmpBuf), decodeSize, eCompressionType::OODLE);

        if (decodeSize == 0 || data.get() == nullptr)
        {
            delete header;
            return false;
        }

        //assertm(decodeSize == header->dcmpSize, "mismatch on decode size.");

        // copy pak header to the decompressed buffer
        memcpy_s(dcmpBuf.get(), header->pakHdrSize, fileBuffer, header->pakHdrSize);

        // copy all of the decoded data into the decompressed buffer
        memcpy_s(dcmpBuf.get() + header->pakHdrSize, header->dcmpSize, data.get(), decodeSize);

        // release the oodle decomp buffer now we are done with it
        data.release();

        if (outBuffer->get() != nullptr)
            outBuffer->reset();

        // overwrite the provided buffer with the newly allocated and populated buffer
        *outBuffer = dcmpBuf;
    }

    delete header;
    return true;
}

#undef max
void CPakFile::CreateHeaderSegmentCollection()
{
    // offset in "HEADER" segment collection buffer for the next asset type's header data
    size_t nextOffsetForAssetTypeHeaders = 0;

    for (auto& it : this->loadedAssetTypeInfo)
    {
        const uint32_t assetType = it.first;
        PakLoadedAssetTypeInfo_t& assetTypeInfo = it.second;
        
        uint32_t headerAlignment = 8;


        // Align the next free header offset to the required alignment of this type's headers
        const size_t nextOffsetAligned = IALIGN(nextOffsetForAssetTypeHeaders, static_cast<size_t>(headerAlignment));

        assetTypeInfo.offsetToNextHeaderInCollection = nextOffsetAligned;
        nextOffsetForAssetTypeHeaders = nextOffsetAligned + (assetTypeInfo.assetCount * assetTypeInfo.headerSize);

        // == Update "HEADER" segment collection ==
        SegmentCollection_t* const collection = &this->segmentCollections[SegmentCollection_t::eType::SCT_HEAD];

        // The size of the header collection will now be equal to the calculated value for the next offset,
        // as this accounts for all of the headers of the asset type we have just processed.
        collection->dataSize = nextOffsetForAssetTypeHeaders;
        collection->dataAlignment = std::max(collection->dataAlignment, headerAlignment);
    }
}

void CPakFile::AllocateSegments()
{
    // for storing the next available space in the collection buffer for each segment
    // each segment gets allocated its own portion of the collection buffer to store all of its pages
    // the offset begins at the start of that portion before each page's aligned size is added on
    size_t segmentNextPageOffsets[PAK_MAX_SEGMENTS] = {};

    for (int segmentIdx = 0; segmentIdx < this->segmentCount(); ++segmentIdx)
    {
        const PakSegmentHdr_t* const segment = &this->header()->GetSegmentHeaders()[segmentIdx];
        SegmentCollection_t* const collection = &this->segmentCollections[segment->GetType()];

        // Only handle non-header segments in this function, as header segments are dealt with separately
        // in CPakFile::CreateHeaderSegmentCollection.
        if (!segment->IsHeaderSegment())
        {
            // align the end of the collection's data to the segment's alignment value
            const size_t collectionSizeAligned = IALIGN(collection->dataSize, static_cast<size_t>(segment->align));

            // offset for this segment's first page is the aligned size of the collection before this segment was added
            segmentNextPageOffsets[segmentIdx] = collectionSizeAligned;

            // align collection size to the alignment of the new segment, then add the new segment's size
            collection->dataSize = collectionSizeAligned + segment->size;

            // if the segment's alignment is greater than the collection's existing alignment, we need to increase the alignment
            // to accommodate this segment. this guarantees that this segment is able to align itself to its required boundary
            collection->dataAlignment = std::max(collection->dataAlignment, segment->align);
        }
    }

    for (int8_t i = 0; i < 4; ++i)
    {
        SegmentCollection_t* const collection = &this->segmentCollections[i];

        // skip any empty collections
        if (collection->dataSize == 0)
            continue;

        // We must allocate aligned memory for the segment, as the asset loading code may rely on aligned data for optimisation.
        // Some SIMD intrinsics require input data to be aligned to a certain boundary, which is only possible if the entire
        // buffer is aligned.
        // This data will be aligned to the highest alignment of any contained page data, as all other data will then be able
        // to align themselves within this alignment.
        // Since all alignments must be a power of 2, data with smaller alignments will always be aligned when the alignment is greater.
        collection->buffer = reinterpret_cast<char*>(_aligned_malloc(collection->dataSize, collection->dataAlignment));
    }

    this->pageBuffers.resize(this->pageCount());

    for (int pageIdx = 0; pageIdx < this->pageCount(); ++pageIdx)
    {
        const PakPageHdr_t* const page = &this->header()->GetPageHeaders()[pageIdx];
        const PakSegmentHdr_t* const segment = &this->header()->GetSegmentHeaders()[page->segment];

        if (!segment->IsHeaderSegment())
        {
            const size_t pageOffsetAligned = IALIGN(segmentNextPageOffsets[page->segment], page->align);

            this->pageBuffers[pageIdx] = this->segmentCollections[segment->GetType()].buffer + pageOffsetAligned;

            segmentNextPageOffsets[page->segment] = pageOffsetAligned + page->size;
        }
        else
        {
            this->pageBuffers[pageIdx] = this->segmentCollections[SegmentCollection_t::eType::SCT_HEAD].buffer;
        }
    }
}

const bool CPakFile::DecodePatchCommands()
{
    if (!this->p.patchFunc)
        p.patchFunc = g_pakPatchApi[CMD_0];

    RBitRead* const bitbuf = &p.bitbuf;
    while (p.patchDestinationSize + p.numBytesToSkip)
    {
        int8_t cmd = 0;
        if (p.numBytesToPatch == 0)
        {
            bitbuf->ConsumeData(patchStreamCursor, bitbuf->BitsAvailable());

            // advance patch data buffer by the number of bytes that have just been fetched
            this->patchStreamCursor = &patchStreamCursor[bitbuf->BitsAvailable() >> 3];

            // store the number of bits remaining to complete the data read
            bitbuf->m_bitsUnoccupied = bitbuf->BitsAvailable() & 7; // number of bits above a whole byte

            cmd = p.commands[bitbuf->ReadBits(6)];

            bitbuf->DiscardBits(p.unk[bitbuf->ReadBits(6)]);

            // get the next patch function to execute
            p.patchFunc = g_pakPatchApi[cmd];

            if (cmd <= 3u)
            {
                const int index = static_cast<int>(bitbuf->ReadBits(8));
                const uint8_t bitExponent = p.unk2[index]; // number of stored bits for the data size

                bitbuf->DiscardBits(p.unk3[index]);

                p.numBytesToPatch = (1ull << bitExponent) + bitbuf->ReadBits(bitExponent);

                bitbuf->DiscardBits(bitExponent);
            }
            else
            {
                p.numBytesToPatch = s_patchCmdToBytesToProcess[cmd];
            }
        }

        if (!p.patchFunc(this, &p.numRemainingFileBufferBytes))
            break;
    }

    return p.patchDestinationSize == 0;
}

extern __int64 PakPatch_DecodeData(char* buffer, int arrayBitDepth, char* out_1, char* out_2);


void CPakFile::ParsePatchEditStream()
{
    char* buf = patchDataBuffer.get();
    buf = &buf[PakPatch_DecodeData(buf, 6, p.commands, p.unk)];
    buf = &buf[PakPatch_DecodeData(buf, 8, p.unk2, p.unk3)];

    this->p.bitbuf.ConsumeData(buf, 64);
    this->p.bitbuf.m_bitsUnoccupied = 0;

    const uint32_t patchDataOffset = static_cast<uint32_t>(this->p.bitbuf.ReadBits(24));
    this->p.bitbuf.DiscardBits(24); // clear 24 bits from the offset we have just read

    p.patchReplacementData = buf + patchDataOffset; // offset to actual patch data
    this->patchStreamCursor = buf + sizeof(uint64_t);
}


void CPakFile::CalculateLoadedAssetTypeInfo()
{
    for (int i = 0; i < this->assetCount(); ++i)
    {
        //const PakAsset_t* const asset = &this->internalAssets()[i];
        void* asset = this->rawAsset(static_cast<size_t>(i));

        const uint32_t type = PakAsset_t::Type(asset, this->header()->version);
        const uint32_t headerStructSize = PakAsset_t::HeaderStructSize(asset, this->header()->version);
        const uint32_t version = PakAsset_t::Version(asset, this->header()->version);

#if defined(ASSERTS)
        const bool assetTypeAlreadyFound = this->loadedAssetTypeInfo.contains(type);
#endif // #if defined(ASSERTS)

        PakLoadedAssetTypeInfo_t* const assetType = &this->loadedAssetTypeInfo[type];

#if defined(ASSERTS)
        if (assetTypeAlreadyFound)
        {
            assertm(assetType->headerSize == headerStructSize, "Mismatched asset header size. Already found asset of the same type with a different header size.");
            assertm(assetType->version == version, "Mismatched asset version. Already found asset of the same type with a different version.");
        }
#endif // #ifdef ASSERTS

        assetType->assetCount++;
        assetType->headerSize = headerStructSize;

        assetType->version = version;
    }
}

template<class PakAsset>
void CPakFile::SortAssetsByHeaderPointer()
{
    //assertm(this->sortedAssetPointers.empty(), "already sorted asset pointers.");

    this->sortedAssetPointers.resize(this->assetCount());
    std::vector<PakAsset*> tempAssetPointers(this->assetCount());

    auto sortAssetFunc = [this](PakAsset* const a, PakAsset* const b)
    {
#define SWAP_DWORDS(num) ((num >> 32) | ((num & 0xFFFFFFFF) << 32))
        return SWAP_DWORDS(reinterpret_cast<size_t>(a->headPagePtr.ptr)) < SWAP_DWORDS(reinterpret_cast<size_t>(b->headPagePtr.ptr));
#undef SWAP_DWORDS
    };

    int numAssetsInNewPages = 0;
    for (int i = 0; i < this->assetCount(); ++i)
    {
        PakAsset* const asset = reinterpret_cast<PakAsset*>(this->rawAsset(i));
        this->sortedAssetPointers.at(i) = asset;

        if (this->firstPageIdx != 0 && asset->headPagePtr.index >= this->firstPageIdx)
            numAssetsInNewPages++;
    }

    // sort once to group all assets that use new pages together
    std::sort(reinterpret_cast<std::vector<PakAsset*>*>(&this->sortedAssetPointers)->begin(), reinterpret_cast<std::vector<PakAsset*>*>(&this->sortedAssetPointers)->end(), sortAssetFunc);

    const int numOldAssets = this->assetCount() - numAssetsInNewPages;

    // copy all of the assets in new pages to the front of the vector
    memcpy(
        tempAssetPointers.data(),
        this->sortedAssetPointers.data() + numOldAssets,
        sizeof(PakAsset*) * numAssetsInNewPages
    );

    // copy all of the assets in old pages to the back of the vector
    memcpy(
        tempAssetPointers.data() + numAssetsInNewPages,
        this->sortedAssetPointers.data(),
        sizeof(PakAsset*) * numOldAssets
    );

    //this->sortedAssetPointers = tempAssetPointers;
    memcpy(
        this->sortedAssetPointers.data(),
        tempAssetPointers.data(),
        sizeof(void*) * assetCount()
    );
}




static std::unordered_map<AssetType_t, std::string> s_ParsedPrefixes(63);


#include "PakLoading/texture.h"

void CPakFile::ProcessAssets()
{

    for (int i = 0; i < assetCount(); i++) {
        if (m_pAssetsInternal[i].type == (uint32_t)AssetType_t::UIMG) {
            CPakAsset asset{this,&m_pAssetsInternal[i],""};
            UIImageAtlasAssetHeader_v10_t* hdr = (UIImageAtlasAssetHeader_v10_t*)asset.header();
            size_t textureId = ~0LL;
            for (int j = 0; j < assetCount(); j++) {
                if (m_pAssetsInternal[j].guid == hdr->atlasGUID) {
                    CPakAsset asset{ this, &m_pAssetsInternal[j], "" };
                    textureId = LoadTextureAsset(asset);
                }
            }
            if(textureId==~0LL)
                continue;
            loadImageAtlasFromRpak(hdr,(ShaderSizeData_t*)asset.cpu(),textureId);
        }
        else if (m_pAssetsInternal[i].type == (uint32_t)AssetType_t::FONT) {
            CPakAsset asset{this,&m_pAssetsInternal[i],""};
            UIFontAtlasAssetHeader_v6_t* hdr = (UIFontAtlasAssetHeader_v6_t*)asset.header();
            size_t textureId = ~0LL;
            for (int j = 0; j < assetCount(); j++) {
                if (m_pAssetsInternal[j].guid == hdr->atlasGUID) {
                    CPakAsset asset{ this, &m_pAssetsInternal[j], "" };

                    textureId = LoadTextureAsset(asset);
                }
            }
            if(textureId==~0LL)
                continue;
            loadRpakFont(hdr,textureId);
        }
    }
    

}


void LoadRpak(const std::filesystem::path& path) {
    CPakFile file;
    file.ParseFileBuffer(path);
}