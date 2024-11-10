#include "l1cache.h"

#define SET_SIZE 2

uint32_t time;

uint8_t DRAMBuffer[DRAM_SIZE];

uint8_t L1CacheBuffer[L1_SIZE];
CacheLine L1CacheLines[L1_SIZE / BLOCK_SIZE];

MemoryLayer DRAM;
Cache L1Cache;
MemoryLayer* firstLayer = NULL;

/**************** Time Manipulation ***************/

void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/**************** Memory Manipulation ***************/

/*
 * @brief converts a MemoryLayer* to a Cache*.
 * Calling this function with a pointer that is not garanteed
 * to be a Cache* is undefined behaviour.
 *
 * @param layer, MemoryLayer* to be converted.
 *
 * @return Cache* to the corresponding layer.
 */
static Cache* MemoryLayerToCache(MemoryLayer* layer) {
	if (!layer || !layer->next)
		exit(-1);
	return (Cache*) layer;
}

/*
 * @See MemoryLayerToCache
 */
static const Cache* MemoryLayerToCacheConst(const MemoryLayer* layer) {
	if (!layer || !layer->next)
		exit(-1);
	return (const Cache*) layer;
}

/*
 * @brief converts a Cache* to an AssociativeCache*.
 * Calling this on a Cache that is not associative is undefined behaviour.
 *
 * @param cache, Cache* to be converted.
 *
 * @return AssociativeCache* to the corresponding cache.
 */
static AssociativeCache* CacheToAssociativeCache(Cache* cache) {
	if (!cache || !cache->associative)
		exit(-1);
	return (AssociativeCache*) cache;
}

/*
 * @See CacheToAssociativeCache
 */
static const AssociativeCache* CacheToAssociativeCacheConst(const Cache* cache) {
	if (!cache || !cache->associative)
		exit(-1);
	return (const AssociativeCache*) cache;
}

void initCache() {
	DRAM.buffer = DRAMBuffer;
	DRAM.blockCount = DRAM_SIZE / BLOCK_SIZE;
	DRAM.next = NULL;
	DRAM.readTime = DRAM_READ_TIME;
	DRAM.writeTime = DRAM_WRITE_TIME;

	L1Cache.layer.buffer = L1CacheBuffer;
	L1Cache.layer.blockCount =  L1_SIZE / BLOCK_SIZE;
	L1Cache.layer.next = &DRAM;
	L1Cache.layer.readTime = L1_READ_TIME;
	L1Cache.layer.writeTime = L1_WRITE_TIME;
	L1Cache.lines = L1CacheLines;
	L1Cache.associative = 0;

	firstLayer = &L1Cache.layer;

	MemoryLayer* current = firstLayer;
	while (current->next) {
		Cache* cache = MemoryLayerToCache(current);
		for (uint32_t block = 0; block < current->blockCount; block++)
			cache->lines[block].valid = 0;
		if (cache->associative) {
			AssociativeCache* associative = CacheToAssociativeCache(cache);
			uint32_t setCount = current->blockCount / SET_SIZE;
			for (uint32_t set = 0; set < setCount; set++)
				associative->MRUbuffer[set] = 1;
		}
		current = current->next;
	}
}

/*
 * @brief gets the word offset of the given address (not in bytes).
 *
 * @param address, address to get the word offset from.
 *
 * @return word offset.
 */
static uint32_t getWordOffset(uint32_t address) {
	return (address / WORD_SIZE) % (BLOCK_SIZE / WORD_SIZE);
}

/*
 * @brief gets the tag of the given address, relative to the given layer.
 *
 * @param layer, memory layer the tag will be used in.
 * @param address, address to get the tag from.
 *
 * @return tag.
 */
static uint32_t MemoryLayerGetTag(const MemoryLayer* layer, uint32_t address) {
	if (!layer || layer->blockCount == 0)
		exit(-1);
	if (!layer->next || !MemoryLayerToCacheConst(layer)->associative)
		return (address / (layer->blockCount * BLOCK_SIZE));
	return (address / ((layer->blockCount / SET_SIZE) * BLOCK_SIZE));
}

/*
 * @brief gets the set index associated to the address within the given cache.
 *
 * @param cache, cache to get the set of.
 * @param address, address to get the index set of.
 *
 * @return set index of the address.
 */
static uint32_t AssociativeCacheGetSetIndex(const AssociativeCache* cache, uint32_t address) {
	if (!cache)
		exit(-1);
	return (address / BLOCK_SIZE) % (cache->cache.layer.blockCount / SET_SIZE);
}

/*
 * @brief gets the block index of the given address, within the layer.
 *
 * @param layer, memory layer the block index will be used in.
 * @param address, address to get the block index from.
 *
 * @return block index.
 */
static uint32_t MemoryLayerGetBlockIndex(const MemoryLayer* layer, uint32_t address) {
	if (!layer || layer->blockCount == 0)
		exit(-1);
	uint32_t blockIndex = (address / BLOCK_SIZE) % layer->blockCount;
	if (!layer->next || !MemoryLayerToCacheConst(layer)->associative)
		return blockIndex;

	const AssociativeCache* cache = CacheToAssociativeCacheConst(
		MemoryLayerToCacheConst(layer)
	);
	blockIndex = AssociativeCacheGetSetIndex(cache, address) * SET_SIZE;
	uint32_t tag = MemoryLayerGetTag(layer, address);
	for (uint32_t offset = 0; offset < SET_SIZE; offset++) {
		uint32_t current = blockIndex + offset; /* check if the block is stored */
		if (cache->cache.lines[current].valid && cache->cache.lines[current].tag == tag)
			return current;
	}
	return AssociativeCacheGetSetIndex(cache, address) * SET_SIZE
		+ ((cache->MRUbuffer[blockIndex / SET_SIZE] + 1) % SET_SIZE);
}

/*
 * @brief reconstructs the full address from a cached block.
 *
 * @param layer, memory layer the tag was in.
 * @param oldTag, tag stored in the conflicting block.
 * @param newAddress, address that caused the conflict.
 *
 * @return full address (without word offset).
 */
static uint32_t MemoryLayerRecuperateAddress(const MemoryLayer* layer, uint32_t oldTag, uint32_t newAddress) {
	if (!layer || layer->blockCount == 0)
		exit(-1);
	if (!layer->next || !MemoryLayerToCacheConst(layer)->associative)
		return (oldTag * layer->blockCount * BLOCK_SIZE) +
			(newAddress % (layer->blockCount * BLOCK_SIZE));

	const AssociativeCache* cache = CacheToAssociativeCacheConst(
		MemoryLayerToCacheConst(layer)
	);
	return (oldTag * (layer->blockCount / SET_SIZE) * BLOCK_SIZE)
		+ AssociativeCacheGetSetIndex(cache, newAddress) * BLOCK_SIZE;
}

/*
 * @brief writes a full block of data in the given layer, at the
 * block of the given address.
 *
 * @param layer, layer to write in.
 * @param address, address to write the data to.
 * @param data, block of data to write.
 */
static void writeBack(MemoryLayer* layer, uint32_t address, uint8_t* data) {
	if (!data || !layer || !layer->buffer)
		exit(-1);
	if (layer->next) /* mark as dirty (if not DRAM) */
		MemoryLayerToCache(layer)->lines[MemoryLayerGetBlockIndex(layer, address)].dirty = 1;
	memcpy(
		layer->buffer + MemoryLayerGetBlockIndex(layer, address) * BLOCK_SIZE,
		data,
		BLOCK_SIZE
	);
	time += layer->writeTime;
}

/*
 * @brief fetches a full block of data from the given layer.
 *
 * @param layer, layer to get the data from.
 * @param address, address to get the data from.
 * @param data, block of data to fetch into.
 * @param instant, whether or not to increment the time (on the first call).
 */
static void fetch(MemoryLayer* layer, uint32_t address, uint8_t* data, uint8_t instant) {
	if (!layer || !layer->buffer || !data || (instant != 0 && instant != 1))
		exit(-1);
	uint32_t blockIndex = MemoryLayerGetBlockIndex(layer, address);
	if (!layer->next) { /* in case DRAM was reached */
		memcpy(
			data,
			layer->buffer + blockIndex * BLOCK_SIZE,
			BLOCK_SIZE
		);
		time += layer->readTime * ((instant + 1) % 2);
		return;
	}
	CacheLine* line = MemoryLayerToCache(layer)->lines + blockIndex;
	uint32_t tag = MemoryLayerGetTag(layer, address);
	if (!line->valid || line->tag != tag) { /* handle miss */
		if (line->valid && line->dirty) /* if dirty, then write to next layer */
			writeBack(
				layer->next,
				MemoryLayerRecuperateAddress(layer, line->tag, address),
				layer->buffer + blockIndex * BLOCK_SIZE
			);
		fetch(
			layer->next,
			address,
			layer->buffer + blockIndex * BLOCK_SIZE,
			0
		); /* fetch block from next layer */
		line->tag = tag;
		line->dirty = 0;
		line->valid = 1;
	}
	if (MemoryLayerToCache(layer)->associative) { /* mark as most recently used */
		AssociativeCache* cache = CacheToAssociativeCache(MemoryLayerToCache(layer));
		cache->MRUbuffer[AssociativeCacheGetSetIndex(cache, address)] = blockIndex % SET_SIZE;
	}
	memcpy(
		data,
		layer->buffer + blockIndex * BLOCK_SIZE,
		BLOCK_SIZE
	);
	time += layer->readTime * ((instant + 1) % 2);
}

void read(uint32_t address, uint8_t *data) {
	if (address >= DRAM_SIZE - WORD_SIZE + 1 || !firstLayer)
		exit(-1);
	uint8_t temp[BLOCK_SIZE];
	fetch(firstLayer, address, temp, 1);
	memcpy(
		data,
		temp + getWordOffset(address) * WORD_SIZE,
		WORD_SIZE
	);
	time += firstLayer->readTime;
}

void write(uint32_t address, uint8_t *data) {
	if (address >= DRAM_SIZE - WORD_SIZE + 1 || !firstLayer || !firstLayer->buffer)
		exit(-1);
	uint32_t blockIndex = MemoryLayerGetBlockIndex(firstLayer, address);
	uint8_t temp[BLOCK_SIZE];
	fetch(firstLayer, address, temp, 1);
	if (firstLayer->next) /* mark as dirty (if not DRAM) */
		MemoryLayerToCache(firstLayer)->lines[blockIndex].dirty = 1;
	memcpy(
		firstLayer->buffer + blockIndex * BLOCK_SIZE + getWordOffset(address) * WORD_SIZE,
		data,
		WORD_SIZE
	);
	time += firstLayer->writeTime;
}
