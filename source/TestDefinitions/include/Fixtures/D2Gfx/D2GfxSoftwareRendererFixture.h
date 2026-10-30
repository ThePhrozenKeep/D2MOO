#pragma once


#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>
#include <vector>

#include <doctest.h>

#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>

#include <DynamicArray.h>
#include <EqualityMacros.h>
#include <RandomUnsignedInteger.h>

#include <CmnSubtile.h>
#include <D2Gfx.h>

#include <D2Gfx/CmnSubtileVisitorDefinitions.h>
#include <D2Gfx/D2GfxVisitorDefinitions.h>
#include <Fixtures/D2Gfx/D2GfxOriginalDll.h>


// Globals of the D2MOO implementation that are not declared in a header
extern uint8_t** gpTransPalettes;
extern uint8_t* gpPaletteTable[72];
extern D2PaletteStrc gShadowLightPalettes[49];
extern uint8_t* gCmnInfo_pBuffer;
extern int32_t gCmnInfo_nPitch;
extern int32_t gCmnInfo_nHeight;
extern int32_t gCmnInfo_nUnknown;
extern int32_t gnLineOffsets[732];
extern int32_t gnLineOffsetsPitch;
extern int32_t gnClipLeft;
extern int32_t gnClipTop;
extern int32_t gnClipRight;
extern int32_t gnClipBottom;
extern int32_t gnVisTileMinX;
extern int32_t gnVisTileMaxX;
extern int32_t gnVisTileMinY;
extern int32_t gnVisTileMaxY;
extern int32_t gnVisTileUnclippedMinX;
extern int32_t gnVisTileUnclippedMaxX;
extern int32_t gnVisTileUnclippedMinY;
extern int32_t gnVisTileUnclippedMaxY;
extern int32_t gnFloorBlockMinX;
extern int32_t gnFloorBlockMaxX;
extern int32_t gnFloorBlockMinY;
extern int32_t gnFloorBlockMaxY;
extern int32_t gnFloorBlockUnclippedMinX;
extern int32_t gnFloorBlockUnclippedMaxX;
extern int32_t gnFloorBlockUnclippedMinY;
extern int32_t gnFloorBlockUnclippedMaxY;
extern int32_t gnViewportViewMatrixAdjust;
extern int32_t gbForceFlatLighting;
extern uint8_t gGouraudCache[32][32][32];


DYNAMIC_ARRAY_TYPE(uint8_t)


// Copy of the globals of CmnSubtile.cpp, used to compare the state of both implementations
struct D2GfxSoftwareRendererStateStrc
{
	// Pointers into memory owned by each implementation are stored relative to it, see D2GfxSoftwareRendererGlobals::snapshot()
	uint32_t nTransPalettes;
	uint32_t nBufferOffset;
	int32_t nPitch;
	int32_t nHeight;
	int32_t nUnknown;
	uint8_t* pPaletteTable[72];
	D2PaletteStrc ShadowLightPalettes[49];
	int32_t nLineOffsets[732];
	int32_t nLineOffsetsPitch;
	int32_t nClipLeft;
	int32_t nClipTop;
	int32_t nClipRight;
	int32_t nClipBottom;
	int32_t nVisTileMinX;
	int32_t nVisTileMaxX;
	int32_t nVisTileMinY;
	int32_t nVisTileMaxY;
	int32_t nVisTileUnclippedMinX;
	int32_t nVisTileUnclippedMaxX;
	int32_t nVisTileUnclippedMinY;
	int32_t nVisTileUnclippedMaxY;
	int32_t nFloorBlockMinX;
	int32_t nFloorBlockMaxX;
	int32_t nFloorBlockMinY;
	int32_t nFloorBlockMaxY;
	int32_t nFloorBlockUnclippedMinX;
	int32_t nFloorBlockUnclippedMaxX;
	int32_t nFloorBlockUnclippedMinY;
	int32_t nFloorBlockUnclippedMaxY;
	int32_t nViewportViewMatrixAdjust;
	int32_t bForceFlatLighting;
	uint8_t GouraudCache[32][32][32];
};

BEGIN_VISIT(D2GfxSoftwareRendererStateStrc)
	FIELD(nTransPalettes)
	FIELD(nBufferOffset)
	FIELD(nPitch)
	FIELD(nHeight)
	FIELD(nUnknown)
	FIELD(pPaletteTable) // Palettes without a visitor, only the pointers are compared
	ARRAY(ShadowLightPalettes)
	ARRAY(nLineOffsets)
	FIELD(nLineOffsetsPitch)
	FIELD(nClipLeft)
	FIELD(nClipTop)
	FIELD(nClipRight)
	FIELD(nClipBottom)
	FIELD(nVisTileMinX)
	FIELD(nVisTileMaxX)
	FIELD(nVisTileMinY)
	FIELD(nVisTileMaxY)
	FIELD(nVisTileUnclippedMinX)
	FIELD(nVisTileUnclippedMaxX)
	FIELD(nVisTileUnclippedMinY)
	FIELD(nVisTileUnclippedMaxY)
	FIELD(nFloorBlockMinX)
	FIELD(nFloorBlockMaxX)
	FIELD(nFloorBlockMinY)
	FIELD(nFloorBlockMaxY)
	FIELD(nFloorBlockUnclippedMinX)
	FIELD(nFloorBlockUnclippedMaxX)
	FIELD(nFloorBlockUnclippedMinY)
	FIELD(nFloorBlockUnclippedMaxY)
	FIELD(nViewportViewMatrixAdjust)
	FIELD(bForceFlatLighting)
	ARRAY(GouraudCache)
END_VISIT()


// References to the globals of CmnSubtile.cpp of one implementation
struct D2GfxSoftwareRendererGlobals
{
	static constexpr uint32_t NULL_POINTER = 0xFFFFFFFF;
	static constexpr uint32_t OWN_PALETTE_TABLE = 0xFFFFFFFE;

	uint8_t**& pTransPalettes;
	uint8_t*& pBuffer;
	int32_t& nPitch;
	int32_t& nHeight;
	int32_t& nUnknown;
	uint8_t* (&pPaletteTable)[72];
	D2PaletteStrc(&ShadowLightPalettes)[49];
	int32_t(&nLineOffsets)[732];
	int32_t& nLineOffsetsPitch;
	int32_t& nClipLeft;
	int32_t& nClipTop;
	int32_t& nClipRight;
	int32_t& nClipBottom;
	int32_t& nVisTileMinX;
	int32_t& nVisTileMaxX;
	int32_t& nVisTileMinY;
	int32_t& nVisTileMaxY;
	int32_t& nVisTileUnclippedMinX;
	int32_t& nVisTileUnclippedMaxX;
	int32_t& nVisTileUnclippedMinY;
	int32_t& nVisTileUnclippedMaxY;
	int32_t& nFloorBlockMinX;
	int32_t& nFloorBlockMaxX;
	int32_t& nFloorBlockMinY;
	int32_t& nFloorBlockMaxY;
	int32_t& nFloorBlockUnclippedMinX;
	int32_t& nFloorBlockUnclippedMaxX;
	int32_t& nFloorBlockUnclippedMinY;
	int32_t& nFloorBlockUnclippedMaxY;
	int32_t& nViewportViewMatrixAdjust;
	int32_t& bForceFlatLighting;
	uint8_t(&GouraudCache)[32][32][32];

	// pTransPalettes points to the palette table of the implementation, and pBuffer to the back buffer used with it.
	// They are stored relative to them so that both implementations can be compared.
	void snapshot(D2GfxSoftwareRendererStateStrc& state, const uint8_t* pBackBuffer) const
	{
		if (pTransPalettes == nullptr)
		{
			state.nTransPalettes = NULL_POINTER;
		}
		else if (pTransPalettes == &pPaletteTable[0])
		{
			state.nTransPalettes = OWN_PALETTE_TABLE;
		}
		else
		{
			state.nTransPalettes = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(pTransPalettes));
		}

		state.nBufferOffset = pBuffer ? static_cast<uint32_t>(pBuffer - pBackBuffer) : NULL_POINTER;
		state.nPitch = nPitch;
		state.nHeight = nHeight;
		state.nUnknown = nUnknown;
		std::memcpy(state.pPaletteTable, pPaletteTable, sizeof(state.pPaletteTable));
		std::memcpy(state.ShadowLightPalettes, ShadowLightPalettes, sizeof(state.ShadowLightPalettes));
		std::memcpy(state.nLineOffsets, nLineOffsets, sizeof(state.nLineOffsets));
		state.nLineOffsetsPitch = nLineOffsetsPitch;
		state.nClipLeft = nClipLeft;
		state.nClipTop = nClipTop;
		state.nClipRight = nClipRight;
		state.nClipBottom = nClipBottom;
		state.nVisTileMinX = nVisTileMinX;
		state.nVisTileMaxX = nVisTileMaxX;
		state.nVisTileMinY = nVisTileMinY;
		state.nVisTileMaxY = nVisTileMaxY;
		state.nVisTileUnclippedMinX = nVisTileUnclippedMinX;
		state.nVisTileUnclippedMaxX = nVisTileUnclippedMaxX;
		state.nVisTileUnclippedMinY = nVisTileUnclippedMinY;
		state.nVisTileUnclippedMaxY = nVisTileUnclippedMaxY;
		state.nFloorBlockMinX = nFloorBlockMinX;
		state.nFloorBlockMaxX = nFloorBlockMaxX;
		state.nFloorBlockMinY = nFloorBlockMinY;
		state.nFloorBlockMaxY = nFloorBlockMaxY;
		state.nFloorBlockUnclippedMinX = nFloorBlockUnclippedMinX;
		state.nFloorBlockUnclippedMaxX = nFloorBlockUnclippedMaxX;
		state.nFloorBlockUnclippedMinY = nFloorBlockUnclippedMinY;
		state.nFloorBlockUnclippedMaxY = nFloorBlockUnclippedMaxY;
		state.nViewportViewMatrixAdjust = nViewportViewMatrixAdjust;
		state.bForceFlatLighting = bForceFlatLighting;
		std::memcpy(state.GouraudCache, GouraudCache, sizeof(state.GouraudCache));
	}

	// Same computations as CMNSUBTILE_SetViewport, with an explicit left border
	void set_viewport(int32_t nLeft, int32_t nRight, int32_t nViewportHeight)
	{
		nClipLeft = nLeft;
		nClipTop = 0;
		nClipRight = nRight;
		nClipBottom = nViewportHeight - 47;
		nVisTileMinX = nLeft - 32;
		nVisTileMaxX = nRight;
		nVisTileMinY = -32;
		nVisTileMaxY = nViewportHeight - 47;
		nVisTileUnclippedMinX = nLeft;
		nVisTileUnclippedMaxX = nRight - 32;
		nVisTileUnclippedMinY = 0;
		nVisTileUnclippedMaxY = nViewportHeight - 79;
		nFloorBlockMinX = nLeft - 32;
		nFloorBlockMaxX = nRight;
		nFloorBlockMinY = -16;
		nFloorBlockMaxY = nViewportHeight - 47;
		nFloorBlockUnclippedMinX = nLeft;
		nFloorBlockUnclippedMaxX = nRight - 32;
		nFloorBlockUnclippedMinY = 0;
		nFloorBlockUnclippedMaxY = nViewportHeight - 63;
	}

	void set_line_offsets(int32_t nLinePitch)
	{
		nLineOffsetsPitch = nLinePitch;
		for (int32_t i = 0; i < static_cast<int32_t>(std::size(nLineOffsets)); ++i)
		{
			nLineOffsets[i] = (i - 32) * nLinePitch;
		}
	}
};

inline D2GfxSoftwareRendererGlobals make_moo_software_renderer_globals()
{
	return D2GfxSoftwareRendererGlobals{
		gpTransPalettes,
		gCmnInfo_pBuffer,
		gCmnInfo_nPitch,
		gCmnInfo_nHeight,
		gCmnInfo_nUnknown,
		gpPaletteTable,
		gShadowLightPalettes,
		gnLineOffsets,
		gnLineOffsetsPitch,
		gnClipLeft,
		gnClipTop,
		gnClipRight,
		gnClipBottom,
		gnVisTileMinX,
		gnVisTileMaxX,
		gnVisTileMinY,
		gnVisTileMaxY,
		gnVisTileUnclippedMinX,
		gnVisTileUnclippedMaxX,
		gnVisTileUnclippedMinY,
		gnVisTileUnclippedMaxY,
		gnFloorBlockMinX,
		gnFloorBlockMaxX,
		gnFloorBlockMinY,
		gnFloorBlockMaxY,
		gnFloorBlockUnclippedMinX,
		gnFloorBlockUnclippedMaxX,
		gnFloorBlockUnclippedMinY,
		gnFloorBlockUnclippedMaxY,
		gnViewportViewMatrixAdjust,
		gbForceFlatLighting,
		gGouraudCache,
	};
}

inline D2GfxSoftwareRendererGlobals make_original_software_renderer_globals()
{
	// RVAs of the globals in the original dll, see the address comments in CmnSubtile.cpp
	return D2GfxSoftwareRendererGlobals{
		d2gfx_original_global<uint8_t**>(0x00011448),
		d2gfx_original_global<uint8_t*>(0x0001144C),
		d2gfx_original_global<int32_t>(0x00011450),
		d2gfx_original_global<int32_t>(0x00011454),
		d2gfx_original_global<int32_t>(0x00011458),
		d2gfx_original_global<uint8_t*[72]>(0x00011460),
		d2gfx_original_global<D2PaletteStrc[49]>(0x00011580),
		d2gfx_original_global<int32_t[732]>(0x000146A8),
		d2gfx_original_global<int32_t>(0x00014680),
		d2gfx_original_global<int32_t>(0x0001D220),
		d2gfx_original_global<int32_t>(0x00014690),
		d2gfx_original_global<int32_t>(0x0001469C),
		d2gfx_original_global<int32_t>(0x0001D240),
		d2gfx_original_global<int32_t>(0x00014688),
		d2gfx_original_global<int32_t>(0x00015218),
		d2gfx_original_global<int32_t>(0x0001D230),
		d2gfx_original_global<int32_t>(0x0001D24C),
		d2gfx_original_global<int32_t>(0x000146A0),
		d2gfx_original_global<int32_t>(0x0001D234),
		d2gfx_original_global<int32_t>(0x0001D238),
		d2gfx_original_global<int32_t>(0x0001D248),
		d2gfx_original_global<int32_t>(0x0001D244),
		d2gfx_original_global<int32_t>(0x00014698),
		d2gfx_original_global<int32_t>(0x0001D224),
		d2gfx_original_global<int32_t>(0x0001D23C),
		d2gfx_original_global<int32_t>(0x0001468C),
		d2gfx_original_global<int32_t>(0x0001D22C),
		d2gfx_original_global<int32_t>(0x0001D228),
		d2gfx_original_global<int32_t>(0x00014694),
		d2gfx_original_global<int32_t>(0x0000E040),
		d2gfx_original_global<int32_t>(0x0001D250),
		d2gfx_original_global<uint8_t[32][32][32]>(0x00015220),
	};
}


// RLE encoded data as drawn by the software renderer: (skip, length, pixels[length]) spans, a (0, 0) span ends the current line.
inline std::vector<uint8_t> generate_rle_data(int32_t nLines, int32_t nWidth)
{
	std::vector<uint8_t> data;
	for (int32_t nLine = 0; nLine < nLines; ++nLine)
	{
		int32_t nX = 0;
		while (nX < nWidth && random_unsigned_integer(0, 3) != 0)
		{
			const int32_t nRemaining = nWidth - nX;
			if (random_unsigned_integer(0, 7) == 0)
			{
				// Transparent span without pixels
				const auto nSkip = static_cast<uint8_t>(random_unsigned_integer(1, nRemaining));
				data.push_back(nSkip);
				data.push_back(0);
				nX += nSkip;
				continue;
			}

			const auto nSkip = static_cast<uint8_t>(random_unsigned_integer(0, nRemaining - 1));
			const auto nLength = static_cast<uint8_t>(random_unsigned_integer(1, nRemaining - nSkip));
			data.push_back(nSkip);
			data.push_back(nLength);
			for (int32_t i = 0; i < nLength; ++i)
			{
				data.push_back(static_cast<uint8_t>(random_unsigned_integer(0, 255)));
			}
			nX += nSkip + nLength;
		}

		data.push_back(0);
		data.push_back(0);
	}

	return data;
}

inline std::vector<uint8_t> generate_random_bytes(size_t nSize, uint32_t nMax = 255)
{
	std::vector<uint8_t> data(nSize);
	for (auto& value : data)
	{
		value = static_cast<uint8_t>(random_unsigned_integer(0, nMax));
	}

	return data;
}


// Compares two byte buffers, the visitor is only used to describe the differences since it is slow for big buffers
inline void check_bytes_eq(uint8_t* moo_data, uint8_t* original_data, size_t nSize, const char* context_title)
{
	const bool are_bytes_equal = std::memcmp(moo_data, original_data, nSize) == 0;
	CHECK_MESSAGE(are_bytes_equal, context_title);
	if (!are_bytes_equal)
	{
		DynamicArray<uint8_t> moo_array{ moo_data, static_cast<int>(nSize) };
		DynamicArray<uint8_t> original_array{ original_data, static_cast<int>(nSize) };
		MOO_CHECK_EQ(moo_array, original_array, context_title);
	}
}

inline void check_bytes_eq(std::vector<uint8_t>& moo_data, std::vector<uint8_t>& original_data, const char* context_title)
{
	REQUIRE_EQ(moo_data.size(), original_data.size());
	check_bytes_eq(moo_data.data(), original_data.data(), moo_data.size(), context_title);
}


// Gives both implementations a back buffer, palettes, a gouraud cache and a viewport with the same content.
// The back buffers have margins above and below the visible lines to catch out of bounds writes.
template<class Fixture>
struct D2GfxSoftwareRendererFixture : Fixture
{
	static constexpr int32_t buffer_width = 320;
	static constexpr int32_t buffer_height = 240;
	static constexpr int32_t buffer_pitch = buffer_width;
	static constexpr int32_t buffer_margin_lines = 64;
	static constexpr size_t buffer_size = static_cast<size_t>(buffer_pitch) * (buffer_height + 2 * buffer_margin_lines);
	static constexpr size_t blend_table_size = 256 * 256;

	D2GfxSoftwareRendererGlobals moo_renderer = make_moo_software_renderer_globals();
	D2GfxSoftwareRendererGlobals original_renderer = make_original_software_renderer_globals();

	std::vector<uint8_t> moo_buffer = std::vector<uint8_t>(buffer_size);
	std::vector<uint8_t> original_buffer = std::vector<uint8_t>(buffer_size);

	// Palettes referenced by the palette table, shared by both implementations since they are only read
	std::vector<uint8_t> blend_tables[3];
	std::vector<uint8_t> palettes[72 - 3];
	D2PaletteTableStrc palette_table{};

	std::unique_ptr<D2GfxSoftwareRendererStateStrc> moo_state = std::make_unique<D2GfxSoftwareRendererStateStrc>();
	std::unique_ptr<D2GfxSoftwareRendererStateStrc> original_state = std::make_unique<D2GfxSoftwareRendererStateStrc>();

	D2GfxSoftwareRendererFixture()
	{
		for (auto& blend_table : blend_tables)
		{
			blend_table = generate_random_bytes(blend_table_size);
		}
		for (auto& palette : palettes)
		{
			palette = generate_random_bytes(sizeof(D2PaletteStrc));
		}

		void** pPaletteTableEntries = reinterpret_cast<void**>(&palette_table);
		for (int32_t i = 0; i < 3; ++i)
		{
			pPaletteTableEntries[i] = blend_tables[i].data();
		}
		for (int32_t i = 3; i < 72; ++i)
		{
			pPaletteTableEntries[i] = palettes[i - 3].data();
		}

		const auto buffer_content = generate_random_bytes(buffer_size);
		const auto shadow_light_palettes = generate_random_bytes(sizeof(D2PaletteStrc) * 49);
		// Values of the gouraud cache are indices into the shadow light palettes
		const auto gouraud_cache = generate_random_bytes(32 * 32 * 32, 31);

		const auto setup_data = [&](
			D2GfxSoftwareRendererGlobals& renderer,
			std::vector<uint8_t>& buffer
		) {
			std::memcpy(buffer.data(), buffer_content.data(), buffer_size);
			renderer.pBuffer = buffer.data() + buffer_margin_lines * buffer_pitch;
			renderer.nPitch = buffer_pitch;
			renderer.nHeight = buffer_height;
			renderer.nUnknown = 0;
			renderer.set_line_offsets(buffer_pitch);
			renderer.set_viewport(0, buffer_width, buffer_height);
			renderer.nViewportViewMatrixAdjust = -1;
			renderer.bForceFlatLighting = 0;
			std::memcpy(renderer.pPaletteTable, &palette_table, sizeof(renderer.pPaletteTable));
			renderer.pTransPalettes = &renderer.pPaletteTable[0];
			std::memcpy(renderer.ShadowLightPalettes, shadow_light_palettes.data(), sizeof(renderer.ShadowLightPalettes));
			std::memcpy(renderer.GouraudCache, gouraud_cache.data(), sizeof(renderer.GouraudCache));
		};

		setup_data(moo_renderer, moo_buffer);
		setup_data(original_renderer, original_buffer);
	}

	~D2GfxSoftwareRendererFixture()
	{
		// Do not leave dangling pointers to the buffers and palettes
		const auto reset_data = [](
			D2GfxSoftwareRendererGlobals& renderer
		) {
			renderer.pBuffer = nullptr;
			std::memset(renderer.pPaletteTable, 0, sizeof(renderer.pPaletteTable));
		};

		reset_data(moo_renderer);
		reset_data(original_renderer);
	}

	uint8_t* moo_back_buffer()
	{
		return moo_buffer.data() + buffer_margin_lines * buffer_pitch;
	}

	uint8_t* original_back_buffer()
	{
		return original_buffer.data() + buffer_margin_lines * buffer_pitch;
	}

	// Address of a pixel of the back buffers, the position can be in the margins
	uint8_t* moo_pixel(int32_t nX, int32_t nY)
	{
		return moo_back_buffer() + nY * buffer_pitch + nX;
	}

	uint8_t* original_pixel(int32_t nX, int32_t nY)
	{
		return original_back_buffer() + nY * buffer_pitch + nX;
	}

	// Sets the same viewport for both implementations, as CMNSUBTILE_SetViewport would for the back buffer and the given gnViewMatrixAdjust:
	// 0 for the whole width, 1 for the left half and 2 for the right half (when a panel is open)
	void set_viewport(int32_t nViewMatrixAdjust)
	{
		int32_t nLeft = 0;
		int32_t nRight = buffer_width;
		if (nViewMatrixAdjust == 1)
		{
			nRight = buffer_width - buffer_width / 2;
		}
		else if (nViewMatrixAdjust == 2)
		{
			nLeft = buffer_width / 2;
		}

		moo_renderer.set_viewport(nLeft, nRight, buffer_height);
		original_renderer.set_viewport(nLeft, nRight, buffer_height);
	}

	// Snapshots the globals of both implementations and compares them
	void check_renderer_eq(const char* context_title)
	{
		moo_renderer.snapshot(*moo_state, moo_back_buffer());
		original_renderer.snapshot(*original_state, original_back_buffer());

		// The visitor builds a description for each of the ~50000 elements, only use it when there are differences
		const bool are_states_equal = std::memcmp(moo_state.get(), original_state.get(), sizeof(D2GfxSoftwareRendererStateStrc)) == 0;
		CHECK_MESSAGE(are_states_equal, context_title);
		if (!are_states_equal)
		{
			MOO_CHECK_EQ(*moo_state, *original_state, context_title);
		}
	}

	// Compares the whole back buffers, including the margins
	void check_buffers_eq(const char* context_title)
	{
		check_bytes_eq(moo_buffer.data(), original_buffer.data(), buffer_size, context_title);
	}
};
